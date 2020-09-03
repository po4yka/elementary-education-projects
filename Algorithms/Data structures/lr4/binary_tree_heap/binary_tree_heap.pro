QT += core gui widgets
TEMPLATE = app
CONFIG += c++17
SOURCES = mainwindow.cpp \
    console.cpp \
    drawing.cpp \
    main.cpp \
    setupUI.cpp \
    tree.cpp \
    treeCheckingFunctions.cpp \
    treeHandlers.cpp
HEADERS += \
    basicheaders.h \
    console.h \
    customvector.h \
    mainwindow.h \
    tree.h \
    ui_mainwindow.h
LIBS += $$top_builddir/components/$(OBJECTS_DIR)/libcomponents.a
INCLUDEPATH += $$top_srcdir/components/
TARGET = binary_tree_heap
PRE_TARGETDEPS += $$top_builddir/components/$(OBJECTS_DIR)/libcomponents.a

RESOURCES +=

FORMS += \
    mainwindow.ui
