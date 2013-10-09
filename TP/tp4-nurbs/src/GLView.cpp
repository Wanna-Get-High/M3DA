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
  if (event->button()==Qt::LeftButton) {
    cout << "left mouse : " << event->x() << "," << event->y() << endl;
  }
  if (event->button()==Qt::RightButton) {
    cout << "right mouse : " << event->x() << "," << event->y() << endl;
  }
}

void GLView::mouseMoveEvent(QMouseEvent *event) {
  cout << "motion : " << event->x() << "," << event->y() << endl;
}

void GLView::mouseReleaseEvent(QMouseEvent *event) {
  switch(event->button()){
  case Qt::LeftButton:
      cout << "left mouse released" << endl;
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
        cout << "Z pressed" << endl;
      break;
    case Qt::Key_S:
        cout << "S pressed" << endl;
      break;
    case Qt::Key_Space:
        cout << "space pressed" << endl;
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
      case Qt::Key_Space:
        cout << "space released" << endl;
      case Qt::Key_Z:
        break;
      case Qt::Key_S:
        break;
      default:
        QGLWidget::keyReleaseEvent(event); // dispatch the event to the parent
    }
}



/** ***************************************************************************
  init/update data
  **/
void GLView::initData() {
}

void GLView::updateData() {
  // update data :


  // update view :
  updateGL();
}


/** ******************************************************************* **/
/**
  Drawings
**/



void GLView::paintGL() {
//  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//  glPushMatrix();

//  /// drawing examples

//  glColor3f(1,0,0);
//  ugl::drawText("drawing example",0.2,0.2);

//  glLineWidth(3);
//  glColor3f(0,1,0);
//  glBegin(GL_LINE_STRIP);
//  glVertex2f(0,-0.5);
//  glVertex2f(-0.5,0.5);
//  glVertex2f(0.5,0.5);
//  glVertex2f(0,-0.5);
//  glEnd();

//  glPointSize(10);
//  glColor3f(0,0,1);
//  glBegin(GL_POINTS);
//  glVertex2f(0,-0.5);
//  glVertex2f(-0.5,0.5);
//  glVertex2f(0.5,0.5);
//  glEnd();


//  glPopMatrix();

  Nurbs nurbs = Nurbs();
  nurbs.initialize(3,4);
  nurbs.drawN(0,4);
}

/** ********************************************************************** **/

void GLView::choice1() {
  cout << "choice 1 " << endl;
}
void GLView::choice2() {
  cout << "choice 2 " << endl;
}
void GLView::choice3() {
  cout << "choice 3 " << endl;
}
void GLView::choice4() {
  cout << "choice 4 " << endl;
}
void GLView::choice5() {
  cout << "choice 5 " << endl;
}



