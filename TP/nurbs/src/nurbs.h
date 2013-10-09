#ifndef NURBS_H
#define NURBS_H

#include "UtilGL.h"

class Nurbs
{

protected:
    int _degree;

public:
    Nurbs();

    /// vecteur nodal
    std::vector<double> _knot;

    /// \brief initialise un vecteur nodal periodique uniforme pour
    ///         pour m points de contrôles et un degré p
    ///
    /// \param m le nombre de point de controle
    /// \param p le degre
    void initialize(int m, int p);

    /// \brief initialise un vecteur nodal periodique uniforme pour
    ///         pour m points de contrôles et un degré p
    ///
    /// \param m le nombre de point de controle
    /// \param p le degre
    void evaluate(double k, );
};

#endif // NURBS_H
