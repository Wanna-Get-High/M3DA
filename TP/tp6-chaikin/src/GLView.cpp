/**

  @author F. Aubert
  **/


#include "GLView.h"
#include <iostream>
#include <QPushButton>

using namespace std;
using namespace prog3d;


/// ctor
GLView::GLView(QWidget *parent) : QGLWidget(parent) {

    setFocusPolicy(Qt::StrongFocus); // this widget can now catch the keyboard events

    _choice = 0;
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

    // on met les coordonnees a la bonne echelle.
    double x = event->x() / (double)(width()/2) -1;
    double y = -(event->y() / (double)(height()/2) -1);

    if (event->button()==Qt::LeftButton) {        
        _subdivision.addPointToCurve(x, y, false);
    } else if (event->button()==Qt::RightButton) {
        _subdivision.addPointToCurve(x, y, true);
    }
}

void GLView::mouseMoveEvent(QMouseEvent *event) {
    cout << "motion : " << event->x() << "," << event->y() << endl;
}

void GLView::mouseReleaseEvent(QMouseEvent *event) {
    switch(event->button()){
    case Qt::LeftButton:
        break;
    case Qt::RightButton:
        break;
    default:break;
    }
}

void GLView::wheelEvent(QWheelEvent *event) {
    cout << "mouse wheel :" << event->delta() << endl;
}


void GLView::keyPressEvent(QKeyEvent *event) {
    if (event->isAutoRepeat())
        QGLWidget::keyPressEvent(event);
    else
        switch(event->key()){
        case Qt::Key_Z:
            cout << "particular Chaikin" << endl;
            _subdivision.useParticularChaikin();
            break;
        case Qt::Key_Q:
            cout << "increment a value" << endl;
            _subdivision.incrA();
            break;
        case Qt::Key_A:
            cout << "decrement a value" << endl;
            _subdivision.decrA();
            break;
        case Qt::Key_W:
            cout << "increment nb iteration" << endl;
            _subdivision.incrNbIteration();
            break;
        case Qt::Key_S:
            cout << "decrement nb iteration" << endl;
            _subdivision.decrNbIteration();
            break;
        case Qt::Key_C:
            if(_choice == 1) {
                cout << "close/open the curve" << endl;
                _subdivision.closeCurve();
            } else {
                cout << "Only work on Dyn and Levin curve" << endl;
            }
            break;
        case Qt::Key_R:
            cout << "remove the last point (if the curve is closed -> won't remove one)" << endl;
            _subdivision.removeLastPoint();
            break;
        default:
            QGLWidget::keyPressEvent(event); // dispatch the event to the parent
        }
}

void GLView::keyReleaseEvent(QKeyEvent *event) {
    if (event->isAutoRepeat())
        QGLWidget::keyReleaseEvent(event);
    else
        switch(event->key()){
        default:
            QGLWidget::keyReleaseEvent(event);
        }
}

/** ***************************************************************************
  init/update data
  **/
void GLView::initData() { }

void GLView::updateData() {
    updateGL();
}


/** ******************************************************************* **/
/**
  Drawings
**/

void GLView::paintGL() {
    /// clears the window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    string choice;
    char nbIteration [80];
    char aValue [80];
    string isClosed;
    string isParticular;

    if (_subdivision.choice() == 0) choice = " Chaikin";
    else                            choice = " Dyn and Levin";

    sprintf(nbIteration, " Nb iteration : %d", _subdivision.nbIteration());
    sprintf(aValue, " a : %f", _subdivision.a());

    string nbIt = string(nbIteration);
    string aVal = string(aValue);

    if (_subdivision.isClosed()) isClosed = " Closed";
    else                         isClosed = " Not closed";

    if (_subdivision.isParticular())    isParticular = " Is Particular";
    else                                isParticular = " Isn't' Particular";

    // draw the choice
    glColor3d(1,0,0);
    ugl::drawText(choice, 0, 0);

    // draw the number of iteration
    glColor3d(0,0,1);
    ugl::drawText(nbIt, 0, 0.03);

    // draw the value of a
    glColor3d(1,0,0);
    ugl::drawText(aVal, 0, 0.06);

    // tell if we use the particular process for chaikin
    glColor3d(0,0,1);
    ugl::drawText(isParticular, 0, 0.09);

    // tell if the curve is closed
    glColor3d(1,0,0);
    ugl::drawText(isClosed, 0, 0.12);

     _subdivision.drawCurve();
     _subdivision.drawFixedPoint();
}


/** ********************************************************************** **/

void GLView::choice(int i,const string &s) {
    // i = button number, s = button text
    cout << "choice " << i << " " << s << endl;
    _choice=i;
    _choiceText=s;

    _subdivision.resetCurves();

    switch(_choice) {
    case 0:
        _subdivision.useChaikin();
        break;
    case 1:
        _subdivision.useDynAndLevin();
        break;
    }
}



