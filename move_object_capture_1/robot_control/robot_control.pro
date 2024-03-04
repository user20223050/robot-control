#-------------------------------------------------
#
# Project created by QtCreator 2023-12-27T14:10:20
#
#-------------------------------------------------

QT          += core gui
QT          += core
QT          += serialport
#INCLUDEPATH += E:\dask\move_object_capture_c\eigen-3.4.0\eigen-3.4.0\Eigen
#INCLUDEPATH += E:\dask\move_object_capture_c\qpOASES-master\qpOASES-master\include
INCLUDEPATH += /home/ni/move_object_capture_1/eigen-3.4.0/eigen-3.4.0/Eigen
INCLUDEPATH += /home/ni/move_object_capture_1/qpOASES-master/qpOASES-master/include
CONFIG += resources_big
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += console

TARGET = robot_control
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        widget.cpp \
    robot.cpp \
    mobile_pose.cpp \
    slave_copmputer.cpp \
    ../qpOASES-master/qpOASES-master/src/BLASReplacement.cpp \
    ../qpOASES-master/qpOASES-master/src/Bounds.cpp \
    ../qpOASES-master/qpOASES-master/src/Constraints.cpp \
    ../qpOASES-master/qpOASES-master/src/Flipper.cpp \
    ../qpOASES-master/qpOASES-master/src/Indexlist.cpp \
    ../qpOASES-master/qpOASES-master/src/LAPACKReplacement.cpp \
    ../qpOASES-master/qpOASES-master/src/Matrices.cpp \
    ../qpOASES-master/qpOASES-master/src/MessageHandling.cpp \
    ../qpOASES-master/qpOASES-master/src/Options.cpp \
    ../qpOASES-master/qpOASES-master/src/OQPinterface.cpp \
    ../qpOASES-master/qpOASES-master/src/QProblem.cpp \
    ../qpOASES-master/qpOASES-master/src/QProblemB.cpp \
    ../qpOASES-master/qpOASES-master/src/SolutionAnalysis.cpp \
    ../qpOASES-master/qpOASES-master/src/SparseSolver.cpp \
    ../qpOASES-master/qpOASES-master/src/SQProblem.cpp \
    ../qpOASES-master/qpOASES-master/src/SQProblemSchur.cpp \
    ../qpOASES-master/qpOASES-master/src/SubjectTo.cpp \
    ../qpOASES-master/qpOASES-master/src/Utils.cpp \
    mpc_control.cpp

HEADERS  += widget.h \
    robot.h \
    mobile_pose.h \
    slave_copmputer.h \
    ../qpOASES-master/qpOASES-master/include/qpOASES.hpp \
    mpc_control.h

FORMS    += widget.ui

DISTFILES += \
    ../qpOASES-master/qpOASES-master/src/Makefile
