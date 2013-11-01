#include "Implicit.h"

Implicit::Implicit() {
    _choice = 0;
}

Implicit::Implicit(int choice) {
    _choice = choice;
}

double Implicit::eval(double x, double y) {
    if (_choice == 0) {
        return Implicit::evalCircle(x, y);
    } else if (_choice == 1) {
        return Implicit::evalBlobs(x, y);
    } else {
        std::cerr << "not a valid choice" << std::endl;
        return NULL;
    }
}

double Implicit::evalCircle(double x, double y) {
    return (x*x) + (y*y) - 1;
}

double Implicit::evalBlob(int i, double x, double y) {
    Vector2 center = _centers.at(i);
    double distance = center.dist(x, y);

    return _as.at(i)*std::exp(-(_bs.at(i)*(distance/_rs.at(i))));
}

double Implicit::evalBlobs(double x, double y) {

    int nbBlob = _as.size();
    double eval = 0;

    for (int i = 0; i < nbBlob; ++i) {
        eval += Implicit::evalBlob(i, x, y);
    }

    return 1 - eval;
}


void Implicit::addBlob(double a, double b, Vector2 center, double r) {
    _as.push_back(a);
    _bs.push_back(b);
    _rs.push_back(r);
    _centers.push_back(center);
}

void Implicit::setChoice(int choice) {
    _choice = choice;
}

int Implicit::getChoice() {
    return _choice;
}
