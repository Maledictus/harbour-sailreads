TARGET = harbour-sailreads

VERSION = 0.1

QT += network xmlpatterns

CONFIG += link_pkgconfig sailfishapp c++11 sailfishapp_i18n
PKGCONFIG += mlite5

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

LIBS += -loauth

SOURCES += src/harbour-sailreads.cpp \
    src/application.cpp \
    src/debugmessagehandler.cpp \
    src/settings/applicationsettings.cpp \
    src/sailreadsmanager.cpp \
    src/goodreadsapi.cpp \
    src/settings/accountsettings.cpp \
    src/oauthwrapper.cpp

HEADERS += src/application.h \
    src/debugmessagehandler.h \
    src/settings/applicationsettings.h \
    src/sailreadsmanager.h \
    src/goodreadsapi.h \
    src/settings/accountsettings.h \
    src/oauthwrapper.h

DISTFILES += qml/harbour-sailreads.qml \
    qml/cover/CoverPage.qml \
    qml/pages/AuthorizationPage.qml \
    qml/pages/SplashScreenPage.qml \
    rpm/harbour-sailreads.changes.in \
    rpm/harbour-sailreads.changes.run.in \
    rpm/harbour-sailreads.spec \
    rpm/harbour-sailreads.yaml \
    translations/*.ts \
    harbour-sailreads.desktop

RESOURCES += $${TARGET}.qrc

SAILFISHAPP_ICONS = 86x86 108x108 128x128

lupdate_only{
SOURCES = qml/harbour-sailreads.qml \
    qml/cover/CoverPage.qml \
    qml/pages/AuthorizationPage.qml \
    qml/pages/SplashScreenPage.qml
}

TRANSLATIONS += translations/harbour-sailreads-ru.ts
