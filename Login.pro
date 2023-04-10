# 引入数据库模块
QT       += core gui sql widgets uitools

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    car.cpp \
    main.cpp \
    mainwindow.cpp \
    signup.cpp \
    mysql.cpp \
    init.cpp

HEADERS += \
    car.h \
    mainwindow.h \
    signup.h \
    mysql.h \
    init.h

FORMS += \
    car.ui \
    mainwindow.ui \
    signup.ui \
    init.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# 设置logo
RC_ICONS = logo.ico


RESOURCES += \
    images.qrc

# 引入OpenCV
#INCLUDEPATH += D:\openCV3.2.0\opencv\build\include
#LIBS += -L"D:\openCV3.2.0\opencv\build\x64\vc14\lib\opencv_world320d.lib"

DISTFILES +=

