QT += core gui widgets
TEMPLATE = app
CONFIG += c++17
SOURCES = mainwindow.cpp \
    console.cpp \
    main.cpp \
    radixSort.cpp
HEADERS = mainwindow.h \
    basicheaders.h \
    console.h \
    partitioncustom.h \
    radixSort.h
LIBS += $$top_builddir/components/$(OBJECTS_DIR)/libcomponents.a
INCLUDEPATH += $$top_srcdir/components/
TARGET = radix-sort
PRE_TARGETDEPS += $$top_builddir/components/$(OBJECTS_DIR)/libcomponents.a

RESOURCES +=

FORMS += \
    mainwindow.ui
