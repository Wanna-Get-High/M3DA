/* --------------------------------------------------------------------------
Mise en correspondance de points d'interet detectes dans deux images
Copyright (C) 2010, 2011  Universite Lille 1

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
-------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------
Inclure les fichiers d'entete
-------------------------------------------------------------------------- */
#include <stdio.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

#include "glue.hpp"
#include "francois-lepan.hpp"

// -----------------------------------------------------------------------
/// \brief Detecte les coins.
///
/// @param mImage: pointeur vers la structure image openCV
/// @param iMaxCorners: nombre maximum de coins detectes
/// @return matrice des coins
// -----------------------------------------------------------------------
Mat iviDetectCorners(const Mat& mImage,
                     int iMaxCorners) {

    vector<Point2d> corners;
    double qualityLevel = 0.01;
    double minDistance = 10;
    int blockSize = 3;
    bool useHarrisDetector = false;
    double k = 0.04;

    cv::goodFeaturesToTrack(mImage,             // InputArray image
                            corners,            // OutputArray corners
                            iMaxCorners,        // int maxCorners
                            qualityLevel,       // double qualityLevel
                            minDistance,        // double minDistance
                            Mat(),              // InputArray mask=noArray()
                            blockSize,          // int blockSize=3
                            useHarrisDetector,  // bool useHarrisDetector=false
                            k                   // double k=0.04
                            );

    int size = corners.size();
    Mat mCorners = Mat_<double>(3,size);

    for (int i = 0; i < size; ++i) {
        mCorners.at<double>(0,i) = corners[i].x;
        mCorners.at<double>(1,i) = corners[i].y;
        mCorners.at<double>(2,i) = 1;
    }

    return mCorners;
}

// -----------------------------------------------------------------------
/// \brief Initialise une matrice de produit vectoriel.
///
/// @param v: vecteur colonne (3 coordonnees)
/// @return matrice de produit vectoriel
// -----------------------------------------------------------------------
Mat iviVectorProductMatrix(const Mat& v) {

    Mat mVectorProduct = Mat::eye(3, 3, CV_64F);

    double x = v.at<double>(0,0);
    double y = v.at<double>(0,1);
    double z = v.at<double>(0,2);


    mVectorProduct.at<double>(0,0) = 0;
    mVectorProduct.at<double>(0,1) = -z;
    mVectorProduct.at<double>(0,2) = y;
    mVectorProduct.at<double>(1,0) = z;
    mVectorProduct.at<double>(1,1) = 0;
    mVectorProduct.at<double>(1,2) = -x;
    mVectorProduct.at<double>(2,0) = -y;
    mVectorProduct.at<double>(2,1) = x;
    mVectorProduct.at<double>(2,2) = 0;

    return mVectorProduct;
}

// -----------------------------------------------------------------------
/// \brief Initialise et calcule la matrice fondamentale.
///
/// @param mLeftIntrinsic: matrice intrinseque de la camera gauche
/// @param mLeftExtrinsic: matrice extrinseque de la camera gauche
/// @param mRightIntrinsic: matrice intrinseque de la camera droite
/// @param mRightExtrinsic: matrice extrinseque de la camera droite
/// @return matrice fondamentale
// -----------------------------------------------------------------------
Mat iviFundamentalMatrix(const Mat& mLeftIntrinsic,
                         const Mat& mLeftExtrinsic,
                         const Mat& mRightIntrinsic,
                         const Mat& mRightExtrinsic) {

    // matrice 4*1
    Mat origine = Mat_<double>(4,1);
    origine.at<double>(0) = 0;
    origine.at<double>(1) = 0;
    origine.at<double>(2) = 0;
    origine.at<double>(3) = 1;

    // matrice identité 3*4
    Mat eye34 = Mat::eye(3, 4, CV_64F);

    // (4*1) = (4*4) * (4*1)
    Mat oL = mLeftExtrinsic.inv() * origine;

    //  (3*4) = (3*3 * (3*4) * (4*4)
    // projection Gauche
    Mat pL = mLeftIntrinsic * eye34 * mLeftExtrinsic;

    //  (3*4) = (3*3) * (3*4) * (4*4)
    // projection Droite
    Mat pR = mRightIntrinsic * eye34 * mRightExtrinsic;

    // projection Gauche pseudo-inversé
    Mat pLPlus = pL.inv(DECOMP_SVD);

    // (3*3) = (3*3) * (3*4) * (4*4)
    // calcule de la matrice fondamentale à partir des projection de pL et pR
    Mat mFundamental = iviVectorProductMatrix(pR*oL) * pR*pLPlus;

    // Retour de la matrice fondamentale
    return mFundamental;
}

