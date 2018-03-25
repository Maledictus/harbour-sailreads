TARGET = harbour-sailreads

VERSION = 0.1

QT += network xmlpatterns xml

CONFIG += sailfishapp c++11
PKGCONFIG += mlite5

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

LIBS += -loauth

SOURCES += src/harbour-sailreads.cpp \
    src/application.cpp \
    src/debugmessagehandler.cpp \
    src/goodreadsapi.cpp \
    src/oauthwrapper.cpp \
    src/rpcutils.cpp \
    src/sailreadsmanager.cpp \
    src/userupdate.cpp \
    src/userprofile.cpp \
    src/models/baseproxymodel.cpp \
    src/models/bookshelvesmodel.cpp \
    src/models/bookshelfproxymodel.cpp \
    src/objects/bookshelf.cpp \
    src/settings/accountsettings.cpp \
    src/settings/applicationsettings.cpp \
    src/objects/group.cpp \
    src/objects/friend.cpp \
    src/models/groupsmodel.cpp \
    src/models/friendsmodel.cpp \
    src/objects/groupfolder.cpp \
    src/objects/groupmember.cpp \
    src/models/groupfoldersmodel.cpp \
    src/models/groupmembersmodel.cpp \
    src/models/usergroupsmodel.cpp \
    src/models/foundgroupsmodel.cpp \
    src/objects/topic.cpp \
    src/models/groupfoldertopicsmodel.cpp

HEADERS += src/application.h \
    src/debugmessagehandler.h \
    src/goodreadsapi.h \
    src/oauthwrapper.h \
    src/rpcutils.h \
    src/sailreadsmanager.h \
    src/types.h \
    src/userprofile.h \
    src/userupdate.h \
    src/models/baseproxymodel.h \
    src/models/bookshelfproxymodel.h \
    src/models/bookshelvesmodel.h \
    src/objects/bookshelf.h \
    src/settings/accountsettings.h \
    src/settings/applicationsettings.h \
    src/objects/group.h \
    src/objects/friend.h \
    src/models/groupsmodel.h \
    src/models/friendsmodel.h \
    src/objects/groupfolder.h \
    src/objects/groupmember.h \
    src/models/groupfoldersmodel.h \
    src/models/groupmembersmodel.h \
    src/models/usergroupsmodel.h \
    src/models/foundgroupsmodel.h \
    src/objects/topic.h \
    src/models/groupfoldertopicsmodel.h

DISTFILES += rpm/harbour-sailreads.changes.in \
    rpm/harbour-sailreads.changes.run.in \
    rpm/harbour-sailreads.spec \
    rpm/harbour-sailreads.yaml \
    translations/*.ts \
    harbour-sailreads.desktop

RESOURCES += $${TARGET}.qrc

SAILFISHAPP_ICONS = 86x86 108x108 128x128

CONFIG += sailfishapp_i18n

TRANSLATIONS += translations/harbour-sailreads-ru.ts
