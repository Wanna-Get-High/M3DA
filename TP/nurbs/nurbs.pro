#-------------------------------------------------
#
# Project created by QtCreator 2011-08-17T15:40:55
#
#-------------------------------------------------

CONFIG += debug_and_release
QT += core gui opengl # should be ok for Qt 4.8 and Qt 5.1
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++0x # C++ 11
##CONFIG += with_glew # decomment this line if glew is required

with_glew:DEFINES+=WITH_GLEW # to check if #include <GL/glew.h> should be done in .cpp / .h

unix:!macx:with_glew {
    CONFIG += link_pkgconfig
    PKGCONFIG += glew
}

macx {
    QMAKE_CXXFLAGS -= -std=c++0x # C++ 11
    INCLUDEPATH += /opt/local/include
    CONFIG += link_pkgconfig
    with_glew:LIBS+=-lglew
    LIBS += -lglew -framework OpenGL
}

win32 {
  CONFIG += console
  with_glew:DEFINES+=GLEW_STATIC
  DEFINES+=_STDCALL_SUPPORTED
  with_glew:LIBS+=-lglew32s
  LIBS+= -lopengl32
}

CONFIG(debug,debug|release) {
  TARGET=$$join(TARGET,,,"_d")
}

DESTDIR = $$_PRO_FILE_PWD_/bin
message(Executable path : $$DESTDIR)

INCLUDEPATH += src
DEPENDPATH += src

SOURCES += \
    src/UtilGL.cpp \
    src/MainWindow.cpp \
    src/main.cpp \
    src/GLView.cpp \
    src/Error.cpp \
    src/nurbs.cpp

HEADERS += \
    src/UtilGL.h \
    src/MainWindow.h \
    src/GLView.h \
    src/glsupport.h \
    src/Error.h \
    src/nurbs.h
