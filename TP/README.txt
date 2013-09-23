## Additional libraries
> Add the following lines in the .pro file for OpenCV :

LIBS += -L/opt/local/lib \
     -lopencv_core \
     -lopencv_imgproc \
     -lopencv_highgui \
     -lopencv_calib3d \

INCLUDEPATH += /opt/local/include