QT   += core gui widgets webenginewidgets

TARGET = TabsAndMenues
TEMPLATE = app

CONFIG += c++11

SOURCES += \
    main.cpp \
    browser.cpp \
    browserwindow.cpp \
    webengineview.cpp

HEADERS += \
    browser.h \
    browserwindow.h \
    webengineview.h

FORMS += \
    browserwindow.ui

RESOURCES += \
    resource.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bins
!isEmpty(target.path): INSTALLS += target


