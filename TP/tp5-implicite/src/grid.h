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

    /**
     * @brief _ambigusChoice the choice made for the ambigus case of themarching square algorithm
     */
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
     * @brief interpolatePosition interpolate the position of the points of the new segment
     *          that will be added to _segments
     *
     * @param x0 the x position of the first point of the first segment
     * @param y0 the y position of the first point of the first segment
     * @param x1 the x position of the second point of the first segment
     * @param y1 the y position of the second point of the first segment
     * @param t1 the value of interpolation to find the coordinates of the first point of the new segment
     * @param x2 the x position of the first point of the second segment
     * @param y2 the y position of the first point of the second segment
     * @param x3 the x position of the second point of the second segment
     * @param y3 the y position of the second point of the second segment
     * @param t2 the value of interpolation to find the coordinates of the second point of the new segment
     */
    void interpolatePosition(int x0, int y0, int x1, int y1, double t1,
                             int x2, int y2, int x3, int y3, double t2);

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
     * @brief addSegmentToSegments add a new segment to the _segments vector
     * @param xPoint1 the x coordinate of the first point
     * @param yPoint1 the y coordinate of the first point
     * @param xPoint2 the x coordinate of the second point
     * @param yPoint2 the y coordinate of the second point
     */
    void addSegmentToSegments(double xPoint1, double yPoint1, double xPoint2, double yPoint2);

    /**
     * @brief computeCurvePoints compute the points that will be added to _curves.
     *          It creates a current curve.
     *          It add the first 2 points of the first element of _segments to the current curve.
     *
     *          Then it search for a segment in _segments that has a common point
     *               if he found one
     *                      it add to the current curve
     *                      and remove the founded segment from _segments
     *               else
     *                      it add the current curve to _curves
     *                      and create a new current curve
     *                      then add the first 2 points of the first element of _segments to the current curve.
     *
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


    /**
     * @brief drawGridAsLines draw the grid as lines.
     */
    void drawGridAsLines();

    /**
     * @brief drawCurve draw the curve.
     */
    void drawCurvesWithPoints();

    /**
     * @brief drawCurvesWithLines draw the curve as lines.
     */
    void drawCurvesWithLines();

    /**
     * @brief eval evaluate the chosen function of Implicit class for each point of the grid.
     */
    void eval();

    /**
     * @brief computeCells compute the marching square for every cell of the grid.
     */
    void computeCells();

    /**
     * @brief setChoice set the choice of the function that will be used in Implicit
     * @param choice the choice
     */
    void setChoice(int choice);

    /**
     * @brief getChoice get the current choice
     * @return an integer representing the choice
     */
    int getChoice();

    /**
     * @brief setAmbigusChoice set the ambigus choice
     * @param choice the choice
     */
    void setAmbigusChoice(int choice);

    /**
     * @brief addBlob add a blob to the Implicit class
     */
    void addBlob(double a, double b, Vector2 center, double r);

};

#endif // GRID_H
