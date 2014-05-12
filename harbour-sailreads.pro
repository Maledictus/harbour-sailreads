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

QT += webkit sql xml

SOURCES += src/harbour-sailreads.cpp \
    src/networkaccessmanager.cpp \
    src/localstorage.cpp \
    src/util.cpp \
    src/sailreadsmanager.cpp \
    src/oauthwrapper.cpp \
    src/goodreadsapi.cpp \
    src/recentupdatesmodel.cpp \
	src/userprofile.cpp \
	src/notificationsmodel.cpp \
	src/friendsmodel.cpp \
	src/basemodel.cpp \
	src/groupsmodel.cpp \
	src/shelvesmodel.cpp \
    src/curlwrapper.cpp

HEADERS += \
    src/networkaccessmanager.h \
    src/localstorage.h \
    src/util.h \
    src/sailreadsmanager.h \
    src/oauthwrapper.h \
    src/goodreadsapi.h \
    src/structures.h \
    src/recentupdatesmodel.h \
	src/userprofile.h \
	src/notificationsmodel.h \
	src/friendsmodel.h \
	src/basemodel.h \
    src/objectsmodel.h \
	src/groupsmodel.h \
	src/shelvesmodel.h \
    src/curlwrapper.h

LIBS += -loauth -lcurl

OTHER_FILES += rpm/harbour-sailreads.spec \
    rpm/harbour-sailreads.yaml \
	harbour-sailreads.desktop \
	qml/harbour-sailreads.qml \
	qml/AuthManager.qml \
    qml/FriendsUpdatesPage.qml \
    qml/ProfilePage.qml \
    qml/NotificationsPage.qml \
    qml/FriendsPage.qml \
    qml/GroupsPage.qml \
    qml/ShelvesPage.qml \
    qml/AddEditShelfPage.qml

