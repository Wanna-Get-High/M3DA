################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Error.cpp \
../src/GLView.cpp \
../src/Implicit.cpp \
../src/MainWindow.cpp \
../src/UtilGL.cpp \
../src/grid.cpp \
../src/main.cpp 

OBJS += \
./src/Error.o \
./src/GLView.o \
./src/Implicit.o \
./src/MainWindow.o \
./src/UtilGL.o \
./src/grid.o \
./src/main.o 

CPP_DEPS += \
./src/Error.d \
./src/GLView.d \
./src/Implicit.d \
./src/MainWindow.d \
./src/UtilGL.d \
./src/grid.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/Users/WannaGetHigh/Qt5.1.1/5.1.1/clang_64/include/QtWidgets -I/Users/WannaGetHigh/Qt5.1.1/5.1.1/clang_64/include/QtCore -I/Users/WannaGetHigh/Qt5.1.1/5.1.1/clang_64/include/QtGui -I/Users/WannaGetHigh/Qt5.1.1/5.1.1/clang_64/include/QtOpenGL -I/Users/WannaGetHigh/Qt5.1.1/5.1.1/clang_64/include/QtWidgets -I/Users/WannaGetHigh/Qt5.1.1/5.1.1/clang_64/include -include/Users/WannaGetHigh/Qt5.1.1/5.1.1/clang_64/include/QtCore/qglobal.h -include/Users/WannaGetHigh/Qt5.1.1/5.1.1/clang_64/include/QtGui/qopengl.h -include/Users/WannaGetHigh/Qt5.1.1/5.1.1/clang_64/include/QtOpenGL/QGLWidget -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


