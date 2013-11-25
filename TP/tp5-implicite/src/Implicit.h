#ifndef IMPLICIT_H
#define IMPLICIT_H

#include "UtilGL.h"
#include "Vector2.h"
#include <cmath>

class Implicit
{
    std::vector <Vector2> _centers;

    std::vector <double> _as;
    std::vector <double> _bs;
    std::vector <double> _rs;

    int _choice;

    double evalCircle(double x, double y);

    double evalSoftObjects(double x, double y);

    double evalSoftObject(int i, double  x, double y);

    double evalBlobs(double x, double y);

    double evalBlob(int i, double x, double y);


public:
    Implicit();

    Implicit(int choice);

    double eval(double x, double y);

    void addBlob(double a, double b, Vector2 center, double r);

    void setChoice(int choice);

    int getChoice();

};

#endif // IMPLICIT_H
