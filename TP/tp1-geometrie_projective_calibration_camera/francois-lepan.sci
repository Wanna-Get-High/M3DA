// -----------------------------------------------------------------------
/// \brief Calcule un terme de contrainte a partir d'une homographie.
///
/// \param H: matrice 3*3 définissant l'homographie.
/// \param i: premiere colonne.
/// \param j: deuxieme colonne.
/// \return vecteur definissant le terme de contrainte.
// -----------------------------------------------------------------------
function v = ZhangConstraintTerm(H, i, j)
    v = zeros(6);
    
    v(1) = H(1,i) * H(1,j);
    v(2) = H(1,i) * H(2,j) + H(2,i) * H(1,j);
    v(3) = H(2,i) * H(2,j);
    v(4) = H(3,i) * H(1,j) + H(1,i) * H(3,j);
    v(5) = H(3,i) * H(2,j) + H(2,i) * H(3,j);
    v(6) = H(3,i) * H(3,j);
    
    v = v';
endfunction

// -----------------------------------------------------------------------
/// \brief Calcule deux equations de contrainte a partir d'une homographie
///
/// \param H: matrice 3*3 définissant l'homographie.
/// \return matrice 2*6 definissant les deux contraintes.
// -----------------------------------------------------------------------
function v = ZhangConstraints(H)
  v = [ZhangConstraintTerm(H, 1, 2); ...
    ZhangConstraintTerm(H, 1, 1) - ZhangConstraintTerm(H, 2, 2)];
endfunction

// -----------------------------------------------------------------------
/// \brief Calcule la matrice des parametres intrinseques.
///
/// \param b: vecteur resultant de l'optimisation de Zhang.
/// \return matrice 3*3 des parametres intrinseques.
// -----------------------------------------------------------------------

function A = IntrinsicMatrix(b)
    // sachant que A = [alpha, gama, U0]
    //                 [  0  , Beta, V0]
    //                 [  0  ,  0  ,  1]
    //  et que b = [B11, B12, B22, B13, B23, B33]
    // V0 = (B12*B13 - B11*B23) / (B11*B22 - B12^2)

    v0 = (b(2)*b(4) - b(1)*b(5)) / (b(1)*b(3) - b(2)*b(2));

    lambda = b(6) - ( b(4)*b(4) + v0*( b(2)*b(4) - b(1)*b(5) ) )/b(1);

    alpha = sqrt( lambda / b(1) );

    beta_ = sqrt( (lambda * b(1)) / ( b(1)*b(3) - b(2)*b(2)) );

    gama = -(b(2) * (alpha*alpha) * beta_) / lambda;

    u0 = (gama * v0 / beta_) - ( b(4) * (alpha*alpha)/lambda );
  
    A = [alpha, gama,  u0;
         0    , beta_, v0;
         0    , 0    , 1];
endfunction

function [v]=prod_vect(v1,v2)
    v(1) = v1(2)*v2(3) - v1(3)*v2(2)
    v(2) = v1(3)*v2(1) - v1(1)*v2(3)
    v(3) = v1(1)*v2(2) - v1(2)*v2(1)
endfunction

// -----------------------------------------------------------------------
/// \brief Calcule la matrice des parametres extrinseques.
///
/// \param iA: inverse de la matrice intrinseque.
/// \param H: matrice 3*3 definissant l'homographie.
/// \return matrice 3*4 des parametres extrinseques.
// -----------------------------------------------------------------------
function E = ExtrinsicMatrix(iA, H)
    
  lambda = (1 / (norm(iA*H(:,1))));
  
  r1 = lambda*iA*H(:,1);
  
  r2 = lambda*iA*H(:,2);
  
  r3 = prod_vect(r1,r2);
  
  t = lambda*iA*H(:,3);
    
    
  E = [r1', t(1);
       r2', t(2);
       r3', t(3)];
      
endfunction

