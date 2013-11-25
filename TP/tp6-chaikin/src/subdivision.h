#ifndef CHAIKIN_H
#define CHAIKIN_H

#import "vector2.h"
#import "UtilGL.h"

class Subdivision
{
private:
    std::vector<Vector2> _curve;
    std::vector<Vector2> _subdivisedCurve;

    int _choice;
    int _nbIteration;
    bool _isClosed;
    bool _particular;
    double _a;

    void computeNewPointsUsingChaikin();
    void computeNewPoints();

    std::vector<Vector2> computeSubCurveUsingChaikin(std::vector<Vector2> subCurve);
    std::vector<Vector2> computeSubCurveUsingDynAndLevin(std::vector<Vector2> subCurve);

public:
    Subdivision();

    void addPointToCurve(double x, double y, bool isFixedPoint);

    void removeLastPoint();

    void drawCurve();

    void drawFixedPoint();

    void incrNbIteration();

    void decrNbIteration();

    void resetCurves();

    void useDynAndLevin();

    void useChaikin();

    void closeCurve();

    inline int choice() { return _choice; }
    inline int nbIteration() { return _nbIteration; }
    inline bool isClosed() { return _isClosed; }
    inline double a() { return _a; }
    inline bool isParticular() { return _particular; }

    void useParticularChaikin();
    void incrA();
    void decrA();

};

#endif // CHAIKIN_H
