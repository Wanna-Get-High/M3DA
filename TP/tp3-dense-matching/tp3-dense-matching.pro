TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += dense-matching.cpp \
    glue.cpp \
    francois-lepan.cpp

OTHER_FILES += left.png \
    right.png \

HEADERS += \
    glue.hpp \
    francois-lepan.hpp


LIBS += -L/opt/local/lib \
     -lopencv_core \
     -lopencv_imgproc \
     -lopencv_highgui \
     -lopencv_calib3d \

INCLUDEPATH += /opt/local/include
