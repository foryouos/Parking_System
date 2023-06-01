# 引入数据库模块
QT       += core gui sql widgets uitools

# 音频视频所需要的依赖
QT       += multimedia
QT       += multimediawidgets
QT       += charts
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
# CONFIG += console
CONFIG += c++11
QMAKE_CXXFLAGS+=-std=c++11
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# 去除变量为使用的警告
QMAKE_CXXFLAGS += -Wno-unused-parameter

QMAKE_CXXFLAGS += -Wno-unused-variable

QMAKE_CXXFLAGS_WARN_ON -= -Wunused-function
QMAKE_CXXFLAGS += -Wno-unused-function
#添加 使用OpenMP的pragma
QMAKE_CXXFLAGS += -fopenmp
QMAKE_LFLAGS +=  -fopenmp

LIBS += -fopenmp

# 文件
SOURCES += \
    car.cpp \
    main.cpp \
    signup.cpp \
    mysql.cpp \
    initfile.cpp\
    src/core/chars_identify.cpp \
    src/core/chars_recognise.cpp \
    src/core/chars_segment.cpp \
    src/core/core_func.cpp \
    src/core/feature.cpp \
    src/core/params.cpp \
    src/core/plate_detect.cpp \
    src/core/plate_judge.cpp \
    src/core/plate_locate.cpp \
    src/core/plate_recognize.cpp \
#    src/train/ann_train.cpp \
#    src/train/annCh_train.cpp \
#    src/train/create_data.cpp \
#    src/train/svm_train.cpp \
#    src/train/train.cpp \
    src/util/kv.cpp \
    src/util/program_options.cpp \
    src/util/util.cpp \
    thirdparty/LBP/helper.cpp \
    thirdparty/LBP/lbp.cpp \
    thirdparty/mser/mser2.cpp \
    thirdparty/svm/corrected_svm.cpp \
    thirdparty/textDetect/erfilter.cpp \
    thirdparty/xmlParser/xmlParser.cpp \
    pthreadpool.cpp \
    login.cpp

HEADERS += \
    car.h \
    signup.h \
    mysql.h \
    initfile.h\
    include/easypr/core/character.hpp \
    include/easypr/core/chars_identify.h \
    include/easypr/core/chars_recognise.h \
    include/easypr/core/chars_segment.h \
    include/easypr/core/core_func.h \
    include/easypr/core/feature.h \
    include/easypr/core/params.h \
    include/easypr/core/plate.hpp \
    include/easypr/core/plate_detect.h \
    include/easypr/core/plate_judge.h \
    include/easypr/core/plate_locate.h \
    include/easypr/core/plate_recognize.h \
#    include/easypr/train/ann_train.h \
#    include/easypr/train/annCh_train.h \
#    include/easypr/train/create_data.h \
#    include/easypr/train/svm_train.h \
#    include/easypr/train/train.h \
    include/easypr/util/kv.h \
    include/easypr/util/program_options.h \
    include/easypr/util/switch.hpp \
    include/easypr/util/util.h \
    include/easypr/api.hpp \
    include/easypr/config.h \
    include/easypr/version.h \
    include/easypr.h \
#    test/accuracy.hpp \
#    test/chars.hpp \
#    test/config.hpp \
#    test/plate.hpp \
#    test/result.hpp \
    thirdparty/LBP/helper.hpp \
    thirdparty/LBP/lbp.hpp \
    thirdparty/mser/mser2.hpp \
    thirdparty/svm/precomp.hpp \
    thirdparty/textDetect/erfilter.hpp \
    thirdparty/xmlParser/xmlParser.h \
    pthreadpool.h \
    login.h


FORMS += \
    car.ui \
    signup.ui \
    initfile.ui \
    mainwindow.ui

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

# 引入easyPR所需要的依赖
DISTFILES += \
#    resources/train/ann.7z \
#    resources/train/annCh.7z \
#    resources/train/annGray.7z \
#    resources/train/svm.7z \
    model/ann.xml \
    model/ann_chinese.xml \
    model/annCh.xml \
    model/svm_hist.xml \
#    resources/image/general_test/GroundTruth_others.xml \
#    resources/image/general_test/GroundTruth_windows.xml \
#    resources/image/native_test/GroundTruth_others.xml \
#    resources/image/native_test/GroundTruth_windows.xml \
#    resources/result/Result.xml \
#    resources/text/chinese_mapping \
#    resources/text/dev_team \
#    resources/text/province_mapping \
#    resources/text/thanks \
#    resources/text/batch_test_menu \
#    resources/text/main_menu \
#    resources/text/test_menu \
#    resources/text/train_menu



