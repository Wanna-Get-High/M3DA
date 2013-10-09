#include "nurbs.h"
#include <stream.h>

Nurbs::Nurbs()
{

}

void Nurbs::initialize(int m, int p) {

    _degree = p;

    for (int var = 1; var < m+p+1; ++var) {
        _knot.push_back(var);
    }
}


/// k = indice point de controle
/// p degree
/// en t
double Nurbs::evaluate(double k, int p, double t) {

    if (p == 0) {
        if (t >= _knot[k] && t < _knot[k+1] ) {
            return 1;
        } else {
            return 0;
        }
    }

    double denom1 = (_knot[k+p] - _knot[k]);
    double denom2 = (_knot[k+p+1] - _knot[k+1]);
    double part1 = 0;
    double part2 = 0;

    // on évite la division part 0;
    if (denom1 != 0)
        part1 = (t - _knot[k]) / denom1;

    if (denom2 != 0)
        part2 = (_knot[p+k+1] - t) / denom2;

    return part1*evaluate(k,p-1,t) + part2*evaluate(k+1,p-1,t);
}

void Nurbs::drawN(int k, int n) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();

    /// drawing examples

    glColor3f(1,0,0);
    prog3d::ugl::drawText("DrawN",0,0);

    ////////////////
    // Les lignes //
    ////////////////
    glLineWidth(3);
    glColor3f(0,1,0);
    glBegin(GL_LINE_STRIP);

    int size = _knot.size();

    int begin = _knot[0];
    int end  = _knot[size-1];

    for (double t = begin; t < end ; t+=0.1) {

        double eval = evaluate(k,n,t);

        // on reviens dans le bon repert ()
        t = t/size -1;

        // -1 pour aller a gauche
        glVertex2f(t,eval);
    }
    glEnd();

    ////////////////
    // Les points //
    ////////////////
    glPointSize(10);
    glColor3f(0,0,1);
    glBegin(GL_POINTS);

    for (int i = 0; i < size; ++i) {
        double t = _knot.at(i);

        double eval = evaluate(k,n,t);
        glVertex2f(t,eval);
    }

    glEnd();


    glPopMatrix();
}

