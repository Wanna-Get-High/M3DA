#ifndef VECTOR2_H
#define VECTOR2_H

class Vector2
{
private:
    double _x;
    double _y;

public:
    Vector2();
    Vector2(double x, double y);

     Vector2 operator +(const Vector2& right) const;
     Vector2 operator *(const Vector2& right) const;
     Vector2 operator +(const double right) const;
     Vector2 operator *(const double right) const;

     double dist(Vector2 vec);

     double dist(double x, double y);


     void x(double x);
     void y(double y);

     double x() const;
     double y() const;
};

#endif // VECTOR2_H
