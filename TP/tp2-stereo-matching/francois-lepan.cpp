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

    cout << iviVectorProductMatrix(pR*oL) << endl;

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

    Mat mDistances = Mat_<double>(nbPointsImgLeft,nbPointsImgRight);

    Mat mFundamentalTransposed = mFundamental.t();


    // la droite épipolaire de pi2 (image de droite)
    // associée à m1 (un coin de image de gauche) est la droite d2 = Fm1
    // -> calculer la distance d'un point de pi2 -> d2.
    // Mat droiteEpipolaire =  mFundamental * m2DLeftCorners.col(i);
    // double distance = distance_between(m2DRightCorners.col(j), droiteEpipolaire);
    for (int i = 0; i < nbPointsImgLeft; ++i) {
        Mat currentPointLeftImage = m2DLeftCorners.col(i);
        Mat droiteEpipolaireImageDroite =  mFundamental * currentPointLeftImage;

        // la droite épipolaire de pi1 (image de gauche)
        // associée à m2 (un coin de image de droite) est la droite d1 = FT m2
        // -> calculer la distance d'un point de pi1 -> d1.
        // Mat droiteEpipolaire =  mFundamental.t() * m2DRightCorners.col(j);
        // double distance = distance_between(m2DRightCorners.col(j), droiteEpipolaire);
        for (int j=0; j< nbPointsImgRight; ++j) {
            Mat currentPointLeftImage = m2DRightCorners.col(j);
            Mat droiteEpipolaireImageGauche =  mFundamentalTransposed * currentPointLeftImage;

            double x1 = droiteEpipolaireImageDroite.at<double>(0);
            double y1 = droiteEpipolaireImageDroite.at<double>(1);
            double z1 = droiteEpipolaireImageDroite.at<double>(2);

            double x2 = m2DRightCorners.at<double>(0,j);
            double y2 = m2DRightCorners.at<double>(1,j);
            double z2 = m2DRightCorners.at<double>(2,j);

            double distR = hypot(hypot(x1-x2,y1-y2),z1-z2);

            double x3 = droiteEpipolaireImageGauche.at<double>(0);
            double y3 = droiteEpipolaireImageGauche.at<double>(1);
            double z3 = droiteEpipolaireImageGauche.at<double>(2);

            double x4 = currentPointLeftImage.at<double>(0);
            double y4 = currentPointLeftImage.at<double>(1);
            double z4 = currentPointLeftImage.at<double>(2);

            double distG = hypot(hypot(x3-x4,y3-y4),z3-z4);

            // on additionne les distances
            mDistances.at<double>(i,j) = distR + distG;
        }
    }

    // Retour de la matrice des distances
    return mDistances;
}

// -----------------------------------------------------------------------
/// \brief Initialise et calcule les indices des points homologues.
///
/// @param mDistances: matrice des distances
/// @param dMaxDistance: distance maximale autorisant une association
/// @param mRightHomologous: liste des correspondants des points gauche
/// @param mLeftHomologous: liste des correspondants des points droite
/// @return rien
// -----------------------------------------------------------------------
void iviMarkAssociations(const Mat& mDistances,
                         double dMaxDistance,
                         Mat& mRightHomologous,
                         Mat& mLeftHomologous) {


    // A modifier !
    cout << mRightHomologous << endl;
    cout << mLeftHomologous << endl;
}



// -----------------------------------------------------------------------
/// \brief Imprime les valeurs flotante contenues dans la matrice Mat.
///
/// @param mat: matrice à imprimer
/// @return rien
// -----------------------------------------------------------------------
void printDoubleMatrix(Mat mat){
    for (int i = 0; i < mat.rows; ++i) {
        for (int j = 0; j < mat.cols; ++j) {
            cout << mat.at<double>(i,j) << " ";
        }
        cout << endl;
    }
}

