#include "subdivision.h"

using namespace std;

Subdivision::Subdivision() {
    _nbIteration =  0;
    _choice = 0;
    _particular = false;
    _a = 0;
}

void Subdivision::addPointToCurve(double x, double y, bool isFixedPoint) {
    if (_choice == 0) {
        _curve.push_back(Vector2(x, y, isFixedPoint));
        Subdivision::computeNewPoints();
    } else if (_choice == 1) {
        if (!_isClosed) {
            _curve.push_back(Vector2(x, y, isFixedPoint));
        }
    }
}

void Subdivision::removeLastPoint() {
    if (_choice == 0) {
        _curve.pop_back();
        Subdivision::computeNewPoints();
    } else {
        // only remove points if the curve is closed
        // for the DynAndLevin case
        if (!_isClosed) _curve.pop_back();
    }
}

void Subdivision::drawFixedPoint() {

    glPointSize(10);
    glColor3f(0,1,0);
    glBegin(GL_POINTS);
    int curveSize = _curve.size();
    for (double i = 0; i < curveSize ; i++) {
        Vector2 point = _curve.at(i);
        if (point.isFixedPoint())
            glVertex2f(point.x(),point.y());
    }
    glEnd();
}

void Subdivision::drawCurve() {
    int curveSize = _curve.size();
    int subdivisedCurveSize = _subdivisedCurve.size();
    Vector2 point;

    glPushMatrix();

    glLineWidth(3);

    glColor3f(0,0,1);
    glBegin(GL_LINE_STRIP);
    for (double i = 0; i < subdivisedCurveSize ; i++) {
        point = _subdivisedCurve.at(i);
        glVertex2f(point.x(),point.y());
    }

    // add the first point to close the curve
    if (_choice == 1 && _subdivisedCurve.size() > 0 && _isClosed) {
        point = _subdivisedCurve.at(0);
        glVertex2f(point.x(),point.y());
    }

    glEnd();

    glColor3f(1,0,0);
    glBegin(GL_LINE_STRIP);

    for (double i = 0; i < curveSize ; i++) {
        point = _curve.at(i);
        glVertex2f(point.x(),point.y());
    }

    // add the first point to close the curve
    if (_choice == 1 && _curve.size() > 0 && _isClosed) {
        point = _curve.at(0);
        glVertex2f(point.x(),point.y());
    }
    glEnd();

    glPopMatrix();
}

void Subdivision::computeNewPoints() {
    std::vector<Vector2> subCurve = _curve;

    for (int i = 0; i < _nbIteration; ++i) {
        if (_choice == 0)   subCurve = Subdivision::computeSubCurveUsingChaikin(subCurve);
        else                subCurve = Subdivision::computeSubCurveUsingDynAndLevin(subCurve);
    }

    if (_nbIteration > 0)   _subdivisedCurve = subCurve;
    else                    _subdivisedCurve.clear();
}

std::vector<Vector2> Subdivision::computeSubCurveUsingChaikin(std::vector<Vector2> subCurve) {

    std::vector<Vector2> newSubCurve;

    Vector2 p1; // pi
    Vector2 p2; // pi+1
    Vector2 q1; // qi
    Vector2 q2; // qi+1

    int size = subCurve.size();

    for (int j = 0; j < size-1; ++j) {

        p1 = subCurve.at(j);
        p2 = subCurve.at(j+1);

        // if we choose the particular subdivision (last question)
        if (_particular) {
            if (!p1.isFixedPoint() && !p2.isFixedPoint()) {

                q1 = (p1*(3./4.)) + (p2*(1./4.));
                q1.setIsFixedPoint(false);

                q2 = (p1*(1./4.)) + (p2*(3./4.));
                q2.setIsFixedPoint(false);

                newSubCurve.push_back(q1);
                newSubCurve.push_back(q2);

            } else if (p1.isFixedPoint() && !p2.isFixedPoint()) {
                q1 = p1;
                q1.setIsFixedPoint(p1.isFixedPoint());

                q2 = p1*(1-_a) + p2*_a;
                q2.setIsFixedPoint(false);

                newSubCurve.push_back(q1);
                newSubCurve.push_back(q2);

            } else if (!p1.isFixedPoint() && p2.isFixedPoint()) {
                q1 = p1*_a + p2*(1-_a);
                q2.setIsFixedPoint(false);

                q2 = p2;
                q2.setIsFixedPoint(p2.isFixedPoint());

                newSubCurve.push_back(q1);
                newSubCurve.push_back(q2);
            }
        } else {
            q1 = (p1*(3./4.)) + (p2*(1./4.));
            q1.setIsFixedPoint(false);

            q2 = (p1*(1./4.)) + (p2*(3./4.));
            q2.setIsFixedPoint(false);

            if (p1.isFixedPoint()) newSubCurve.push_back(p1);

            newSubCurve.push_back(q1);
            newSubCurve.push_back(q2);
        }
    }

    if (!subCurve.empty() && subCurve.at(size-1).isFixedPoint()) newSubCurve.push_back(subCurve.at(size-1));

    return newSubCurve;
}

