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
    controls/origin_control.cpp \
    controls/scale_control.cpp \
    main.cpp \
    mainwindow.cpp \
    nodes/node_2d.cpp \
    nodes/sprite_node.cpp \
    world_view.cpp

HEADERS += \
    ../scene_file.h \
    controls/origin_control.h \
    controls/controllable.h \
    controls/scale_control.h \
    mainwindow.h \
    nodes/abstract_node.h \
    nodes/node.h \
    nodes/node_2d.h \
    nodes/node_2d.h \
    nodes/sprite_node.h \
    world_view.h

FORMS += \
    mainwindow.ui

RESOURCES += \
	rsrc.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
