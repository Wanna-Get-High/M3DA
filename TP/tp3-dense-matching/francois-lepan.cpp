/* --------------------------------------------------------------------------
Stereovision dense par calcul de correlation
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
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>
#include <stream.h>
using namespace cv;
using namespace std;
#include "francois-lepan.hpp"

// -----------------------------------------------------------------------
/// \brief Estime la disparite par minimisation du SSD, image gauchee
/// prise comme reference.
///
/// @param psLeftImage: image gauche
/// @param psRightImage: image droite
/// @param psLeftDisparity: carte des disparites calculee
/// @param iMaxDisparity: disparite maximale recherchee
/// @param iWindowHalfSize: demi-taille de la fenetre de correlation
/// @return rien
// -----------------------------------------------------------------------
Mat iviLeftDisparityMap(const Mat& mLeftGray,
                        const Mat& mRightGray,
                        int iMaxDisparity,
                        int iWindowHalfSize) {

    // Images pour les resultats intermediaires
    Mat mSSD(mLeftGray.size(), CV_64F);
    Mat mMinSSD(mLeftGray.size(), CV_64F);
    Mat mLeftDisparityMap(mLeftGray.size(), CV_8U);

    double dMinSSD, *pdPtr1, *pdPtr2;
    unsigned char *pucDisparity;
    int iShift, iRow, iCol;


    // Initialisation de l'image du minimum de SSD
    dMinSSD = pow((double)(2 * iWindowHalfSize + 1), 2.0) * 512.0;
    for (iRow = iWindowHalfSize; iRow < mMinSSD.size().height - iWindowHalfSize; iRow++) {

        // Pointeur sur le debut de la ligne
        pdPtr1 = mMinSSD.ptr<double>(iRow);

        // Sauter la demi fenetre non utilisee
        pdPtr1 += iWindowHalfSize;

        // Remplir le reste de la ligne
        for (iCol = iWindowHalfSize; iCol < mMinSSD.size().width - iWindowHalfSize; iCol++)
            *pdPtr1++ = dMinSSD;
    }


    // Boucler pour tous les decalages possibles
    for (iShift = 0; iShift < iMaxDisparity; iShift++) {

        // Calculer le cout SSD pour ce decalage
        mSSD = iviComputeLeftSSDCost(mLeftGray, mRightGray,
                                     iShift, iWindowHalfSize);
        // Mettre a jour les valeurs minimales
        for (iRow = iWindowHalfSize;
             iRow < mMinSSD.size().height - iWindowHalfSize;
             iRow++) {
            // Pointeurs vers les debuts des lignes
            pdPtr1 = mMinSSD.ptr<double>(iRow);
            pdPtr2 = mSSD.ptr<double>(iRow);
            pucDisparity = mLeftDisparityMap.ptr<unsigned char>(iRow);

            // Sauter la demi fenetre non utilisee
            pdPtr1 += iWindowHalfSize;
            pdPtr2 += iWindowHalfSize;
            pucDisparity += iWindowHalfSize;

            // Comparer sur le reste de la ligne
            for (iCol = iWindowHalfSize;
                 iCol < mMinSSD.size().width - iWindowHalfSize;
                 iCol++) {
                // SSD plus faible que le minimum precedent
                if (*pdPtr1 > *pdPtr2) {
                    *pucDisparity = (unsigned char)iShift;
                    *pdPtr1 = *pdPtr2;
                }
                // Pixels suivants
                pdPtr1++; pdPtr2++; pucDisparity++;
            }
        }
    }
    return mLeftDisparityMap;
}


// -----------------------------------------------------------------------
/// \brief Calcule la somme des differences aux carre, image gauche
/// prise comme reference.
///
/// @param psLeftImage: image gauche
/// @param psRightImage: image droite
/// @param psSSD: somme des differences au carre (double 64bits)
/// @param iShift: decalage teste
/// @param iWindowHalfSize: demi-taille de la fenetre de correlation
/// @return rien
// -----------------------------------------------------------------------
Mat iviComputeLeftSSDCost(const Mat& mLeftGray,
                          const Mat& mRightGray,
                          int iShift,
                          int iWindowHalfSize) {

    Mat mLeftSSDCost(mLeftGray.size(), CV_64F);

    // Pour tous les pixels avec le décalage de iWindowHalfSize
    for(int x = iWindowHalfSize; x <= (mLeftGray.rows - iWindowHalfSize); x++) {
        for(int y = iWindowHalfSize + iShift; y <= (mLeftGray.cols - iWindowHalfSize); y++) {

            double temp = 0;

            // calcule du SSD pour le voisinage
            for(int i = -iWindowHalfSize; i <= iWindowHalfSize; i++) {
                for(int j= -iWindowHalfSize; j <= iWindowHalfSize; j++) {
                    temp += pow( (double)mLeftGray.at<unsigned char>(x+i,y+j) - (double)mRightGray.at<unsigned char>(x+i,y+j-iShift), 2);
                }
            }

            mLeftSSDCost.at<double>(x,y) = temp;
        }
    }

    return mLeftSSDCost;
}



// -----------------------------------------------------------------------indice
/// \brief Estime la disparite par minimisation du SSD, image droite
/// prise comme reference.
///
/// @param psLeftImage: image gauche
/// @param psRightImage: image droite
/// @param psRightDisparity: carte des disparites calculee
/// @param iMaxDisparity: disparite maximale recherchee
/// @param iWindowHalfSize: demi-taille de la fenetre de correlation
/// @return rien
// -----------------------------------------------------------------------
Mat iviRightDisparityMap(const Mat& mLeftGray,
                         const Mat& mRightGray,
                         int iMaxDisparity,
                         int iWindowHalfSize) {

    // Images pour les resultats intermediaires
    Mat mSSD(mRightGray.size(), CV_64F);
    Mat mMinSSD(mRightGray.size(), CV_64F);
    Mat mRightDisparityMap(mRightGray.size(), CV_8U);

    double dMinSSD, *pdPtr1, *pdPtr2;
    unsigned char *pucDisparity;
    int iShift, iRow, iCol;


    // Initialisation de l'image du minimum de SSD
    dMinSSD = pow((double)(2 * iWindowHalfSize + 1), 2.0) * 512.0;
    for (iRow = iWindowHalfSize; iRow < mMinSSD.size().height - iWindowHalfSize; iRow++) {

        // Pointeur sur le debut de la ligne
        pdPtr1 = mMinSSD.ptr<double>(iRow);

        // Sauter la demi fenetre non utilisee
        pdPtr1 += iWindowHalfSize;

        // Remplir le reste de la ligne
        for (iCol = iWindowHalfSize; iCol < mMinSSD.size().width - iWindowHalfSize; iCol++)
            *pdPtr1++ = dMinSSD;
    }


    // Boucler pour tous les decalages possibles
    for (iShift = 0; iShift < iMaxDisparity; iShift++) {

        // Calculer le cout SSD pour ce decalage
        mSSD = iviComputeRightSSDCost(mLeftGray, mRightGray,
                                     iShift, iWindowHalfSize);
        // Mettre a jour les valeurs minimales
        for (iRow = iWindowHalfSize;
             iRow < mMinSSD.size().height - iWindowHalfSize;
             iRow++) {
            // Pointeurs vers les debuts des lignes
            pdPtr1 = mMinSSD.ptr<double>(iRow);
            pdPtr2 = mSSD.ptr<double>(iRow);
            pucDisparity = mRightDisparityMap.ptr<unsigned char>(iRow);

            // Sauter la demi fenetre non utilisee
            pdPtr1 += iWindowHalfSize;
            pdPtr2 += iWindowHalfSize;
            pucDisparity += iWindowHalfSize;

            // Comparer sur le reste de la ligne
            for (iCol = iWindowHalfSize;
                 iCol < mMinSSD.size().width - iWindowHalfSize;
                 iCol++) {
                // SSD plus faible que le minimum precedent
                if (*pdPtr1 > *pdPtr2) {
                    *pucDisparity = (unsigned char)iShift;
                    *pdPtr1 = *pdPtr2;
                }
                // Pixels suivants
                pdPtr1++; pdPtr2++; pucDisparity++;
            }
        }
    }

    return mRightDisparityMap;

}


// -----------------------------------------------------------------------
/// \brief Calcule la somme des differences aux carre, image droite
/// prise comme reference.
///
/// @param psLeftImage: image gauche
/// @param psRightImage: image droite
/// @param psSSD: somme des differences au carre (double 64bits)
/// @param iShift: decalage teste
/// @param iWindowHalfSize: demi-taille de la fenetre de correlation
/// @return rien
// -----------------------------------------------------------------------
Mat iviComputeRightSSDCost(const Mat& mLeftGray,
                           const Mat& mRightGray,
                           int iShift,
                           int iWindowHalfSize) {

    Mat mRightSSDCost(mRightGray.size(), CV_64F);

    // Pour tous les pixels avec le décalage de iWindowHalfSize
    for(int x = iWindowHalfSize; x <= (mRightGray.rows - iWindowHalfSize); x++) {
        for(int y = iWindowHalfSize; y <= (mRightGray.cols - iWindowHalfSize - iShift); y++) {

            double temp = 0;

            // calcule du SSD pour le voisinage
            for(int i = -iWindowHalfSize; i <= iWindowHalfSize; i++) {
                for(int j= -iWindowHalfSize; j <= iWindowHalfSize; j++) {
                    temp += pow((double)mRightGray.at<unsigned char>(x+i,y+j) - (double)mLeftGray.at<unsigned char>(x+i,y+j+iShift), 2);
                }
            }

            mRightSSDCost.at<double>(x,y) = temp;
        }
    }

    return mRightSSDCost;
}

// -----------------------------------------------------------------------
/// \brief Verifie la coherence des cartes gauche et froite.
///
/// @param psLeftDisparity: carte gauche des disparites
/// @param psRightDisparity: carte droite des disparites
/// @param psDisparity: carte des disparites fusionnee
/// @param psValidityMask: carte des disparites valides
/// @return rien
// -----------------------------------------------------------------------
Mat iviLeftRightConsistency(const Mat& mLeftDisparity,
                            const Mat& mRightDisparity,
                            Mat& mValidityMask) {

    Mat mDisparity(mLeftDisparity.size(), CV_8U);

    for (int i = 0; i < mDisparity.rows; ++i) {
        for (int j = 0; j < mDisparity.cols; ++j) {

            double dispariteGauche = (double)mLeftDisparity.at<unsigned char>(i,j);
            double dispariteGaucheCorrespondant = (double)mRightDisparity.at<unsigned char>(i, j - dispariteGauche);

            double dispariteDroite = (double)mRightDisparity.at<unsigned char>(i,j);
            double dispariteDroiteCorrespondant = (double)mLeftDisparity.at<unsigned char>(i, j + dispariteDroite);

            if (dispariteGauche != dispariteGaucheCorrespondant || dispariteDroite != dispariteDroiteCorrespondant) {
                mValidityMask.at<unsigned char>(i,j) = 255;
            } else {
                mDisparity.at<unsigned char>(i,j) = dispariteGauche;
            }
        }
    }

    return mDisparity;
}
