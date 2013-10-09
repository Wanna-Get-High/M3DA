#include "nurbs.h"

Nurbs::Nurbs()
{

}

void Nurbs::initialize(int m, int p) {

    _degree = p;

    for (int var = 1; var < m+p+1; ++var) {
        _knot.push_back(var);
    }
}

///
/// k = indice point de controle
/// p degree
void Nurbs::evaluate(double k, int p, double t) {

    if (p == 0) {
        if (t > _knot.at(k) && t < _knot.at(k+1) ) {

        } else {

        }
    }
}
