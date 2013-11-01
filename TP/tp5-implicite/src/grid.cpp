#include "grid.h"

Grid::Grid(int x, int y)
{
    _width = x;
    _height = y;

    _ambigusChoice = 0;

    // the values of the grid that has to be between -1 and 1
    _begin = -1;
    _end = 1;
    _widthStep = (_end - _begin) / (x-1); // -1 to have the right number of points
    _heightStep = (_end - _begin) / (y-1);

    // création dynamique à deux dimension
    _grid = new double* [x];

    for (int i = 0; i < x; ++i) {
        _grid[i] = new double[y];
    }

    _cuttedGrid = new int* [x];

    for (int i = 0; i < x; ++i) {
        _cuttedGrid[i] = new int[y];
    }

}

void Grid::eval() {
    int k = 0;
    int l = 0;

    for (double i = _begin; i <= _end; i += _widthStep) {
        l = 0;
        for (double j = _begin; j <= _end; j += _heightStep) {

            _grid[k][l] = _implicit.eval(i, j);

            if (_grid[k][l] < 0)    _cuttedGrid[k][l] = 0;
            else                    _cuttedGrid[k][l] = 1;

            l++;
        }

        k++;
    }

    Grid::computeCells();
}



void Grid::drawGridAsPoints() {

    glPushMatrix();
    glPointSize(10);
    glBegin(GL_POINTS);

    int k = 0;
    int l = 0;

    for (double i = _begin; i <= _end; i += _widthStep) {
        l = 0;
        for (double j = _begin; j <= _end; j += _heightStep) {

            // change the color of the point depending on the
            // sign of the evaluated value.
            if (_grid[k][l]  >= 0) {
                glColor3f(1,0,0);
            } else {
                glColor3f(0,0,1);
            }
            glVertex2d(i, j);
            l++;
        }

        k++;
    }

    glEnd();
    glPopMatrix();
}

void Grid::drawGridAsLines() {

    glPushMatrix();
    glLineWidth(3);
    glColor3f(0,0,0);

    for (double i = _begin; i <= _end; i += _widthStep) {
        glBegin(GL_LINE_STRIP);
        glVertex2d(i, -1);
        glVertex2d(i, 1);
        glEnd();
    }

    for (double i = _begin; i <= _end; i += _heightStep) {
        glBegin(GL_LINE_STRIP);
        glVertex2d(-1, i);
        glVertex2d(1, i);
        glEnd();
    }

    glPopMatrix();
}



void Grid::drawCurvesWithPoints() {

    glPushMatrix();
    glPointSize(10);
    glBegin(GL_POINTS);

    glColor3f(0,1,0);

    int nbCurve = _curves.size();

    for (int i = 0; i < nbCurve; ++i) {

        std::vector<Vector2> curve = _curves.at(i);
        int size = curve.size();

        for (int j = 0; j < size; ++j) {

            Vector2 point  = curve.at(j);
            double pointX = changeSystemReference(point.x(),_width);
            double pointY = changeSystemReference(point.y(),_height);

            glVertex2d(pointX, pointY);
        }
    }

    glEnd();
    glPopMatrix();
}


void Grid::drawCurvesWithLines() {

    glPushMatrix();
    glLineWidth(3);
    glColor3f(1,0,0);

    int nbCurve = _curves.size();

    for (int i = 0; i < nbCurve; ++i) {
        glBegin(GL_LINE_STRIP);

        std::vector<Vector2> curve = _curves.at(i);
        int size = curve.size();

        for (int j = 0; j < size; ++j) {

            Vector2 point  = curve.at(j);
            double pointX = changeSystemReference(point.x(),_width);
            double pointY = changeSystemReference(point.y(),_height);

            glVertex2f(pointX, pointY);
        }

        Vector2 point  = curve.at(0);
        double pointX = changeSystemReference(point.x(),_width);
        double pointY = changeSystemReference(point.y(),_height);

        glVertex2f(pointX, pointY);

        glEnd();
    }

    glPopMatrix();
}


void Grid::computeCells() {

    _curves.clear();
    _segments.clear();

    for (int i = 0; i < _width-1; ++i) {
        for (int j = 0; j < _height-1; ++j) {
            Grid::computeCell(i, j, i+1, j, i+1, j+1, i, j+1);
        }
    }

    Grid::computeCurvePoints();
}


void Grid::computeCurvePoints() {

    // we initialize the 1st curve of the several potential curves
    std::vector<Vector2> curve;
    std::vector<Vector2> newSegment = _segments.at(0);

    // we add the points of the first segment
    curve.push_back(newSegment.at(0));
    curve.push_back(newSegment.at(1));

    // we erase the first segment
    _segments.erase(_segments.begin());

    int index;
    bool stillSegmentToAttach = true;
    int size;

    // while there is still segment in the segment vector
    while (_segments.size() > 0) {

        // if we founded a segment that has a point in common
        if (stillSegmentToAttach) {

            // we get the last added point to find the next segment
            // that has a point in common
            Vector2 lastVectorAdded = curve.at(curve.size()-1);

            index = 0;
            stillSegmentToAttach = false;
            size = _segments.size();

            // while we didn't find a segment and didn't reach the end of the segment vector
            while (!stillSegmentToAttach && index < size) {

                // retrieve the next segment
                std::vector<Vector2> currentSegment = _segments.at(index);

                // retrieve the points
                Vector2 point1 = currentSegment.at(0);
                Vector2 point2 = currentSegment.at(1);

                // if there is a point in common add the other point of the segment
                // and erase this segment from the list
                if (point1.x() == lastVectorAdded.x() && point1.y() == lastVectorAdded.y()) {

                    curve.push_back(point2);

                    _segments.erase(_segments.begin()+index);

                    stillSegmentToAttach = true;

                } else if (point2.x() == lastVectorAdded.x() && point2.y() == lastVectorAdded.y()) {

                    curve.push_back(point1);

                    _segments.erase(_segments.begin()+index);

                    stillSegmentToAttach = true;
                }

                index++;
            }

        }
         // if we didn't founded a segment that has a point in common
        else {

            // add the first founded curve
            _curves.push_back(curve);

            // clear it and add the first segment of the remaining ones
            curve.clear();
            newSegment = _segments.at(0);

            curve.push_back(newSegment.at(0));
            curve.push_back(newSegment.at(1));

            // erase the first segment added to the curve.
            _segments.erase(_segments.begin());

            stillSegmentToAttach = true;
        }
    }

    // add the last curve if there is one
    _curves.push_back(curve);
}