std::vector<Vector2> Subdivision::computeSubCurveUsingDynAndLevin(std::vector<Vector2> subCurve) {

    std::vector<Vector2> newSubCurve;

    Vector2 p1; // pi
    Vector2 p2; // pi+1
    Vector2 p3; // pi+2
    Vector2 p4; // pi-1
    Vector2 q2; // qi+1

    int size = subCurve.size();

    // Cas particulier : premier point
    p1 = subCurve.at(0);
    p2 = subCurve.at(1);
    p3 = subCurve.at(2);
    p4 = subCurve.at(size-1);

    q2 = ((p3+p4)*(-1./16.)) + ((p2+p1)*(9./16.));

    newSubCurve.push_back(p1);
    newSubCurve.push_back(q2);

    // le reste
    for (int j = 1; j < size-2; ++j) {

        p1 = subCurve.at(j);
        p2 = subCurve.at(j+1);
        p3 = subCurve.at(j+2);
        p4 = subCurve.at(j-1);

        q2 = ((p3+p4)*(-1./16.)) + ((p2+p1)*(9./16.));

        newSubCurve.push_back(p1);
        newSubCurve.push_back(q2);
    }

    // cas particulier : avant dernier point
    p1 = subCurve.at(size - 2);
    p2 = subCurve.at(size - 1);
    p3 = subCurve.at(0);
    p4 = subCurve.at(size - 3);

    q2 = ((p3+p4)*(-1./16.)) + ((p2+p1)*(9./16.));

    newSubCurve.push_back(p1);
    newSubCurve.push_back(q2);

    // cas particulier : dernier point
    p1 = subCurve.at(size - 1);
    p2 = subCurve.at(0);
    p3 = subCurve.at(1);
    p4 = subCurve.at(size - 2);

    q2 = ((p3+p4)*(-1./16.)) + ((p2+p1)*(9./16.));

    newSubCurve.push_back(p1);
    newSubCurve.push_back(q2);

    // on met a jour la courbe precedente
    return newSubCurve;
}


void Subdivision::incrNbIteration() {
    _nbIteration++;

    if (_choice == 0 || (_choice == 1 && _isClosed)) {
        Subdivision::computeNewPoints();
    }
}

void Subdivision::decrNbIteration() {
    if (_nbIteration > 0) _nbIteration--;

    if (_choice == 0 || (_choice == 1 && _isClosed)) {
        Subdivision::computeNewPoints();
    }
}

void Subdivision::resetCurves() {
    _curve.clear();
    _subdivisedCurve.clear();
    _nbIteration = 0;
}

void Subdivision::closeCurve() {
    // close the curve and compute the subdivised curve
    if (!_isClosed && _choice == 1) {
        _isClosed = true;
        Subdivision::computeNewPoints();
    }
    // open the curve and clear the subdivised curve
    else if (_isClosed && _choice == 1) {
        _isClosed = false;
        _subdivisedCurve.clear();
    }
}

void Subdivision::useChaikin() {
    _choice = 0;
    _particular = false;
    _isClosed = false;
}

void Subdivision::useParticularChaikin() {
    if (_choice == 0) {
        _particular = !_particular;
        Subdivision::computeNewPoints();
    }
}

void Subdivision::useDynAndLevin() {
    _choice = 1;
    _isClosed = false;
    _particular = false;
}

void Subdivision::incrA() {
    if (_a <= 0.9) _a+=0.1;
    if (_choice == 0 && _particular) Subdivision::computeNewPoints();
}

void Subdivision::decrA() {
    if (_a > 0) _a-=0.1;
    if (_choice == 0 && _particular) Subdivision::computeNewPoints();
}

