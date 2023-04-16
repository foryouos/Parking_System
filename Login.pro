# 引入数据库模块
QT       += core gui sql widgets uitools

# 音频视频所需要的依赖
QT       += multimedia
QT       += multimediawidgets
QT       += charts
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
    initfile.cpp

HEADERS += \
    car.h \
    mainwindow.h \
    signup.h \
    mysql.h \
    initfile.h

FORMS += \
    car.ui \
    mainwindow.ui \
    signup.ui \
    initfile.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# 设置logo
RC_ICONS = logo.ico


RESOURCES += \
    images.qrc

# 引入OpenCV
INCLUDEPATH += D:\Qt\build\install\include\
               D:\Qt\build\install\include\opencv2\
               D:\Qt\build\install\include\opencv

LIBS += -L D:\Qt\build\install\x64\mingw\lib\libopencv_*.a