void Grid::computeCell(int x0, int y0, int x1, int y1,
                       int x2, int y2,  int x3, int y3) {

    short value = Grid::computeContourValue(_cuttedGrid[x0][y0],
                                            _cuttedGrid[x1][y1],
                                            _cuttedGrid[x2][y2],
                                            _cuttedGrid[x3][y3]);

    // single segment
    if (value == 14 || value == 1) { // botom left changing value
        Grid::computeSingleSegment(x3, y3, x0, y0, x3, y3, x2, y2);
    } else if (value == 13 || value == 2) { // bottom right changing value
        Grid::computeSingleSegment(x2, y2, x1, y1, x2, y2, x3, y3);
    } else if (value == 11 || value == 4) { // top right changing value
        Grid::computeSingleSegment(x1, y1, x0, y0, x1, y1, x2, y2);
    } else if (value == 7 || value == 8) { // top left changing value
        Grid::computeSingleSegment(x0, y0, x1, y1, x0, y0, x3, y3);
    }
    else if (value == 12 || value == 3) { // both top or bottom changing value
        Grid::computeSingleSegment(x0, y0, x3, y3, x1, y1, x2, y2);
    } else if (value == 9 || value == 6) { // both left or right changing value
        Grid::computeSingleSegment(x0, y0, x1, y1, x3, y3, x2, y2);
    }
    // two segment
    else if (value == 10 || value == 5) {
        if (_ambigusChoice == 0) {
            Grid::computeSingleSegment(x0, y0, x1, y1, x1, y1, x2, y2);
            Grid::computeSingleSegment(x0, y0, x3, y3, x3, y3, x2, y2);
        } else {
            Grid::computeSingleSegment(x3, y3, x0, y0, x0, y0, x1, y1);
            Grid::computeSingleSegment(x1, y1, x2, y2, x2, y2, x3, y3);
        }
    }
}

short Grid::computeContourValue(int val0, int val1, int val2, int val3) {

    short value = val0;
    value = (value << 1) | val1;
    value = (value << 1) | val2;
    value = (value << 1) | val3;

    return value;
}



void Grid::computeSingleSegment(int x0, int y0, int x1, int y1,
                                int x2, int y2,  int x3, int y3) {

    double t1 = Grid::computePositionFactor(_grid[x0][y0], _grid[x1][y1]);
    double t2 = Grid::computePositionFactor(_grid[x2][y2], _grid[x3][y3]);

    Grid::interpolatePosition(x0, y0, x1, y1, t1, x2, y2, x3, y3, t2);
}


void Grid::interpolatePosition(int x0, int y0, int x1, int y1, double t1,
                               int x2, int y2, int x3, int y3, double t2) {

    double xPoint1 = x0;
    double yPoint1 = y0;

    double xPoint2 = x2;
    double yPoint2 = y2;


    // interpolation has to be done on the x axis
    if (x0 != x1) {
        xPoint1 = x0+(x1-x0)*t1;
    }
    // else it has to be done on the y axis
    else {
        yPoint1 = y0+(y1-y0)*t1;
    }

    // interpolation has to be done on the x axis
    if (x2 != x3) {
        xPoint2 = x2+(x3-x2)*t2;
    }
    // else it has to be done on the y axis
    else {
        yPoint2 = y2+(y3-y2)*t2;
    }

    Grid::addSegmentToSegments(xPoint1, yPoint1, xPoint2, yPoint2);
}


void Grid::addSegmentToSegments(double xPoint1, double yPoint1, double xPoint2, double yPoint2) {

    std::vector<Vector2> newSegment;
    newSegment.push_back(Vector2(xPoint1,yPoint1));
    newSegment.push_back(Vector2(xPoint2,yPoint2));
    _segments.push_back(newSegment);

}

double Grid::computePositionFactor(double val1, double val2) {
    return std::abs(val1) / (std::abs(val1) + std::abs(val2));
}


double Grid::changeSystemReference(double value, double size) {
    return value / (size-1) * 2 -1;
}


void Grid::setChoice(int choice) {
    _implicit.setChoice(choice);
}

int Grid::getChoice() {
    return _implicit.getChoice();
}


void Grid::addBlob(double a, double b, Vector2 center, double r) {
    _implicit.addBlob(a, b, center, r);
}

void Grid::setAmbigusChoice(int choice) {
    _ambigusChoice = choice;
}
