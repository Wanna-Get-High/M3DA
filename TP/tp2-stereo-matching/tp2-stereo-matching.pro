TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES +=glue.cpp \
    prenom-nom.cpp \
    stereo-matching.cpp \

HEADERS += \
    glue.hpp \
    prenom-nom.hpp

OTHER_FILES += \
    left.png \
    left.xml \
    right.png \
    right.xml \


LIBS += -L/opt/local/lib \
     -lopencv_core \
     -lopencv_imgproc \
     -lopencv_highgui \
     -lopencv_calib3d \

INCLUDEPATH += /opt/local/include
