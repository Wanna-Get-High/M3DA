#include "grid.h"

Grid::Grid(int x, int y)
{
    _width = x;
    _height = y;

    // crétion dinamique à deux dimension
    _grid = new double* [x];

    for (int i = 0; i < x; ++i) {
        _grid[i] = new double[y];
    }
}


void Grid::drawGrid() {

    glPushMatrix();

    glPointSize(5);
    glColor3f(1,0,0);
    glBegin(GL_POINTS);

    for (int i = 0; i < _width; ++i) {
        for (int j = 0; j < _height; ++j) {

            double pointX = normalize(i,_width);
            double pointY = normalize(j,_height);

            glVertex2f(pointX,pointY);
        }
    }

    glEnd();
    glPopMatrix();
}

double Grid::normalize(double value, double size) {
    return value / (size-1) * 2 -1;
}

