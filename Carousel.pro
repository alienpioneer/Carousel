QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CarouselBasic.cpp \
    Conveyor/Conveyor_T2K.cpp \
    RectangleWidget.cpp \
    SemicircleWidgetAlt.cpp \
    SingleLevelCarousel.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    CarouselBasic.h \
    Conveyor/Conveyor_T2K.h \
    MainWindow.h \
    RectangleWidget.h \
    SemicircleWidgetAlt.h \
    SingleLevelCarousel.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
