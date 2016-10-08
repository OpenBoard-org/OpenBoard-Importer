TEMPLATE = app
TARGET = OpenBoardImporter

CONFIG += debug_and_release

QT += core \
      widgets

HEADERS += \
    UBUpgradeToOef.h \
    UBUpgradeToOefDelegate.h \
    UBActionWidget.h \
    UBDirectories.h

SOURCES += \
    UBUpgradeToOef.cpp \
    main.cpp \
    UBUpgradeToOefDelegate.cpp \
    UBActionWidget.cpp \
    UBDirectories.cpp
