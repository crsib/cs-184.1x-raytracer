#-------------------------------------------------
#
# Project created by QtCreator 2013-11-09T23:04:38
#
#-------------------------------------------------

QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = raytracer
TEMPLATE = app


SOURCES += main.cpp\
	RTMainWindow.cpp \
    rt/Object.cpp \
    rt/Sphere.cpp \
	rt/Triangle.cpp \
    rt/Camera.cpp \
	rt/Scene.cpp

HEADERS  += RTMainWindow.h \
    rt/AABB.h \
	rt/Ray.h \
    rt/Object.h \
    rt/Sphere.h \
    rt/Triangle.h \
    rt/Transform.h \
    rt/Camera.h \
	rt/Scene.h \
    rt/Light.h \
    rt/Hit.h \
    rt/Material.h

INCLUDEPATH += .

FORMS    += RTMainWindow.ui
