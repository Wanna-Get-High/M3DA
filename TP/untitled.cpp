Mat iviDistancesMatrix(const Mat& m2DLeftCorners,
                       const Mat& m2DRightCorners,
                       const Mat& mFundamental) {

    int nbpointLeft=m2DLeftCorners.cols;
    int nbpointRight=m2DLeftCorners.cols;

    //std::cout <<m2DLeftCorners<< std::endl;
    Mat mDroiteFromLeft=mFundamental.t()*m2DLeftCorners;
   // std::cout << mDroiteFromLeft.cols<< " : " << mDroiteFromLeft.rows << std::endl;
   // std::cout <<mDroiteFromLeft<< std::endl;
    Mat mDroiteFromRight=mFundamental*m2DRightCorners;

    // A modifier !
    Mat mDistances = Mat::zeros(nbpointLeft,nbpointRight,CV_64F);
    Mat mDistances2 = Mat::zeros(nbpointRight,nbpointLeft,CV_64F);

    for(int i=0;i<nbpointLeft;++i){
        double a=mDroiteFromLeft.at<double>(0,i);
        double b=mDroiteFromLeft.at<double>(1,i);
        double c=mDroiteFromLeft.at<double>(2,i);
        for(int y=0;y<nbpointRight;++y){
            mDistances.at<double>(i,y)=std::abs(a*m2DRightCorners.at<double>(0,y)+b*m2DRightCorners.at<double>(1,y)+c)/hypot(a,b);
        }
    }
    
    for(int i=0;i<nbpointRight;++i){
        double a=mDroiteFromRight.at<double>(0,i);
        double b=mDroiteFromRight.at<double>(1,i);
        double c=mDroiteFromRight.at<double>(2,i);
        for(int y=0;y<nbpointLeft;++y){
            mDistances2.at<double>(i,y)+=std::abs(a*m2DLeftCorners.at<double>(0,y)+b*m2DLeftCorners.at<double>(1,y)+c)/hypot(a,b);
        }
    }
    mDistances=mDistances+mDistances2.t();
    // Retour de la matrice fondamentale
    return mDistances;
}
//----------------------------------------
///\brief test si l'element est dans la matrice
///
///@param matrice : liste des correspondances des points
///@param elem : element a tester
///@return 1 si l'element est déja dans la liste
///@return 0 sinon
//-------------------------------------------------------
int isInMatrix(Mat matrice,int elem){
    int result = 0;
    for(int i =0;i<matrice.rows;i++){
        if (matrice.at<int>(i)==elem)
            result = 1;
    }
    return result;
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

    mRightHomologous = Mat_<int>(mDistances.cols,1);
    mLeftHomologous =  Mat_<int>(mDistances.rows,1);

    for (int i=0; i<mRightHomologous.rows;i++){
        mRightHomologous.at<int>(i) = -1;
    }

    for (int i=0; i<mLeftHomologous.rows;i++){
        mLeftHomologous.at<int>(i) = -1;
    }
    double minD = 1000000.;
    int minJ = -1;

    for (int i = 0;i<mDistances.rows;i++){
        for(int j =0;j<mDistances.cols;j++){
            if(minD>mDistances.at<double>(i,j)){
                minD = mDistances.at<double>(i,j);
                minJ = j;
            }
        }
        if (isInMatrix(mRightHomologous,minJ)==1){
             mRightHomologous.at<int>(i,0) = -1;
             mLeftHomologous.at<int>(minJ,0) = -1;
        }else{
//        if (minD >dMaxDistance){
//            mLeftHomologous.at<int>(i,0) = -1;
//            mRightHomologous.at<int>(minJ,0) = -1;
//        }else{
            mRightHomologous.at<int>(i,0) = minJ;
            mLeftHomologous.at<int>(minJ,0) = i;
//        }
        }
        minD = 1000000;
        minJ = -1;

    }

    std::cout <<"M = "<<std::endl << " " << mRightHomologous << std::endl;
    std::cout <<"M = "<<std::endl << " " << mLeftHomologous << std::endl;
}