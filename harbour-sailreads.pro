TARGET = harbour-sailreads

VERSION = 0.4.0

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
    src/models/searchreviewsmodel.cpp \
    src/models/updatesmodel.cpp \
    src/objects/update.cpp \
    src/objects/update/commentupdate.cpp \
    src/objects/update/reviewupdate.cpp \
    src/objects/update/readstatusupdate.cpp \
    src/objects/update/userstatusupdate.cpp \
    src/models/profileupdatesmodel.cpp \
    src/itemrequestcanceler.cpp \
    src/modelrequestcanceler.cpp \
    src/objects/readstatus.cpp \
    src/qmltypes/readstatusitem.cpp \
    src/models/commentsmodel.cpp \
    src/models/readstatuscommentsmodel.cpp \
    src/objects/userstatus.cpp \
    src/qmltypes/userstatusitem.cpp \
    src/models/userstatuscommentsmodel.cpp \
    src/objects/recommendation.cpp \
    src/qmltypes/recommendationitem.cpp \
    src/models/recommendationcommentsmodel.cpp \
    src/models/friendsrequestsmodel.cpp \
    src/models/friendsrecommendationsmodel.cpp \
    src/objects/friendrecommendation.cpp \
    src/models/foundbooksmodel.cpp \
    src/objects/quote.cpp \
    src/models/quotesbasemodel.cpp \
    src/models/userquotesmodel.cpp \
    src/models/bookquotesmodel.cpp


HEADERS += src/application.h \
    src/debugmessagehandler.h \
    src/goodreadsapi.h \
    src/rpcutils.h \
    src/sailreadsmanager.h \
    src/types.h \
    src/userupdate.h \
    src/models/basemodel.h \
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
    src/models/searchreviewsmodel.h \
    src/models/updatesmodel.h \
    src/objects/update.h \
    src/objects/update/commentupdate.h \
    src/objects/update/reviewupdate.h \
    src/objects/update/readstatusupdate.h \
    src/objects/update/userstatusupdate.h \
    src/models/profileupdatesmodel.h \
    src/itemrequestcanceler.h \
    src/modelrequestcanceler.h \
    src/objects/readstatus.h \
    src/qmltypes/readstatusitem.h \
    src/models/commentsmodel.h \
    src/models/readstatuscommentsmodel.h \
    src/objects/userstatus.h \
    src/qmltypes/userstatusitem.h \
    src/models/userstatuscommentsmodel.h \
    src/objects/recommendation.h \
    src/qmltypes/recommendationitem.h \
    src/models/recommendationcommentsmodel.h \
    src/models/friendsrequestsmodel.h \
    src/models/friendsrecommendationsmodel.h \
    src/objects/friendrecommendation.h \
    src/models/foundbooksmodel.h \
    src/objects/quote.h \
    src/models/quotesbasemodel.h \
    src/models/userquotesmodel.h \
    src/models/bookquotesmodel.h

DISTFILES += translations/*.ts \
    sailreads.desktop \
    rpm/harbour-sailreads.changes.run.in \
    rpm/harbour-sailreads.spec \
    rpm/harbour-sailreads.changes

RESOURCES += sailreads.qrc

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172

TRANSLATIONS += translations/harbour-sailreads.ts

lupdate_only {
SOURCES = qml/harbour-sailreads.qml \
    qml/components/BaseActionsItem.qml \
    qml/components/BaseImage.qml \
    qml/components/BookListItem.qml \
    qml/components/BookShelfListItem.qml \
    qml/components/BoxBackground.qml \
    qml/components/ClickableLabel.qml \
    qml/components/CollapsedLabel.qml \
    qml/components/CommentListItem.qml \
    qml/components/FriendListItem.qml \
    qml/components/FriendsRecommendationsListView.qml \
    qml/components/FriendsRequestsListView.qml \
    qml/components/GroupListItem.qml \
    qml/components/HighlightImage.qml \
    qml/components/HighlightImageBase.qml \
    qml/components/IconText.qml \
    qml/components/IconTextButton.qml \
    qml/components/KeyValueLabel.qml \
    qml/components/MessageListItem.qml \
    qml/components/MessagesFolder.qml \
    qml/components/MessagesListView.qml \
    qml/components/MoreButton.qml \
    qml/components/NavigationBar.qml \
    qml/components/NavigationButton.qml \
    qml/components/NewMessageItem.qml \
    qml/components/NotificationsListView.qml \
    qml/components/PosterHeaderItem.qml \
    qml/components/RatingBox.qml \
    qml/components/RatingComponent.qml \
    qml/components/ReviewListItem.qml \
    qml/components/ShortBookRow.qml \
    qml/components/TagDelegate.qml \
    qml/components/UpdateListItem.qml \
    qml/components/UserShortReview.qml \
    qml/components/UserStatusRow.qml \
    qml/components/VotingBox.qml \
    qml/cover/CoverPage.qml \
    qml/dialogs/AddCommentDialog.qml \
    qml/dialogs/AddEditReviewDialog.qml \
    qml/dialogs/AddEditShelfDialog.qml \
    qml/dialogs/AddGroupFolderTopicDialog.qml \
    qml/dialogs/BooksReviewsSortingDialog.qml \
    qml/dialogs/UpdatesFilterDialog.qml \
    qml/pages/AboutPage.qml \
    qml/pages/AddBookToShelvesPage.qml \
    qml/pages/AuthorBooksPage.qml \
    qml/pages/AuthorPage.qml \
    qml/pages/AuthorSeriesPage.qml \
    qml/pages/AuthorizationPage.qml \
    qml/pages/BookEditionsPage.qml \
    qml/pages/BookPage.qml \
    qml/pages/BookReviewsPage.qml \
    qml/pages/BookShelvesPage.qml \
    qml/pages/BooksPage.qml \
    qml/pages/FriendsPage.qml \
    qml/pages/FriendsUpdatesPage.qml \
    qml/pages/GroupFolderPage.qml \
    qml/pages/GroupFolderTopicPage.qml \
    qml/pages/GroupMembersPage.qml \
    qml/pages/GroupPage.qml \
    qml/pages/GroupSearchPage.qml \
    qml/pages/GroupsPage.qml \
    qml/pages/InboxPage.qml \
    qml/pages/LoginPage.qml \
    qml/pages/MessagePage.qml \
    qml/pages/MessagesFolderSelectorPage.qml \
    qml/pages/MessagesPage.qml \
    qml/pages/NotificationsPage.qml \
    qml/pages/ProfilePage.qml \
    qml/pages/ReadStatusPage.qml \
    qml/pages/RecommendationPage.qml \
    qml/pages/ReviewPage.qml \
    qml/pages/SearchBookReviewPage.qml \
    qml/pages/SearchPage.qml \
    qml/pages/SeriesPage.qml \
    qml/pages/SettingsPage.qml \
    qml/pages/SplashScreenPage.qml \
    qml/pages/StatusPage.qml \
    qml/pages/UserQuotesPage.qml \
    qml/pages/UserReviewsPage.qml \
    qml/pages/UserStatusPage.qml \
    qml/pages/WebAuthorizationPage.qml \
    qml/pages/WebViewPage.qml \
}
