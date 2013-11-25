#include "point.h"

Point::Point() : Vector2() {}

Point::Point(double x, double y, bool isFixedPoint) : Vector2(x, y) {
    _isFixedPoint = isFixedPoint;
}
