#include <iostream>

#include "GLView.h"

#include "MainWindow.h"

#include <QGLFormat>
#include <QGridLayout>
#include <QTimer>
#include <QKeyEvent>
#include <QPushButton>
#include <QtGui>
#include <QDebug>

#include "Error.h"

using namespace std;

/*!
*
* @file
*
* @brief
* @author F. Aubert
*
*/


MainWindow::MainWindow() :
    QMainWindow(NULL) {

    // build a GLWidget
    QGLFormat format=QGLFormat();
    format.setVersion(4,2); // there's a bug somewhere for the compatibility profile on AMD driver : comment this line if there's a problem with the openGL version
    format.setProfile(QGLFormat::CompatibilityProfile);
    format.setSwapInterval(1);
    format.setOption(QGL::DoubleBuffer | QGL::DepthBuffer | QGL::StencilBuffer | QGL::AlphaChannel);
    QGLFormat::setDefaultFormat(format);

    _glView = new GLView(this);

    // application buttons
    _choice.clear();
    _choice.push_back(new QPushButton("Chaikin")); // choice 0
    _choice.push_back(new QPushButton("Dyn & Levin")); // choice 1

    // set up for the main window : set a central widget (main container), and set a gridLayout to this central widget
    QWidget *central=new QWidget(this);
    this->setCentralWidget(central);
    QGridLayout *layout=new QGridLayout(central);

    // add widgets in layout
    for(unsigned int i=0;i<_choice.size();i++) {
        layout->addWidget(_choice[i],i,0);
        connect(_choice[i],SIGNAL(clicked()),this,SLOT(choice()));
    }

    // add the button to show the commands
    QPushButton* command = new QPushButton("Commands");
    layout->addWidget(command,2,0);
    connect(command,SIGNAL(clicked()),this,SLOT(commands()));

    layout->addWidget(_glView,0,1,20,20);

    // update GLWidget every 20ms
    QTimer *timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),_glView,SLOT(updateData())); // calls GLView::update() (that will call paintGL() )
    timer->start(0);

    if (!_choice.empty()) _choice[0]->click();
}

void MainWindow::commands() {
    QFrame* popup1 = new QFrame(this, Qt::Window);
    popup1->resize(600,180);

    QString label = "Commands:\n\n";
    label += "  Left click -> add a non fix point\n";
    label += "  Right click -> add a fix point (only work on Chaikin)\n";
    label += "  W -> increment the nb of iteration\n";
    label += "  S -> Decrement the nb of iteration\n";
    label += "  R -> remove the last added point (If used with Dyn&Levin will work when curve isn't closed)\n";
    label += "  C -> close the curve (And show the computed subcurve)\n";
    label += "  Q -> increment the a value\n";
    label += "  A -> decrement the a value\n";
    label += "  Z -> Chose the particular process or not \n";

    new QLabel(label,popup1);

    popup1->show();
}

void MainWindow::choice() {
    QPushButton *b=dynamic_cast<QPushButton *>(QObject::sender());
    unsigned int i;
    for(i=0;i<_choice.size();++i) if (b==_choice[i]) break;
    _glView->choice(i,b->text().toStdString());
}


// key events
void MainWindow::keyPressEvent(QKeyEvent *e) {
    if (e->key()==Qt::Key_Escape) {
        this->close();
    } else {
        QWidget::keyPressEvent(e); // send the event to the parent widget
    }
}

