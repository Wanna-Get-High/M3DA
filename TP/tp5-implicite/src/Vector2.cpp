#include "vector2.h"
#include "glsupport.h"
#include <cmath>
#include "UtilGL.h"


Vector2::Vector2() {
    _x = 0;
    _y = 0;
}

Vector2::Vector2(double x, double y) {
    _x = x;
    _y = y;
 }

Vector2 Vector2::operator +(const Vector2& right) const {
    Vector2 result;
    result.x(x() + right.x());
    result.y(y() + right.y());

    return result;
}

Vector2 Vector2::operator +(const double right) const {
    Vector2 result;
    result.x(x() + right);
    result.y(y() + right);

    return result;
}


Vector2 Vector2::operator *(const Vector2& right) const {
    Vector2 result;
    result.x(x() * right.x());
    result.y(y() * right.y());

    return result;
}

Vector2 Vector2::operator *(const double right) const {
    Vector2 result;
    result.x(x() * right);
    result.y(y() * right);

    return result;
}


double Vector2::dist(Vector2 vec) {
    return Vector2::dist(vec.x(), vec.y());
}

double Vector2::dist(double x, double y) {
    return std::sqrt(std::pow(x - _x, 2) + std::pow(y - _y, 2));
}

double Vector2::x() const { return _x; }
double Vector2::y() const { return _y; }

void Vector2::x(double x) { _x = x; }
void Vector2::y(double y) { _y = y; }
