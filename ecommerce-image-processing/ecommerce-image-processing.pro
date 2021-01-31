QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    enhancer.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    enhancer.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# OpenCV

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../build-openCV/lib/release/ -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_videoio -lopencv_calib3d
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../build-openCV/lib/debug/ -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_videoio -lopencv_calib3d
#else:unix: LIBS += -L$$PWD/../../build-openCV/lib/ -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_videoio -lopencv_calib3d

#LIBS += $(shell pkg-config opencv --libs)
#INCLUDEPATH += /usr/local/include/opencv4
#DEPENDPATH += $$PWD/../../build-openCV/include

#PKGCONFIG += opencv


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../opencv-master/build/lib/release/ -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_videoio -lopencv_calib3d
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../opencv-master/build/lib/debug/ -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_videoio -lopencv_calib3d
else:unix: LIBS += -L$$PWD/../../opencv-master/build/lib/ -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_videoio -lopencv_calib3d -lopencv_imgproc

INCLUDEPATH += /usr/local/include/opencv4
DEPENDPATH += $$PWD/../../opencv-master/include

PKGCONFIG += opencv
