/**

  @author F. Aubert
  **/


#include "GLView.h"
#include <iostream>
#include "nurbs.h"

using namespace std;
using namespace prog3d;


/// ctor
GLView::GLView(QWidget *parent) : QGLWidget(parent) {

    setFocusPolicy(Qt::StrongFocus); // this widget can now catch the keyboard events
}


/// dtor
GLView::~GLView() {
}

/**
 initialize GL context (with glew ou without glew) + common set up
 **/
void GLView::initializeGL() {

#ifdef WITH_GLEW
    // initialization of all available OpenGL functionalities
    if (glewInit()!=GLEW_OK) {
        throw ErrorD("cant initialize glew");
    }
#endif

    cout << "GL version :" << glGetString(GL_VERSION) << endl;
    cout << "Shading Language  : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

    // common OpenGL set up
    glEnable(GL_DEPTH_TEST);  // hidden parts removal with depth buffer algorithm
    glClearColor(1,1,1,1); // color value when glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) is done
    glClearDepth(1.0);       // depth value when glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) is done

    glDisable(GL_STENCIL_TEST);
    glClearStencil(0);

    // polygon drawings set up
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

    glPixelStorei(GL_PACK_ALIGNMENT,4);
    glPixelStorei(GL_UNPACK_ALIGNMENT,4);

    ugl::initUtilGL(this);

    initData();

}


/** ***************************************************************************
resize
  **/


/// when the graphics window is sized/resized (including the first time the window is shown)
void GLView::resizeGL(int width, int height) {
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1,1,-1,1,-1,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}



/** **********************************************************************
  Events
  **/
void GLView::mousePressEvent(QMouseEvent *event) {

    // clique gauche
    if (event->button()==Qt::LeftButton) {

        // on met les coordonnees a la bonne echelle.
        double x = event->x() / (double)(width()/2) -1;
        double y = -(event->y() / (double)(height()/2) -1);

        // on ajoute un point de controle
        _nurbs.addControlePoint(x ,y );

        //cout << "left mouse : " << x << "," << x << endl;
    }

    // clique droit
    if (event->button()==Qt::RightButton) {
        // on retire le dernier point de controle
        _nurbs.removeControlePoint();
    }

}

void GLView::keyPressEvent(QKeyEvent *event) {
    if (event->isAutoRepeat())
        QGLWidget::keyPressEvent(event);
    else
        switch(event->key()){

        // en appuyant sur Q
        case Qt::Key_Q:
            // on incremente le degre de la courbe
            _nurbs.incrDegree();
            break;

        // en appuyant sur W
        case Qt::Key_W:
            // on decremente le degre de la courbe
            _nurbs.decrDegree();
            break;
        default:
            QGLWidget::keyPressEvent(event); // dispatch the event to the parent
        }
}

void GLView::mouseReleaseEvent(QMouseEvent *event) {
    switch(event->button()){
    case Qt::LeftButton:
        break;
    case Qt::RightButton:
        break;
    default:
        break;
    }
}

void GLView::keyReleaseEvent(QKeyEvent *event) {
    if (event->isAutoRepeat())
        QGLWidget::keyReleaseEvent(event);
    else
        switch(event->key()){
        case Qt::Key_Space:
            break;
        case Qt::Key_Z:
            break;
        case Qt::Key_S:
            break;
        default:
            QGLWidget::keyReleaseEvent(event); // dispatch the event to the parent
        }
}

void GLView::mouseMoveEvent(QMouseEvent *event) {
    cout << "motion : " << event->x() << "," << event->y() << endl;
}

void GLView::wheelEvent(QWheelEvent *event) {
    cout << "mouse wheel :" << event->delta() << endl;
}

/** ***************************************************************************
            init/update data
  **/
void GLView::initData() {

    // on initialise le pas pour l'animation
    _step = 0;

    // la classe NURBS utiliser pour evaluer une courbe B-Spline.
    _nurbs = Nurbs();

    // on initialise avec un vecteur nodale vide.
    _nurbs.initialize(0,0);
}

void GLView::updateData() {

    // le pas utilise pour la vitesse du deplacement du point.
    _step += 0.01;

    if (_step > 1) _step = 0;

    // update view :
    updateGL();
}


/** *******************************************************************
             Drawings
**/

void GLView::paintGL() {
    _nurbs.updateView(_step);
}

/** ********************************************************************** **/

void GLView::choice1() {
    _choice = 1;
}

void GLView::choice2() {
    _choice = 2;
}

void GLView::choice3() {
    _choice = 3;
}

void GLView::choice4() {
    _choice = 4;
}

void GLView::choice5() {
    _choice = 5;
}



