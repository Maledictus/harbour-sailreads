# The name of your app.
# NOTICE: name defined in TARGET has a corresponding QML filename.
#         If name defined in TARGET is changed, following needs to be
#         done to match new name:
#         - corresponding QML filename must be changed
#         - desktop icon filename must be changed
#         - desktop filename must be changed
#         - icon definition filename in desktop file must be changed
QMAKE_CXXFLAGS += -std=c++0x

TARGET = harbour-sailreads

CONFIG += sailfishapp

QT += webkit sql

SOURCES += src/harbour-sailreads.cpp \
    src/networkaccessmanager.cpp \
    src/localstorage.cpp \
    src/util.cpp \
    src/sailreadsmanager.cpp \
    src/oauthwrapper.cpp \
    src/goodreadsapi.cpp

HEADERS += \
    src/networkaccessmanager.h \
    src/localstorage.h \
    src/util.h \
    src/sailreadsmanager.h \
    src/oauthwrapper.h \
    src/goodreadsapi.h

LIBS += -loauth

OTHER_FILES += rpm/harbour-sailreads.spec \
    rpm/harbour-sailreads.yaml \
	harbour-sailreads.desktop \
	qml/harbour-sailreads.qml \
	qml/AuthManager.qml

