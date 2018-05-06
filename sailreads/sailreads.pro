TARGET = harbour-sailreads

VERSION = 0.1

QT += network xmlpatterns xml

CONFIG += sailfishapp c++11 sailfishapp_i18n

PKGCONFIG += mlite5

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

SOURCES += src/harbour-sailreads.cpp \
    src/application.cpp \
    src/debugmessagehandler.cpp \
    src/goodreadsapi.cpp \
    src/oauthwrapper.cpp \
    src/rpcutils.cpp \
    src/sailreadsmanager.cpp \
    src/userupdate.cpp \
    src/models/baseproxymodel.cpp \
    src/models/bookshelfproxymodel.cpp \
    src/models/bookshelvesmodel.cpp \
    src/models/commentsmodel.cpp \
    src/models/foundgroupsmodel.cpp \
    src/models/friendsmodel.cpp \
    src/models/groupfoldersmodel.cpp \
    src/models/groupfoldertopicsmodel.cpp \
    src/models/groupsmodel.cpp \
    src/models/groupmembersmodel.cpp \
    src/models/reviewsmodel.cpp \
    src/models/usergroupsmodel.cpp \
    src/objects/author.cpp \
    src/objects/book.cpp \
    src/objects/bookshelf.cpp \
    src/objects/comment.cpp \
    src/objects/friend.cpp \
    src/objects/group.cpp \
    src/objects/groupfolder.cpp \
    src/objects/groupmember.cpp \
    src/objects/review.cpp \
    src/objects/series.cpp \
    src/objects/serieswork.cpp \
    src/objects/topic.cpp \
    src/objects/user.cpp \
    src/objects/work.cpp \
    src/qmltypes/bookitem.cpp \
    src/qmltypes/groupitem.cpp \
    src/qmltypes/topicitem.cpp \
    src/qmltypes/userprofile.cpp \
    src/settings/accountsettings.cpp \
    src/settings/applicationsettings.cpp \


HEADERS += src/application.h \
    src/debugmessagehandler.h \
    src/goodreadsapi.h \
    src/oauthwrapper.h \
    src/rpcutils.h \
    src/sailreadsmanager.h \
    src/types.h \
    src/userupdate.h \
    src/models/baseproxymodel.h \
    src/models/bookshelfproxymodel.h \
    src/models/bookshelvesmodel.h \
    src/models/commentsmodel.h \
    src/models/foundgroupsmodel.h \
    src/models/friendsmodel.h \
    src/models/groupfoldersmodel.h \
    src/models/groupfoldertopicsmodel.h \
    src/models/groupsmodel.h \
    src/models/groupmembersmodel.h \
    src/models/reviewsmodel.h \
    src/models/usergroupsmodel.h \
    src/objects/author.h \
    src/objects/book.h \
    src/objects/bookshelf.h \
    src/objects/comment.h \
    src/objects/friend.h \
    src/objects/group.h \
    src/objects/groupfolder.h \
    src/objects/groupmember.h \
    src/objects/review.h \
    src/objects/series.h \
    src/objects/serieswork.h \
    src/objects/topic.h \
    src/objects/user.h \
    src/objects/work.h \
    src/qmltypes/groupitem.h \
    src/qmltypes/bookitem.h \
    src/qmltypes/topicitem.h \
    src/qmltypes/userprofile.h \
    src/settings/accountsettings.h \
    src/settings/applicationsettings.h \

DISTFILES += translations/*.ts \
    sailreads.desktop

QML_SOURCES = \
        qml/*.qml \
        qml/pages/*.qml \
        qml/cover/*.qml \
        qml/components/*.qml \
        qml/dialogs/*.qml \
        qml/utils/*.js

RESOURCES += sailreads.qrc

SAILFISHAPP_ICONS = 86x86 108x108 128x128

TRANSLATIONS += translations/harbour-sailreads-ru.ts

QMAKE_RPATHDIR += $$CURRENT_RPATH_DIR

LIBS += -L../liboauth -loauth
INCLUDEPATH += ../liboauth/src/

lupdate_only {
SOURCES += $$QML_SOURCES
}
