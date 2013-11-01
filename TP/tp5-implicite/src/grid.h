#ifndef GRID_H
#define GRID_H

#include "UtilGL.h"
#include "Implicit.h"
#include "Vector2.h"
#include <cmath>
#include <iostream>

class Grid
{
private:
    /**
     * @brief _implicit the class that evaluate the values
     */
    Implicit _implicit;

    /**
     * @brief _grid the grid of points.
     */
    double **_grid;

    /**
     * @brief _cuttedGrid the grid that contains 0
     *  if the value of _grid is < 0 and 1 otherwise.
     */
    int **_cuttedGrid;

    int _ambigusChoice;

    /**
     * @brief _curves the vector that contains the curves to draw.
     */
    std::vector< std::vector<Vector2> > _curves;


    /**
     * @brief _segments the vector that contains all of the founded segment to draw.
     */
    std::vector< std::vector<Vector2> > _segments;


    /**
     * @brief _width the width of the grid.
     */
    int _width;

    /**
     * @brief _height the height of the grid.
     */
    int _height;

    /**
     * @brief _begin the begining index of the draw panel
     */
    double _begin;
    /**
     * @brief _end the end index of the draw panel
     */
    double _end;

    /**
     * @brief _widthStep the width step between each point of the draw panel
     */
    double _widthStep;

    /**
     * @brief _heightStep the height step between each point of the draw panel
     */
    double _heightStep;

    /**
     * @brief computeCell compute the marching square for the current cell with the given coordinates.
     *
     * @param x0 the first cell x index
     * @param y0 the first cell y index
     * @param x1 the second cell x index
     * @param y1 the second cell y index
     * @param x2 the third cell x index
     * @param y2 the third cell y index
     * @param x3 the fourth cell x index
     * @param y3 the fourth cell y index
     */
    void computeCell(int x0, int y0, int x1, int y1,
                     int x2, int y2,  int x3, int y3);

    /**
     * @brief computeContourValue compute the value that will be used
     *          to know in with cas we are of the marching square algorithm.
     *
     * @param val0 the value of the first point of the square (top left).
     * @param val1 the value of the second point of the square (top right).
     * @param val2 the value of the third point of the square (bottom right).
     * @param val3 the value of the fourth point of the square (bottom left).
     *
     * @return a short number (between 0 and 15)
     */
    short computeContourValue(int val0, int val1, int val2, int val3);

    /**
     * @brief computePositionFactor compute the value that represent the position
     *          where the value is equal to 0.
     *          Will be used for the linear interpolation.
     *
     * @param val1 the beginning of the bracket.
     * @param val2 the end of the bracket.
     *
     * @return a value (between 0 and 1)
     */
    double computePositionFactor(double val1, double val2);

    /**
     * @brief computeSingleSegment compute the position of the points of the segment and add it to the _segments vector.
     *
     *  TODO : don't get the points 2 times.
     *
     * @param x0 the x position of the first point of the first segment
     * @param y0 the y position of the first point of the first segment
     * @param x1 the x position of the second point of the first segment
     * @param y1 the y position of the second point of the first segment
     * @param x2 the x position of the first point of the second segment
     * @param y2 the y position of the first point of the second segment
     * @param x3 the x position of the second point of the second segment
     * @param y3 the y position of the second point of the second segment
     */
    void computeSingleSegment(int x0, int y0, int x1, int y1,
                              int x2, int y2,  int x3, int y3);


    /**
     * @brief interpolatePosition interpolate the position between the two points passed in parameters.
     *          This interpolated point is then added to _segments.
     *          The position searched is the one where the value is equal to 0.
     *
     * @param x0 the x position of the first point
     * @param y0 the y position of the first point
     * @param x1 the x position of the second point
     * @param y1 the y position of the second point
     * @param t the value of interpolation
     */
    ////////////////////////////////////////////////////////////////////////////////////////////
    void interpolatePosition(int x0, int y0, int x1, int y1, double t1, int x2, int y2, int x3, int y3, double t2);

    /**
     * @brief changeSystemReference change the value to be between -1 and 1.
     *
     * @param value the value to be changed.
     * @param size the size used to change its position.
     *
     * @return the new value.
     */
    double changeSystemReference(double value, double size);


    /**
     * @brief addPointToSegments add a point to the _segments vector.
     *
     * @param xPos the x position of the point to be added.
     * @param yPos the y position of the point to be added.
     */
    ////////////////////////////////////////////////////////////////////////////////////////////
    void addSegmentToSegments(double xPoint1, double yPoint1, double xPoint2, double yPoint2);

    /**
     * @brief sortCurvePoints sort the points so that you can draw lines between each points easily.
     */
    void computeCurvePoints();

public:
    /**
     * @brief Grid create a grid of dimension [x][y]
     * @param x the width of the grid
     * @param y the height of the grid
     */
    Grid(int x, int y);

    /**
     * @brief drawGrid draw the grid.
     */
    void drawGridAsPoints();


    void drawGridAsLines();

    /**
     * @brief drawCurve draw the curve.
     */
    void drawCurvesWithPoints();


    void drawCurvesWithLines();

    /**
     * @brief eval evaluate the function eval of Implicit class for each point of the grid
     */
    void eval();

    /**
     * @brief computeCells compute the marching square for every cell of the grid.
     */
    void computeCells();

    void setChoice(int choice);

    int getChoice();

    void setAmbigusChoice(int choice);

    void addBlob(double a, double b, Vector2 center, double r);
};

#endif // GRID_H
