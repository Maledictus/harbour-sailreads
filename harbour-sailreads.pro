TARGET = harbour-sailreads

VERSION = 0.3.0

QT += network xmlpatterns xml

CONFIG += sailfishapp c++11 sailfishapp_i18n

PKGCONFIG += mlite5

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

SOURCES += src/harbour-sailreads.cpp \
    src/application.cpp \
    src/debugmessagehandler.cpp \
    src/goodreadsapi.cpp \
    src/rpcutils.cpp \
    src/sailreadsmanager.cpp \
    src/userupdate.cpp \
    src/models/baseproxymodel.cpp \
    src/models/bookshelvesmodel.cpp \
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
    src/qmltypes/seriesitem.cpp \
    src/qmltypes/topicitem.cpp \
    src/qmltypes/userprofile.cpp \
    src/settings/accountsettings.cpp \
    src/settings/applicationsettings.cpp \
    src/objects/friendrequest.cpp \
    src/oauth1.cpp \
    src/models/followersmodel.cpp \
    src/models/followingsmodel.cpp \
    src/qmltypes/authorprofileitem.cpp \
    src/models/basebooksmodel.cpp \
    src/models/authorbooksmodel.cpp \
    src/qmltypes/reviewitem.cpp \
    src/models/basecommentsmodel.cpp \
    src/models/topiccommentsmodel.cpp \
    src/models/reviewcommentsmodel.cpp \
    src/authserver.cpp \
    src/models/authorseriesmodel.cpp \
    src/models/bookeditionsmodel.cpp \
    src/objects/notification.cpp \
    src/objects/message.cpp \
    src/models/messagesmodel.cpp \
    src/qmltypes/messageitem.cpp \
    src/models/notificationsmodel.cpp \
    src/models/searchreviewsmodel.cpp


HEADERS += src/application.h \
    src/debugmessagehandler.h \
    src/goodreadsapi.h \
    src/rpcutils.h \
    src/sailreadsmanager.h \
    src/types.h \
    src/userupdate.h \
    src/models/baseproxymodel.h \
    src/models/bookshelvesmodel.h \
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
    src/qmltypes/seriesitem.h \
    src/qmltypes/topicitem.h \
    src/qmltypes/userprofile.h \
    src/settings/accountsettings.h \
    src/settings/applicationsettings.h \
    src/objects/friendrequest.h \
    src/oauth1.h \
    src/models/followersmodel.h \
    src/models/followingsmodel.h \
    src/qmltypes/authorprofileitem.h \
    src/models/basebooksmodel.h \
    src/models/authorbooksmodel.h \
    src/qmltypes/reviewitem.h \
    src/models/basecommentsmodel.h \
    src/models/topiccommentsmodel.h \
    src/models/reviewcommentsmodel.h \
    src/authserver.h \
    src/models/authorseriesmodel.h \
    src/models/bookeditionsmodel.h \
    src/objects/notification.h \
    src/objects/message.h \
    src/models/messagesmodel.h \
    src/qmltypes/messageitem.h \
    src/models/notificationsmodel.h \
    src/models/searchreviewsmodel.h

DISTFILES += translations/*.ts \
    sailreads.desktop \
    rpm/harbour-sailreads.changes.run.in \
    rpm/harbour-sailreads.spec \
    rpm/harbour-sailreads.changes

QML_SOURCES = \
        qml/*.qml \
        qml/pages/*.qml \
        qml/cover/*.qml \
        qml/components/*.qml \
        qml/dialogs/*.qml \
        qml/utils/*.js

RESOURCES += sailreads.qrc

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172

TRANSLATIONS += translations/harbour-sailreads.ts

lupdate_only {
SOURCES += $$QML_SOURCES
}
