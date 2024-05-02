QT += core gui widgets charts serialport

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    driverdev.cpp \
    logfile.cpp \
    main.cpp \
    mainwindow.cpp \
    pythronmotordriver.cpp \
    thorlabscamera.cpp \
    thorlabs/tl_camera_sdk_load.c \
    led_driver.cpp \
    threadedtimer.cpp


win32: SOURCES += win11/analogdaq.cpp
unix: SOURCES += linux/analogdaq.cpp


HEADERS += \
    driverdev.h \
    logfile.h \
    mainwindow.h \
    pythronmotordriver.h \
    thorlabscamera.h \
    led_driver.h \
    threadedtimer.h


win32: HEADERS += win11/Analog.h win11/Board.h win11/cbw.h win11/analogdaq.h
unix: HEADERS += linux/usb-2408.h linux/pmd.h linux/analogdaq.h

FORMS += \
    mainwindow.ui




# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/micado/bin
!isEmpty(target.path): INSTALLS += target

# library for DAQ
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/win11/ -lcbw64
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/win11/ -lcbw64
else:unix: LIBS += -L/opt/micado/lib/ -Wl,-rpath=/opt/micado/lib/ -lmccusb -ldl -lpthread
unix: INCLUDEPATH += /opt/micado/include $$PWD/linux
unix: DEPENDPATH += /opt/micado/include
win32: INCLUDEPATH += $$PWD/win11
unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += libusb-1.0 hidapi-libusb


INCLUDEPATH += $$PWD/thorlabs
