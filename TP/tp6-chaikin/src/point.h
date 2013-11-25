#ifndef POINTS_H
#define POINTS_H

#include "vector2.h"

class Point : public Vector2
{
private:
    bool _isFixedPoint;

public:
    Point();

    Point(double x, double y, bool isFixedPoint);

    inline bool isFixedPoint() { return _isFixedPoint; }
};

#endif // POINTS_H