// -----------------------------------------------------------------------
/// \brief Initialise et calcule la matrice des distances entres les
/// points de paires candidates a la correspondance.
///
/// @param mLeftCorners: liste des points 2D image gauche
/// @param mRightCorners: liste des points 2D image droite
/// @param mFundamental: matrice fondamentale
/// @return matrice des distances entre points des paires
// -----------------------------------------------------------------------
Mat iviDistancesMatrix(const Mat& m2DLeftCorners,
                       const Mat& m2DRightCorners,
                       const Mat& mFundamental) {

    int nbPointsImgLeft = m2DLeftCorners.cols;
    int nbPointsImgRight = m2DRightCorners.cols;

    Mat droitesEpipolaireDepuisImageGauche = mFundamental.t() * m2DLeftCorners;

    Mat droitesEpipolaireDepuisImageDroite = mFundamental * m2DRightCorners;

    Mat mDistances = Mat_<double>(nbPointsImgLeft,nbPointsImgRight);
    Mat mDistances1 = Mat_<double>(nbPointsImgRight,nbPointsImgLeft);

    // distances entre "corners" image de droite avec
    // la droite epipolaire sur image droite
    for (int i = 0; i < nbPointsImgLeft; ++i) {

        double dx = droitesEpipolaireDepuisImageGauche.at<double>(0,i);
        double dy = droitesEpipolaireDepuisImageGauche.at<double>(1,i);
        double dz = droitesEpipolaireDepuisImageGauche.at<double>(2,i);

        for (int j=0; j< nbPointsImgRight; ++j) {

            double px = m2DRightCorners.at<double>(0,j);
            double py = m2DRightCorners.at<double>(1,j);

            mDistances.at<double>(i,j) = abs(dx*px + dy*py + dz) / sqrt(pow(dx,2) + pow(dy,2));
        }
    }

    // distances entre "corners" image de gauche avec
    // la droite epipolaire sur image gauche
    for (int i = 0; i < nbPointsImgRight; ++i) {

        double dx = droitesEpipolaireDepuisImageDroite.at<double>(0,i);
        double dy = droitesEpipolaireDepuisImageDroite.at<double>(1,i);
        double dz = droitesEpipolaireDepuisImageDroite.at<double>(2,i);

        for (int j=0; j< nbPointsImgLeft; ++j) {

            double px = m2DLeftCorners.at<double>(0,j);
            double py = m2DLeftCorners.at<double>(1,j);

            mDistances1.at<double>(i,j) = abs(dx*px + dy*py + dz) / sqrt(pow(dx,2) + pow(dy,2));
        }
    }

    return mDistances + mDistances1.t();
}

// -----------------------------------------------------------------------
/// \brief Initialise et calcule les indices des points homologues.
///
/// @param mDistances: matrice des distances
/// @param fMaxDistance: distance maximale autorisant une association
/// @param mRightHomologous: liste des correspondants des points gauche
/// @param mLeftHomologous: liste des correspondants des points droite
/// @return rien
// -----------------------------------------------------------------------
void iviMarkAssociations(Mat& mDistances,
                         double dMaxDistance,
                         Mat& mRightHomologous,
                         Mat& mLeftHomologous) {
    int nbTurn = 0;
    int index = 0;

    mRightHomologous = initMatrix(mDistances.cols);
    mLeftHomologous = initMatrix(mDistances.rows);

    if (mDistances.rows > mDistances.cols)  nbTurn = mDistances.cols;
    else                                    nbTurn = mDistances.rows;

    int x;
    int y;

    while (index < nbTurn) {
        if (findMinValue(mDistances,x,y,dMaxDistance)) {

            mRightHomologous.at<int>(x) = y;
            mLeftHomologous.at<int>(y) = x;

            removeLigneColFrom(mDistances,x,y);
        }
        index++;
    }

    cout << mRightHomologous << endl;
    cout << mLeftHomologous << endl;
}


// -----------------------------------------------------------------------
/// \brief initialise une matrice avec une taille de size remplit de -1
///
/// @param size: taille de la matrice
/// @return rien
// -----------------------------------------------------------------------
Mat initMatrix(int size) {
    Mat matrix = Mat_<int>(size,1);

    for (int i = 0; i < size; ++i) {
        matrix.at<int>(i) = -1;
    }

    return matrix;
}

// -----------------------------------------------------------------------
/// \brief met des -1 sur toute la ligne i et la colonne j.
///
/// @param mDistances: matrice des distances
/// @param x: position de la ligne
/// @param y: position de la colonne
/// @return rien
// -----------------------------------------------------------------------
void removeLigneColFrom(Mat& mDistances, int x, int y) {
    for (int i = 0; i < mDistances.rows; ++i) {
        mDistances.at<double>(i,y) = -1;
    }

    for (int i = 0; i < mDistances.cols; ++i) {
        mDistances.at<double>(x,i) = -1;
    }
}


// -----------------------------------------------------------------------
/// \brief recupere la valeur maximum contenu dans la matrice maxDistances et stock dans i et j sa position.
///
/// @param mDistances: matrice des distances
/// @param x: position de la ligne
/// @param y: position de la colonne
/// @param maxDist: distance maximale
/// @return rien
// -----------------------------------------------------------------------
bool findMinValue(Mat mDistances, int& x, int& y, int maxDist) {
    double min = maxDist;
    bool found = false;

    for (int i = 0; i < mDistances.rows; ++i) {
        for (int j = 0; j < mDistances.cols; ++j) {
            double val = mDistances.at<double>(i,j);
            if (val != -1 && val < maxDist && val < min) {
                min = val;
                x = i;
                y = j;
                found = true;
            }
        }
    }

    return found;
}
