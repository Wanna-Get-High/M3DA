#ifndef GRID_H
#define GRID_H

#include "UtilGL.h"
#include <iostream>

class Grid
{

public:
    Grid();
    Grid(int x, int y);

    double **_grid;
    int _width;
    int _height;

    void drawGrid();
    double normalize(double value, double size);

};





#endif // GRID_H
