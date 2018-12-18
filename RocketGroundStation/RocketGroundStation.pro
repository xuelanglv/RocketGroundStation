#-------------------------------------------------
#
# Project created by QtCreator 2018-07-28T09:44:59
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets serialport

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

TARGET = RocketGroundStation
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        mainpage.cpp \
    showlabel.cpp \
    fhog.cpp \
    kcftracker.cpp \
    main.cpp \
    common.cpp \
    serialcontroler.cpp

HEADERS += \
        mainpage.h \
    showlabel.h \
    ffttools.hpp \
    fhog.hpp \
    kcftracker.hpp \
    tracker.h \
    common.h \
    recttools.hpp \
    serialcontroler.h

FORMS += \
        mainpage.ui



unix|win32: LIBS += -lUser32

RESOURCES += \
    resources.qrc

INCLUDEPATH += D:/OpenCV/opencv/build/include \
                D:/OpenCV/opencv/build/include/opencv \
                D:/OpenCV/opencv/build/include/opencv2

LIBS += D:/OpenCV/opencv/build/x64/vc15/lib/*.lib
