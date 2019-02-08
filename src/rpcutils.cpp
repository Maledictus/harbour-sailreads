/*
The MIT License(MIT)

Copyright (c) 2018-2019 Oleg Linkin <maledictusdemagog@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "rpcutils.h"

#include <algorithm>

#include <QRegularExpression>
#include <QtDebug>

#include "objects/update/commentupdate.h"
#include "objects/update/readstatusupdate.h"
#include "objects/update/reviewupdate.h"
#include "objects/update/userstatusupdate.h"
#include "objects/author.h"
#include "objects/book.h"
#include "objects/group.h"
#include "objects/message.h"
#include "objects/notification.h"
#include "objects/readstatus.h"
#include "objects/review.h"
#include "objects/series.h"
#include "objects/serieswork.h"
#include "objects/topic.h"
#include "objects/update.h"
#include "objects/user.h"
#include "objects/userstatus.h"
#include "objects/work.h"
#include "objects/recommendation.h"

namespace Sailreads
{
namespace RpcUtils
{
namespace Parser
{
namespace
{
QString PrepareDateTimeString(const QString& str)
{
    auto sections = str.split(" ");
    if (sections.count() != 6) {
        return str;
    }
    QString sIso = QString("%1-%2-%3T%4%5")
            .arg(sections[5])
            .arg(QLocale(QLocale::English).toDateTime(sections[1], "MMM").date().month(), 2, 10, QChar('0'))
            .arg(sections[2].toInt(), 2, 10, QChar('0'))
            .arg(sections[3])
            .arg(sections[4]);
    return sIso;
}
}

UserPtr ParseUser(const QDomElement& element)
{
    auto user = std::make_shared<User>();
    const auto& fieldsList = element.childNodes();
    for (int i = 0, count = fieldsList.size(); i < count; ++i) {
        const auto& fieldElement = fieldsList.at(i).toElement();
        if (fieldElement.tagName() == "id") {
            user->SetId(fieldElement.text());
        }
        else if (fieldElement.tagName() == "name" || fieldElement.tagName() == "display_name") {
            user->SetUserName(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "first_name") {
            user->SetFirstName(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "last_name") {
            user->SetLastName(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "user_name") {
            user->SetNickName(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "link") {
            user->SetWebUrl(QUrl(fieldElement.text()));
        }
        else if (fieldElement.tagName() == "image_url" || fieldElement.tagName() == "p2_image_url") {
            user->SetAvatarUrl(QUrl(fieldElement.text()));
        }
        else if (fieldElement.tagName() == "small_image_url") {
            user->SetSmallAvatarUrl(QUrl(fieldElement.text()));
        }
        else if (fieldElement.tagName() == "is_following") {
            user->SetIsFollowing(fieldElement.text() == "true");
        }
        else if (fieldElement.tagName() == "is_follower") {
            user->SetIsFollower(fieldElement.text() == "true");
        }
        else if (fieldElement.tagName() == "about") {
            user->SetAbout(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "age") {
            user->SetAge(fieldElement.text().toUInt());
        }
        else if (fieldElement.tagName() == "gender") {
            user->SetGender(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "location") {
            user->SetLocation(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "website") {
            user->SetWebSite(QUrl(fieldElement.text()));
        }
        else if (fieldElement.tagName() == "joined") {
            user->SetJoinedDate(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "last_active") {
            user->SetLastUpdateDate(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "interests") {
            user->SetInterests(fieldElement.text ());
        }
        else if (fieldElement.tagName() == "favorite_books") {
            user->SetFavoriteBooksDesc(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "favorite_authors") {
            user->SetFavoriteAuthors(ParseFavoriteAuthors(fieldElement));
        }
        else if (fieldElement.tagName() == "friend") {
            user->SetIsFriend(fieldElement.text() == "true");
        }
        else if (fieldElement.tagName() == "friend_status") {
            user->SetFriendStatus(fieldElement.text());
        }
        else if (fieldElement.tagName() == "friend_request") {
            user->SetFriendRequest(ParseFriendRequest(fieldElement));
        }
        else if (fieldElement.tagName() == "updates_rss_url") {
            user->SetUpdateRSSUrl(QUrl(fieldElement.text()));
        }
        else if (fieldElement.tagName() == "reviews_rss_url") {
            user->SetReviewRSSUrl(QUrl(fieldElement.text()));
        }
        else if (fieldElement.tagName() == "friends_count") {
            user->SetFriendsCount(fieldElement.text().toUInt());
        }
        else if (fieldElement.tagName() == "groups_count") {
            user->SetGroupsCount(fieldElement.text().toUInt());
        }
        else if (fieldElement.tagName() == "reviews_count") {
            user->SetBooksCount(fieldElement.text().toUInt());
        }
        else if (fieldElement.tagName() == "user_shelves") {
            user->SetBookShelves(ParseBookShelves(fieldElement));
        }
        else if (fieldElement.tagName() == "private") {
            user->SetPrivate(fieldElement.text() == "true");
        }
        else if (fieldElement.tagName() == "updates") {
            user->SetRecentUpdates(ParseUpdatesList(fieldElement));
        }
        else if (fieldElement.tagName() == "user_statuses") {
            // No any sense to implement this
        }
     }
    return user;
}

BookShelf ParseBookShelf(const QDomElement& element)
{
    BookShelf shelf;
    const auto& fieldsList = element.childNodes();
    for (int i = 0, fieldsCount = fieldsList.size(); i < fieldsCount; ++i) {
        const auto& fieldElement = fieldsList.at (i).toElement ();
        if (fieldElement.tagName() == "id") {
            shelf.SetId(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "name") {
            shelf.SetName(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "book_count") {
            shelf.SetBooksCount(fieldElement.text().toUInt());
        }
        else if (fieldElement.tagName() == "exclusive_flag") {
            shelf.SetExclusive(fieldElement.text() == "true");
        }
        else if (fieldElement.tagName() == "description") {
            shelf.SetDescription(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "sortable_flag") {
            shelf.SetSortable(fieldElement.text() == "true");
        }
        else if (fieldElement.tagName() == "featured") {
            shelf.SetFeatured(fieldElement.text() == "true");
        }
        else if (fieldElement.tagName() == "recommend_for") {
            shelf.SetRecommendFor(fieldElement.text() == "true");
        }
    }

    return shelf;
}

FavoriteAuthors_t ParseFavoriteAuthors(const QDomElement& faElement)
{
    FavoriteAuthors_t auths;
    const auto& nodes = faElement.childNodes();
    for (int i = 0, count = nodes.size(); i < count; ++i) {
        const auto& elem = nodes.at(i).toElement();
        const auto& ids = elem.elementsByTagName("id");
        const auto& names = elem.elementsByTagName("name");
        if (!ids.isEmpty() && !names.isEmpty()) {
            auths.push_back({ ids.at(0).toElement().text().toULongLong(),
                    names.at(0).toElement().text() });
        }
    }
    return auths;
}

GroupMember ParseGroupMember(const QDomElement& element)
{
    GroupMember gm;
    const auto& fieldsList = element.childNodes();
    for (int i = 0, fieldsCount = fieldsList.size(); i < fieldsCount; ++i) {
        const auto& fieldElement = fieldsList.at (i).toElement ();
        if (fieldElement.tagName() == "comments_count") {
            gm.SetCommentsCount(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "title") {
            gm.SetTitle(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "user") {
            gm.SetUser(ParseUser(fieldElement));
        }
    }

    return gm;
}

GroupFolder ParseGroupFolder(const QDomElement& element)
{
    GroupFolder gf;
    const auto& fieldsList = element.childNodes();
    for (int i = 0, fieldsCount = fieldsList.size(); i < fieldsCount; ++i) {
        const auto& fieldElement = fieldsList.at (i).toElement ();
        if (fieldElement.tagName() == "id") {
            gf.SetId(fieldElement.text());
        }
        else if (fieldElement.tagName() == "name") {
            gf.SetName(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "items_count") {
            gf.SetItemsCount(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "sub_items_count") {
            gf.SetSubItemsCount(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "updated_at" && !fieldElement.text().isEmpty()) {
            gf.SetUpdatedAt(QDateTime::fromString(PrepareDateTimeString(fieldElement.text()),
                    Qt::ISODate));
        }
    }

    return gf;
}

Group::GroupAccess GetGroupAccessFromString(const QString& accessStr)
{
    if (accessStr.toLower() == "public") {
        return Group::Public;
    }
    else if (accessStr.toLower() == "restricted") {
        return Group::Restricted;
    }
    else if (accessStr.toLower() == "private") {
        return Group::Private;
    }
    else if (accessStr.toLower() == "secret") {
        return Group::Secret;
    }
    else {
        return Group::Public;
    }
}

GroupPtr ParseGroup(const QDomElement& element)
{
    GroupPtr group = std::make_shared<Group>();
    const auto& fieldsList = element.childNodes();
    for (int i = 0, fieldsCount = fieldsList.size(); i < fieldsCount; ++i) {
        const auto& fieldElement = fieldsList.at (i).toElement ();
        if (fieldElement.tagName() == "id") {
            group->SetId(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "title") {
            group->SetName(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "access") {
            group->SetGroupAccess(GetGroupAccessFromString(fieldElement.text()));
        }
        else if (fieldElement.tagName() == "users_count" ||
                fieldElement.tagName() == "group_users_count") {
            group->SetUsersCount(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "image_url" || fieldElement.tagName() == "p1_image_url") {
            group->SetImageUrl(QUrl(fieldElement.text()));
        }
        else if (fieldElement.tagName() == "last_activity_at" && !fieldElement.text().isEmpty()) {
            group->SetLastActivity(QDateTime::fromString(PrepareDateTimeString(fieldElement.text()),
                    Qt::ISODate));
        }
        else if (fieldElement.tagName() == "link") {
            group->SetUrl(QUrl(fieldElement.text()));
        }
        else if (fieldElement.tagName() == "is_member") {
            group->SetIsMember(fieldElement.text() == "true");
        }
        else if (fieldElement.tagName() == "description") {
            group->SetDescription(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "location") {
            group->SetLocation(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "display_folder_count") {
            group->SetDisplayFolderCount(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "display_topics_per_folder") {
            group->SetDisplayTopicsPerFolderCount(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "bookshelves_public_flag") {
            group->SetBookshelvesPublicFlag(fieldElement.text() == "true");
        }
        else if (fieldElement.tagName() == "add_book_flag") {
            group->SetAddBookFlag(fieldElement.text() == "true");
        }
        else if (fieldElement.tagName() == "add_events_flag") {
            group->SetAddEventsFlag(fieldElement.text() == "true");
        }
        else if (fieldElement.tagName() == "polls_flag") {
            group->SetPollsFlag(fieldElement.text() == "true");
        }
        else if (fieldElement.tagName() == "discussion_public_flag") {
            group->SetDiscussionPublicFlag(fieldElement.text() == "true");
        }
        else if (fieldElement.tagName() == "real_world_flag") {
            group->SetRealWorldFlag(fieldElement.text() == "true");
        }
        else if (fieldElement.tagName() == "accepting_new_members_flag") {
            group->SetAcceptingNewMemberFlag(fieldElement.text() == "true");
        }
        else if (fieldElement.tagName() == "category") {
            group->SetCategory(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "subcategory") {
            group->SetSubCategory(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "rules") {
            group->SetRules(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "folders") {
            group->SetGroupFolders(ParseGroupFolders(fieldElement));
        }
        else if (fieldElement.tagName() == "currently_reading") {
            //TODO currently reading group books
        }
        else if (fieldElement.tagName() == "moderators") {
            group->SetGroupModerators(ParseGroupMembers(fieldElement));
        }
        else if (fieldElement.tagName() == "members") {
            group->SetGroupMembers(ParseGroupMembers(fieldElement));
        }
    }

    return group;
}

Friend ParseFriend(const QDomElement& element)
{
    Friend fr;
    const auto& fieldsList = element.childNodes();
    for (int i = 0, fieldsCount = fieldsList.size(); i < fieldsCount; ++i) {
        const auto& fieldElement = fieldsList.at (i).toElement ();
        if (fieldElement.tagName() == "id") {
            fr.SetId(fieldElement.text());
        }
        else if (fieldElement.tagName() == "name") {
            fr.SetName(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "link") {
            fr.SetUrl(QUrl(fieldElement.text()));
        }
        else if (fieldElement.tagName() == "image_url") {
            fr.SetAvatarUrl(QUrl(fieldElement.text()));
        }
        else if (fieldElement.tagName() == "small_image_url") {
            fr.SetSmallAvatarUrl(QUrl(fieldElement.text()));
        }
        else if (fieldElement.tagName() == "friends_count") {
            fr.SetFriendsCount(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "reviews_count") {
            fr.SetBooksCount(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "created_at" && !fieldElement.text().isEmpty()) {
            fr.SetCreatedDate(QDateTime::fromString(PrepareDateTimeString(fieldElement.text()),
                    Qt::ISODate));
        }
        else if (fieldElement.tagName() == "is_mutual_friend") {
            fr.SetIsMutualFriend(true);
        }
    }

    return fr;
}

FriendRequest ParseFriendRequest(const QDomElement& element)
{
    FriendRequest fr;
    const auto& fieldsList = element.childNodes();
    for (int i = 0, fieldsCount = fieldsList.size(); i < fieldsCount; ++i) {
        const auto& fieldElement = fieldsList.at (i).toElement ();
        if (fieldElement.tagName() == "id") {
            fr.SetId(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "created_at" && !fieldElement.text().isEmpty()) {
            fr.SetCreatedAt(QDateTime::fromString(PrepareDateTimeString(fieldElement.text()),
                    Qt::ISODate));
        }
        else if (fieldElement.tagName() == "message") {
            fr.SetMessage(fieldElement.text());
        }
        else if (fieldElement.tagName() == "from_user") {
            fr.SetFromUser(ParseUser(fieldElement));
        }
    }

    return fr;
}

FriendRecommendation ParseFriendRecommendation(const QDomElement& element)
{
    FriendRecommendation fr;
    const auto& fieldsList = element.childNodes();
    for (int i = 0, fieldsCount = fieldsList.size(); i < fieldsCount; ++i) {
        const auto& fieldElement = fieldsList.at (i).toElement ();
        if (fieldElement.tagName() == "id") {
            fr.SetId(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "created_at" && !fieldElement.text().isEmpty()) {
            fr.SetCreateDate(QDateTime::fromString(PrepareDateTimeString(fieldElement.text()),
                    Qt::ISODate));
        }
        else if (fieldElement.tagName() == "recommender_user") {
            fr.SetRecommenderUser(ParseUser(fieldElement));
        }
        else if (fieldElement.tagName() == "recommended_user") {
            fr.SetRecommendedUser(ParseUser(fieldElement));
        }
    }

    return fr;
}

TopicPtr ParseTopic(const QDomElement& element)
{
    TopicPtr topic = std::make_shared<Topic>();
    const auto& fieldsList = element.childNodes();
    for (int i = 0, fieldsCount = fieldsList.size(); i < fieldsCount; ++i) {
        const auto& fieldElement = fieldsList.at (i).toElement ();
        if (fieldElement.tagName() == "id") {
            topic->SetId(fieldElement.text());
        }
        else if (fieldElement.tagName() == "title") {
            topic->SetTitle(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "comments_count") {
            topic->SetCommentsCount(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "last_comment_at" && !fieldElement.text().isEmpty()) {
            topic->SetLastCommentDate(QDateTime::fromString(PrepareDateTimeString(fieldElement.text()),
                    Qt::ISODate));
        }
        else if (fieldElement.tagName() == "context_type") {
            topic->SetContextType(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "context_id") {
            topic->SetContextId(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "unread_count") {
            topic->SetUnreadCount(fieldElement.text().toInt());
        }
        else if (fieldElement.tagName() == "author_user") {
            topic->SetAuthor(ParseUser(fieldElement));
        }
        else if (fieldElement.tagName() == "folder") {
            topic->SetGroupFolder(ParseGroupFolder(fieldElement));
        }
        else if (fieldElement.tagName() == "author_user_id") {
            UserPtr author = topic->GetAuthorPtr();
            if (!author) {
                author = std::make_shared<User>();
            }
            author->SetId(fieldElement.text());
            topic->SetAuthor(author);
        }
        else if (fieldElement.tagName() == "author_user_name") {
            UserPtr author = topic->GetAuthorPtr();
            if (!author) {
                author = std::make_shared<User>();
            }
            author->SetUserName(fieldElement.text().trimmed());
            topic->SetAuthor(author);
        }
        else if (fieldElement.tagName() == "new_comments_count") {
            topic->SetUnreadCount(fieldElement.text().toInt());
        }
        else if (fieldElement.tagName() == "comments_per_page") {
            topic->SetCommentsPerPage(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "updated_at" && !fieldElement.text().isEmpty()) {
            topic->SetUpdatedAt(QDateTime::fromString(PrepareDateTimeString(fieldElement.text()),
                    Qt::ISODate));
        }
        else if (fieldElement.tagName() == "subject_type") {
           topic->SetSubjectType(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "group") {
            topic->SetGroup(ParseGroup(fieldElement));
        }
        else if (fieldElement.tagName() == "comments") {
            topic->SetComments(ParseComments(fieldElement));
        }
    }

    return topic;
}

Comment ParseComment(const QDomElement& element)
{
    Comment comment;
    const auto& fieldsList = element.childNodes();
    for (int i = 0, fieldsCount = fieldsList.size(); i < fieldsCount; ++i) {
        const auto& fieldElement = fieldsList.at (i).toElement ();
        if (fieldElement.tagName() == "id") {
            comment.SetId(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "body") {
            comment.SetBody(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "user") {
            comment.SetAuthor(ParseUser(fieldElement));
        }
        else if (fieldElement.tagName() == "updated_at" && !fieldElement.text().isEmpty()) {
            comment.SetUpdateAtDate(QDateTime::fromString(PrepareDateTimeString(fieldElement.text()),
                    Qt::ISODate));
        }
        else if (fieldElement.tagName() == "can_delete") {
            comment.SetCanDelete(fieldElement.text() == "true");
        }
    }

    return comment;
}

BookPtr ParseBook(const QDomElement& element)
{
    BookPtr book = std::make_shared<Book>();
    const auto& fieldsList = element.childNodes();
    for (int i = 0, fieldsCount = fieldsList.size(); i < fieldsCount; ++i) {
        const auto& fieldElement = fieldsList.at (i).toElement ();
        if (fieldElement.tagName() == "id") {
            book->SetId(fieldElement.text());
        }
        else if (fieldElement.tagName() == "isbn") {
            book->SetISBN(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "isbn13") {
            book->SetISBN13(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "asin") {
            book->SetASIN(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "kindle_asin") {
            book->SetKindleASIN(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "marketplace_id") {
            book->SetMarketplaceId(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "country_code") {
            book->SetCountryCode(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "text_reviews_count") {
            book->SetTextReviewsCount(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "title") {
            book->SetTitle(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "title_without_series") {
            book->SetTitleWithoutSeries(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "image_url") {
            book->SetImageUrl(QUrl(fieldElement.text()));
        }
        else if (fieldElement.tagName() == "small_image_url") {
            book->SetSmallImageUrl(QUrl(fieldElement.text()));
        }
        else if (fieldElement.tagName() == "large_image_url") {
            book->SetLargeImageUrl(QUrl(fieldElement.text()));
        }
        else if (fieldElement.tagName() == "link") {
            book->SetLink(QUrl(fieldElement.text()));
        }
        else if (fieldElement.tagName() == "num_pages") {
            book->SetNumPages(fieldElement.text().toInt());
        }
        else if (fieldElement.tagName() == "format") {
            book->SetFormat(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "edition_information") {
            book->SetEditionInformation(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "publisher") {
            book->SetPublisher(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "publication_day") {
            book->SetPublicationDay(fieldElement.text().toInt());
        }
        else if (fieldElement.tagName() == "publication_year") {
            book->SetPublicationYear(fieldElement.text().toInt());
        }
        else if (fieldElement.tagName() == "publication_month") {
            book->SetPublicationMonth(fieldElement.text().toInt());
        }
        else if (fieldElement.tagName() == "average_rating") {
            book->SetAverageRating(fieldElement.text().toDouble());
        }
        else if (fieldElement.tagName() == "ratings_count") {
            book->SetRatingsCount(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "description") {
            book->SetDescription(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "published") {
            book->SetPublishedYear(fieldElement.text().toInt());
        }
        else if (fieldElement.tagName() == "authors") {
            book->SetAuthors(ParseAuthors(fieldElement));
        }
        else if (fieldElement.tagName() == "author") {
            book->SetAuthors({ ParseAuthor(fieldElement) });
        }
        else if (fieldElement.tagName() == "work") {
            book->SetWork(ParseWork(fieldElement));
        }
        else if (fieldElement.tagName() == "language") {
            book->SetLanguage(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "is_ebook") {
            book->SetIsEBook(fieldElement.text() == "true");
        }
        else if (fieldElement.tagName() == "reviews_widget") {
            book->SetReviewsWidgetContent(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "similar_books") {
            book->SetSimilarBooks(ParseBooksList(fieldElement));
        }
        else if (fieldElement.tagName() == "series_works") {
            book->SetSeriesList(ParseSeriesFromSeriesWorks(fieldElement));
        }
        else if (fieldElement.tagName() == "my_review") {
            book->SetReview(ParseReview(fieldElement));
        }
        else if (fieldElement.tagName() == "friend_reviews") {
            book->SetFriendReviews(ParseReviewsList(fieldElement));
        }
    }

    return book;
}

ReviewPtr ParseReview(const QDomElement& element)
{
    ReviewPtr review = std::make_shared<Review>();
    BookPtr book = std::make_shared<Book>();
    const auto& fieldsList = element.childNodes();
    for (int i = 0, fieldsCount = fieldsList.size(); i < fieldsCount; ++i) {
        const auto& fieldElement = fieldsList.at (i).toElement ();
        if (fieldElement.tagName() == "id") {
            review->SetId(fieldElement.text());
        }
        else if (fieldElement.tagName() == "book") {
            book = ParseBook(fieldElement);
        }
        else if (fieldElement.tagName() == "book_id") {
            book->SetId(fieldElement.text());
        }
        else if (fieldElement.tagName() == "rating") {
            review->SetRating(fieldElement.text().toInt());
        }
        else if (fieldElement.tagName() == "votes") {
            review->SetLikesCount(fieldElement.text().toInt());
        }
        else if (fieldElement.tagName() == "spoiler_flag") {
            //TODO
        }
        else if (fieldElement.tagName() == "spoilers_state") {
            //TODO
        }
        else if (fieldElement.tagName() == "shelves") {
            BookShelves_t shelves;
            const auto& shelvesLits = fieldElement.childNodes();
            for (int i = 0, shelvesCount = shelvesLits.size(); i < shelvesCount; ++i) {
                const auto& shelfElement = shelvesLits.at (i).toElement ();
                BookShelf shelf;
                shelf.SetId(shelfElement.attribute("id").toULongLong());
                shelf.SetName(shelfElement.attribute("name"));
                shelves << shelf;
            }
            review->SetShelves(shelves);
        }
        else if (fieldElement.tagName() == "recommended_for") {
            //TODO
        }
        else if (fieldElement.tagName() == "recommended_by") {
            //TODO
        }
        else if (fieldElement.tagName() == "started_at" && !fieldElement.text().isEmpty()) {
            review->SetStartedDate(QDateTime::fromString(PrepareDateTimeString(fieldElement.text()),
                    Qt::ISODate));
        }
        else if (fieldElement.tagName() == "read_at" && !fieldElement.text().isEmpty()) {
            review->SetReadDate(QDateTime::fromString(PrepareDateTimeString(fieldElement.text()),
                Qt::ISODate));
        }
        else if (fieldElement.tagName() == "date_added" && !fieldElement.text().isEmpty()) {
            review->SetAddedDate(QDateTime::fromString(PrepareDateTimeString(fieldElement.text()),
                    Qt::ISODate));
        }
        else if (fieldElement.tagName() == "date_updated" && !fieldElement.text().isEmpty()) {
            review->SetUpdatedDate(QDateTime::fromString(PrepareDateTimeString(fieldElement.text()),
                    Qt::ISODate));
        }
        else if (fieldElement.tagName() == "date_read" && !fieldElement.text().isEmpty()) {
            review->SetReadDate(QDateTime::fromString(PrepareDateTimeString(fieldElement.text()),
                    Qt::ISODate));
        }
        else if (fieldElement.tagName() == "read_count") {
            review->SetReadCount(fieldElement.text().toInt());
        }
        else if (fieldElement.tagName() == "body") {
            review->SetBody(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "comments_count") {
            review->SetCommentsCount(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "url") {
            review->SetUrl(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "owned") {
            review->SetOwned(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "user") {
            review->SetUser(ParseUser(fieldElement));
        }
        else if (fieldElement.tagName() == "comments") {
            review->SetComments(ParseComments(fieldElement));
        }
    }

    review->SetBook(book);

    return review;
}

ReviewPtr ParseReviewFromShelf(const QDomElement& element)
{
    ReviewPtr review = std::make_shared<Review>();
    BookShelf shelf;
    const auto& fieldsList = element.childNodes();
    for (int i = 0, fieldsCount = fieldsList.size(); i < fieldsCount; ++i) {
        const auto& fieldElement = fieldsList.at(i).toElement();
        if (fieldElement.tagName() == "review-id") {
            review->SetId(fieldElement.text());
        }
        else if (fieldElement.tagName() == "updated-at") {
            review->SetUpdatedDate(QDateTime::fromString(fieldElement.text(), Qt::ISODate));
        }
        else if (fieldElement.tagName() == "created-at") {
            review->SetAddedDate(QDateTime::fromString(fieldElement.text(), Qt::ISODate));
        }
        else if (fieldElement.tagName() == "user-shelf-id") {
            shelf.SetId(fieldElement.text().toULong());
        }
        else if (fieldElement.tagName() == "name") {
            shelf.SetName(fieldElement.text());
        }
        else if (fieldElement.tagName() == "exclusive") {
            shelf.SetExclusive(fieldElement.text() == "true");
        }
    }

    review->SetShelves({ shelf });
    return review;
}

ReviewInfo ParseReviewInfo(const QDomElement& element)
{
    ReviewInfo info;
    const auto& fieldsList = element.childNodes();
    for (int i = 0, fieldsCount = fieldsList.size(); i < fieldsCount; ++i) {
        const auto& fieldElement = fieldsList.at (i).toElement ();
        if (fieldElement.tagName() == "id") {
            info.m_ReviewId = fieldElement.text();
        }
        else if (fieldElement.tagName() == "user-id") {
            info.m_UserId = fieldElement.text();
        }
        else if (fieldElement.tagName() == "book-id") {
            info.m_BookId = fieldElement.text();
        }
        else if (fieldElement.tagName() == "rating") {
            info.m_Rating = fieldElement.text().toInt();
        }
        else if (fieldElement.tagName() == "review") {
            info.m_Review = fieldElement.text();
        }
        else if (fieldElement.tagName() == "read-at") {
            info.m_ReadDate = QDateTime::fromString(fieldElement.text(), Qt::ISODate);
        }
        else if (fieldElement.tagName() == "updated-at") {
            info.m_UpdateDate = QDateTime::fromString(fieldElement.text(), Qt::ISODate);
        }
        else if (fieldElement.tagName() == "created-at") {
            info.m_CreateDate = QDateTime::fromString(fieldElement.text(), Qt::ISODate);
        }
        else if (fieldElement.tagName() == "started-at") {
            info.m_StartedDate = QDateTime::fromString(fieldElement.text(), Qt::ISODate);
        }
        else if (fieldElement.tagName() == "work-id") {
            info.m_WorkId = fieldElement.text().toULongLong();
        }
        else if (fieldElement.tagName() == "read-count") {
            info.m_ReadCount = fieldElement.text().toInt();
        }
    }

    return info;
}

AuthorPtr ParseAuthor(const QDomElement& element)
{
    AuthorPtr author = std::make_shared<Author>();
    const auto& fieldsList = element.childNodes();
    for (int i = 0, fieldsCount = fieldsList.size(); i < fieldsCount; ++i) {
        const auto& fieldElement = fieldsList.at (i).toElement ();
        if (fieldElement.tagName() == "id") {
            author->SetId(fieldElement.text());
        }
        else if (fieldElement.tagName() == "name") {
            author->SetName(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "role") {
            //TODO
        }
        else if (fieldElement.tagName() == "large_image_url") {
            author->SetLargeImageUrl(QUrl(fieldElement.text()));
        }
        else if (fieldElement.tagName() == "image_url") {
            author->SetImageUrl(QUrl(fieldElement.text()));
        }
        else if (fieldElement.tagName() == "small_image_url") {
            author->SetSmallImageUrl(QUrl(fieldElement.text()));
        }
        else if (fieldElement.tagName() == "link") {
            author->SetLink(QUrl(fieldElement.text()));
        }
        else if (fieldElement.tagName() == "average_rating") {
            author->SetAverageRating(fieldElement.text().toDouble());
        }
        else if (fieldElement.tagName() == "ratings_count") {
            author->SetRatingsCount(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "text_reviews_count") {
            author->SetTextReviewsCount(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "fans_count") {
            author->SetFansCount(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "author_followers_count") {
            author->SetFollowersCount(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "about") {
            author->SetAbout(fieldElement.text());
        }
        else if (fieldElement.tagName() == "influences") {
            author->SetInfluences(fieldElement.text());
        }
        else if (fieldElement.tagName() == "works_count") {
            author->SetWorksCount(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "gender") {
            author->SetGender(fieldElement.text());
        }
        else if (fieldElement.tagName() == "hometown") {
            author->SetHomeTown(fieldElement.text());
        }
        else if (fieldElement.tagName() == "born_at") {
            author->SetBirthDate(QDateTime::fromString(fieldElement.text(), "yyyy/MM/dd"));
        }
        else if (fieldElement.tagName() == "died_at") {
            author->SetDeathDate(QDateTime::fromString(fieldElement.text(), "yyyy/MM/dd"));
        }
        else if (fieldElement.tagName() == "goodreads_author") {
            author->SetIsGoodreadsAuthor(fieldElement.text() == "true");
        }
        else if (fieldElement.tagName() == "user") {

            QDomElement idElem = fieldElement.firstChildElement("id");
            if (!idElem.isNull()) {
                author->SetUserId(idElem.text());
            }
        }
        else if (fieldElement.tagName() == "books") {
            author->SetBooks(ParseBooksList(fieldElement));
        }
        else if (fieldElement.tagName() == "author_following_id") {
            author->SetFollowingId(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "fanship_id") {
            author->SetFollowingId(fieldElement.text().toULongLong());
        }
    }

    return author;
}

WorkPtr ParseWork(const QDomElement& element)
{
    WorkPtr work = std::make_shared<Work>();
    const auto& fieldsList = element.childNodes();
    for (int i = 0, fieldsCount = fieldsList.size(); i < fieldsCount; ++i) {
        const auto& fieldElement = fieldsList.at (i).toElement ();
        if (fieldElement.tagName() == "id") {
            work->SetId(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "books_count") {
            work->SetBooksCount(fieldElement.text().toInt());
        }
        else if (fieldElement.tagName() == "best_book_id") {
            work->SetBestBooksId(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "reviews_count") {
            work->SetReviewsCount(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "ratings_sum") {
            work->SetRatingSum(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "ratings_count") {
            work->SetRatingsCount(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "text_reviews_count") {
            work->SetTextReviewsCount(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "original_publication_year") {
            work->SetOriginalPublicationYear(fieldElement.text().toInt());
        }
        else if (fieldElement.tagName() == "original_publication_month") {
            work->SetOriginalPublicationMonth(fieldElement.text().toInt());
        }
        else if (fieldElement.tagName() == "original_publication_day") {
            work->SetOriginalPublicationDay(fieldElement.text().toInt());
        }
        else if (fieldElement.tagName() == "original_title") {
            work->SetOriginalTitle(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "rating_dist") {
            work->SetRatingDist(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "best_book") {
            work->SetBestBook(ParseBook(fieldElement));
        }
    }

    return work;
}

SeriesPtr ParseSeries(const QDomElement& element)
{
    SeriesPtr series = std::make_shared<Series>();
    const auto& fieldsList = element.childNodes();
    for (int i = 0, fieldsCount = fieldsList.size(); i < fieldsCount; ++i) {
        const auto& fieldElement = fieldsList.at (i).toElement ();
        if (fieldElement.tagName() == "id") {
            series->SetId(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "title") {
            series->SetTitle(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "description") {
            series->SetDescription(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "note") {
            series->SetNote(fieldElement.text().trimmed());
        }
        else if (fieldElement.tagName() == "series_works_count") {
            series->SetSeriesWorksCount(fieldElement.text().toInt());
        }
        else if (fieldElement.tagName() == "primary_work_count") {
            series->SetPrimaryWorkCount(fieldElement.text().toInt());
        }
        else if (fieldElement.tagName() == "numbered") {
            series->SetNumbered(fieldElement.text() == "true");
        }
        else if (fieldElement.tagName() == "series_works") {
            series->SetSeriesWorks(ParseSeriesWorks(fieldElement));
        }
    }

    return series;
}

SeriesWorkPtr ParseSeriesWork(const QDomElement& element)
{
    SeriesWorkPtr seriesWork = std::make_shared<SeriesWork>();
    const auto& fieldsList = element.childNodes();
    for (int i = 0, fieldsCount = fieldsList.size(); i < fieldsCount; ++i) {
        const auto& fieldElement = fieldsList.at (i).toElement ();
        if (fieldElement.tagName() == "id") {
            seriesWork->SetId(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "user_position") {
            seriesWork->SetPosition(fieldElement.text().toInt());
        }
        else if (fieldElement.tagName() == "work") {
            seriesWork->SetWork(ParseWork(fieldElement));
        }
        else if (fieldElement.tagName() == "series") {
            seriesWork->SetSeries(ParseSeries(fieldElement));
        }
    }

    return seriesWork;
}

MessagePtr ParseMessage(const QDomElement& element)
{
    MessagePtr message = std::make_shared<Message>();
    const auto& fieldsList = element.childNodes();
    for (int i = 0, fieldsCount = fieldsList.size(); i < fieldsCount; ++i) {
        const auto& fieldElement = fieldsList.at (i).toElement ();
        if (fieldElement.tagName() == "id") {
            message->SetId(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "created_at") {
            message->SetCreateDate(QDateTime::fromString(PrepareDateTimeString(fieldElement.text()),
                    Qt::ISODate));
        }
        else if (fieldElement.tagName() == "updated_at") {
            message->SetUpdateDate(QDateTime::fromString(PrepareDateTimeString(fieldElement.text()),
                    Qt::ISODate));
        }
        else if (fieldElement.tagName() == "read_at" && !fieldElement.text().isEmpty()) {
            message->SetReadDate(QDateTime::fromString(PrepareDateTimeString(fieldElement.text()),
                    Qt::ISODate));
        }
        else if (fieldElement.tagName() == "folder") {
            message->SetFolder(fieldElement.text());
        }
        else if (fieldElement.tagName() == "parent_message_id") {
            message->SetParentMessageId(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "subject") {
            message->SetSubject(fieldElement.text());
        }
        else if (fieldElement.tagName() == "body") {
            message->SetBody(fieldElement.text());
        }
        else if (fieldElement.tagName() == "from_user") {
            message->SetFromUser(ParseUser(fieldElement));
        }
        else if (fieldElement.tagName() == "to_user") {
            message->SetToUser(ParseUser(fieldElement));
        }
    }

    return message;
}

QString ParseResourceId(const QDomElement& element)
{
    const auto& resourceList = element.childNodes();
    for (int i = 0, fieldsCount = resourceList.size(); i < fieldsCount; ++i) {
        const auto& fieldElement = resourceList.at (i).toElement ();
        if (fieldElement.tagName() == "topic") {
            return ParseTopic(fieldElement)->GetId();
        }
        else if (fieldElement.tagName() == "review") {
            return ParseReview(fieldElement)->GetId();
        }
        else if (fieldElement.tagName() == "friend") {
            const auto& friendList = fieldElement.childNodes();
            for (int j = 0, frFieldsCount = friendList.size(); j < frFieldsCount; ++j) {
                const auto& friendFieldElement = friendList.at(j).toElement ();
                if (friendFieldElement.tagName() == "user_id") {
                    return friendFieldElement.text();
                }
            }
        }
    }
    return 0;
}

NotificationPtr ParseNotification(const QDomElement& element)
{
    NotificationPtr notification = std::make_shared<Notification>();
    const auto& fieldsList = element.childNodes();
    for (int i = 0, fieldsCount = fieldsList.size(); i < fieldsCount; ++i) {
        const auto& fieldElement = fieldsList.at (i).toElement ();
        if (fieldElement.tagName() == "actors") {
            notification->SetActors(ParseUsers(fieldElement));
        }
        else if (fieldElement.tagName() == "new") {
            notification->SetIsNew(fieldElement.text() == "true");
        }
        else if (fieldElement.tagName() == "created_at") {
            notification->SetCreateDate(QDateTime::fromString(PrepareDateTimeString(fieldElement.text()),
                    Qt::ISODate));
        }
        else if (fieldElement.tagName() == "body") {
            const auto& bodyList = fieldElement.childNodes();
            for (int i = 0, bodyListCount = bodyList.size(); i < bodyListCount; ++i) {
                const auto& bodyElement = bodyList.at (i).toElement ();
                if (bodyElement.tagName() == "html") {
                    notification->SetHtmlTextBody(bodyElement.text());
                }
                else if (bodyElement.tagName() == "text") {
                    notification->SetPlainTextBody(bodyElement.text());
                }
            }
        }
        else if (fieldElement.tagName() == "url") {
            notification->SetUrl(QUrl(fieldElement.text()));
        }
        else if (fieldElement.tagName() == "resource_type") {
            //Comment, DeletedComment, Rating, Poll, ChoiceAwardsResultsAnnouncement,
            //Rating, Friend, ChoiceAwardsFinalRoundAnnouncement, Comment, Challenge,
            //Rating, Giveaway, Recommendation
            notification->SetResourceType(fieldElement.text());
        }
        else if (fieldElement.tagName() == "group_resource_type") {
            //Topic, Review, Poll, ChoiceAwardsResultsAnnouncement,
            //Friend, Friend, ChoiceAwardsFinalRoundAnnouncement, Blog, Challenge,
            //UserChallenge, Giveaway, Recommendation
            notification->SetGroupResourceType(fieldElement.text());
        }
        else if (fieldElement.tagName() == "group_resource") {
            notification->SetResourceId(ParseResourceId(fieldElement));
        }
    }

    return notification;
}

ReadStatusUpdate* ParseReadStatusUpdate(const QDomElement& element)
{
    ReadStatusUpdate* rsu = new ReadStatusUpdate();
    const auto& fieldsList = element.childNodes();
    for (int i = 0, fieldsCount = fieldsList.size(); i < fieldsCount; ++i) {
        const auto& fieldElement = fieldsList.at (i).toElement ();
        if (fieldElement.tagName() == "id") {
            rsu->SetId(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "review_id") {
            rsu->SetReviewId(fieldElement.text());
        }
        else if (fieldElement.tagName() == "user_id") {
            rsu->SetUserId(fieldElement.text());
        }
        else if (fieldElement.tagName() == "old_status") {
            rsu->SetOldStatus(fieldElement.text());
        }
        else if (fieldElement.tagName() == "status") {
            rsu->SetCurrentStatus(fieldElement.text());
        }
        else if (fieldElement.tagName() == "updated_at") {
            rsu->SetUpdateDate(QDateTime::fromString(PrepareDateTimeString(fieldElement.text()),
                    Qt::ISODate));
        }
        else if (fieldElement.tagName() == "review") {
            const auto& bookElement = fieldElement.firstChildElement("book");
            if (!bookElement.isNull()) {
                BookPtr book = ParseBook(bookElement);
                rsu->SetBook(book);
            }
        }
    }
    return rsu;
}

UserStatusUpdate* ParseUserStatusUpdate(const QDomElement& element)
{
    UserStatusUpdate* usu = new UserStatusUpdate();
    const auto& fieldsList = element.childNodes();
    for (int i = 0, fieldsCount = fieldsList.size(); i < fieldsCount; ++i) {
        const auto& fieldElement = fieldsList.at (i).toElement ();
        if (fieldElement.tagName() == "id") {
            usu->SetId(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "user_id") {
            usu->SetUserId(fieldElement.text());
        }
        else if (fieldElement.tagName() == "book_id") {
            usu->SetBookId(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "page") {
            usu->SetPage(fieldElement.text().toInt());
        }
        else if (fieldElement.tagName() == "comments_count") {
            usu->SetCommentsCount(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "created_at") {
            usu->SetCreateDate(QDateTime::fromString(PrepareDateTimeString(fieldElement.text()),
                    Qt::ISODate));
        }
        else if (fieldElement.tagName() == "updated_at") {
            usu->SetUpdateDate(QDateTime::fromString(PrepareDateTimeString(fieldElement.text()),
                    Qt::ISODate));
        }
        else if (fieldElement.tagName() == "body") {
            usu->SetBody(fieldElement.text());
        }
        else if (fieldElement.tagName() == "percent") {
            usu->SetPercent(fieldElement.text().toInt());
        }
        else if (fieldElement.tagName() == "work_id") {
            usu->SetWorkId(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "review_id") {
            usu->SetReviewId(fieldElement.text());
        }
        else if (fieldElement.tagName() == "book") {
            usu->SetBook(ParseBook(fieldElement));
        }
    }
    return usu;
}


Update::UpdateType GetUpdateType(const QString& type)
{
    if (type == "comment") {
        return Update::Comment;
    }
    else if (type == "readstatus") {
        return Update::ReadStatus;
    }
    else if (type == "review") {
        return Update::Review;
    }
    else if (type == "userchallenge") {
        return Update::UserChallenge;
    }
    else if (type == "friend") {
        return Update::Friend;
    }
    else if (type == "userstatus") {
        return Update::UserStatus;
    }
    else {
        return Update::Unknown;
    }
}

UpdatePtr ParseUpdate(const QDomElement& element)
{
    UpdatePtr update = std::make_shared<Update>();
    update->SetUpdateType(GetUpdateType(element.attribute("type")));

    QString body;
    int rating = 0;
    BookPtr book;
    ReadStatusUpdate *rsu = nullptr;
    UserStatusUpdate *usu = nullptr;
    const auto& fieldsList = element.childNodes();
    for (int i = 0, fieldsCount = fieldsList.size(); i < fieldsCount; ++i) {
        const auto& fieldElement = fieldsList.at (i).toElement ();
        if (fieldElement.tagName() == "action_text") {
            update->SetActionText(fieldElement.text());
        }
        else if (fieldElement.tagName() == "link") {
            update->SetLink(QUrl(fieldElement.text()));
        }
        else if (fieldElement.tagName() == "image_url") {
            update->SetImageUrl(QUrl(fieldElement.text()));
        }
        else if (fieldElement.tagName() == "actor") {
            update->SetActor(ParseUser(fieldElement));
        }
        else if (fieldElement.tagName() == "updated_at") {
            update->SetUpdateDate(QDateTime::fromString(fieldElement.text(), Qt::RFC2822Date));
        }
        else if (fieldElement.tagName() == "object") {
            const auto& objectsList = fieldElement.childNodes();
            for (int j = 0, objectsCount = objectsList.size(); j < objectsCount; ++j) {
                const auto& objectElement = objectsList.at(j).toElement();
                if (objectElement.tagName() == "book") {
                    book = ParseBook(objectElement);
                }
                else if (objectElement.tagName() == "userstatus") {
                    usu = ParseUserStatusUpdate(objectElement);
                }
                else if (objectElement.tagName() == "read_status") {
                    rsu = ParseReadStatusUpdate(objectElement);
                }
            }
        }
        else if (fieldElement.tagName() == "action" && fieldElement.attribute("type") == "rating") {
            const auto& ratingElement = fieldElement.firstChildElement("rating");
            if (!ratingElement.isNull()) {
                rating = ratingElement.text().toInt();
            }
        }
        else if (fieldElement.tagName() == "body") {
            body = fieldElement.text();
        }
    }

    QObjectPtr updateObject;
    switch (update->GetUpdateType()) {
    case Update::Review: {
        ReviewUpdate *ru = new ReviewUpdate();
        ru->SetBody(body);
        ru->SetRating(rating);
        ru->SetBook(book);
        updateObject.reset(ru);
        break;
    }
    case Update::Comment: {
        CommentUpdate *cu = new CommentUpdate();
        cu->SetBody(body);
        updateObject.reset(cu);
        break;
    }
    case Update::ReadStatus: {
        if (rsu) {
            updateObject.reset(rsu);
        }
        break;
    }
    case Update::UserStatus: {
        if (usu) {
            updateObject.reset(usu);
        }
        break;
    }
    default:
        break;
    }

    update->SetUpdateObject(updateObject);

    return update;
}


ReadStatusPtr ParseReadStatus(const QDomElement& element)
{
    ReadStatusPtr rs = std::make_shared<ReadStatus>();
    const auto& fieldsList = element.childNodes();
    for (int i = 0, fieldsCount = fieldsList.size(); i < fieldsCount; ++i) {
        const auto& fieldElement = fieldsList.at (i).toElement ();
        if (fieldElement.tagName() == "id") {
            rs->SetId(fieldElement.text());
        }
        else if (fieldElement.tagName() == "header") {
            rs->SetHeader(fieldElement.text());
        }
        else if (fieldElement.tagName() == "likes_count") {
            rs->SetLikesCount(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "comments_count") {
            rs->SetCommentsCount(fieldElement.text().toInt());
        }
        else if (fieldElement.tagName() == "rating_id") {
            rs->SetRatingId(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "created_at") {
            rs->SetCreateDate(QDateTime::fromString(PrepareDateTimeString(fieldElement.text()),
                    Qt::ISODate));
        }
        else if (fieldElement.tagName() == "updated_at") {
            rs->SetUpdateDate(QDateTime::fromString(PrepareDateTimeString(fieldElement.text()),
                    Qt::ISODate));
        }
        else if (fieldElement.tagName() == "status") {
            rs->SetStatus(fieldElement.text());
        }
        else if (fieldElement.tagName() == "review_id") {
            rs->SetReviewId(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "book") {
            rs->SetBook(ParseBook(fieldElement));
        }
        else if (fieldElement.tagName() == "user") {
            rs->SetUser(ParseUser(fieldElement));
        }
        else if (fieldElement.tagName() == "comments") {
            rs->SetComments(ParseComments(fieldElement));
        }
    }
    return rs;
}

UserStatusPtr ParseUserStatus(const QDomElement& element)
{
    UserStatusPtr us = std::make_shared<UserStatus>();
    UserStatuses_t prevStatutes;
    const auto& fieldsList = element.childNodes();
    for (int i = 0, fieldsCount = fieldsList.size(); i < fieldsCount; ++i) {
        const auto& fieldElement = fieldsList.at (i).toElement ();
        if (fieldElement.tagName() == "id") {
            us->SetId(fieldElement.text());
        }
        else if (fieldElement.tagName() == "header") {
            us->SetHeader(fieldElement.text());
        }
        else if (fieldElement.tagName() == "likes_count") {
            us->SetLikesCount(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "comments_count") {
            us->SetCommentsCount(fieldElement.text().toInt());
        }
        else if (fieldElement.tagName() == "rating_id") {
            us->SetRatingId(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "created_at") {
            us->SetCreateDate(QDateTime::fromString(PrepareDateTimeString(fieldElement.text()),
                    Qt::ISODate));
        }
        else if (fieldElement.tagName() == "updated_at") {
            us->SetUpdateDate(QDateTime::fromString(PrepareDateTimeString(fieldElement.text()),
                    Qt::ISODate));
        }
        else if (fieldElement.tagName() == "page") {
            us->SetPage(fieldElement.text().toInt());
        }
        else if (fieldElement.tagName() == "percent") {
            us->SetPercent(fieldElement.text().toInt());
        }
        else if (fieldElement.tagName() == "work_id") {
            us->SetWorkId(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "book") {
            us->SetBook(ParseBook(fieldElement));
        }
        else if (fieldElement.tagName() == "user") {
            us->SetUser(ParseUser(fieldElement));
        }
        else if (fieldElement.tagName() == "comments") {
            us->SetComments(ParseComments(fieldElement));
        }
        else if (fieldElement.tagName() == "user_status") {
            prevStatutes << ParseUserStatus(fieldElement);
        }
    }

    us->SetPreviousStatuses(prevStatutes);
    return us;
}

RecommendationPtr ParseRecommendation(const QDomElement& element)
{
    RecommendationPtr recommendation = std::make_shared<Recommendation>();
    const auto& fieldsList = element.childNodes();
    for (int i = 0, fieldsCount = fieldsList.size(); i < fieldsCount; ++i) {
        const auto& fieldElement = fieldsList.at (i).toElement ();
        if (fieldElement.tagName() == "id") {
            recommendation->SetId(fieldElement.text());
        }
        else if (fieldElement.tagName() == "likes_count") {
            recommendation->SetLikesCount(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "created_at") {
            recommendation->SetCreateDate(QDateTime::fromString(fieldElement.text(), Qt::ISODate));
        }
        else if (fieldElement.tagName() == "rating_id") {
            recommendation->SetRatingId(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "book") {
            recommendation->SetBook(ParseBook(fieldElement));
        }
        else if (fieldElement.tagName() == "from_user") {
            recommendation->SetFromUser(ParseUser(fieldElement));
        }
        else if (fieldElement.tagName() == "to_user") {
            recommendation->SetToUser(ParseUser(fieldElement));
        }
        else if (fieldElement.tagName() == "comments") {
            recommendation->SetComments(ParseComments(fieldElement));
        }
    }
    return recommendation;
}

GroupMembers_t ParseGroupMembers(const QDomElement& element)
{
    GroupMembers_t members;
    const auto& nodes = element.childNodes();
    for (int i = 0, count = nodes.size(); i < count; ++i) {
        const auto& elem = nodes.at(i).toElement();
        members << ParseGroupMember(elem);
    }
    return members;
}

GroupFolders_t ParseGroupFolders(const QDomElement& element)
{
    GroupFolders_t folders;
    const auto& nodes = element.childNodes();
    for (int i = 0, count = nodes.size(); i < count; ++i) {
        const auto& elem = nodes.at(i).toElement();
        folders << ParseGroupFolder(elem);
    }
    return folders;
}

BookShelves_t ParseBookShelves(const QDomElement& element)
{
    BookShelves_t shelves;
    const auto& nodes = element.childNodes();
    for (int i = 0, count = nodes.size(); i < count; ++i) {
        const auto& elem = nodes.at(i).toElement();
        shelves << ParseBookShelf(elem);
    }
    return shelves;
}

Groups_t ParseGroups(const QDomElement& element)
{
    Groups_t groups;
    const auto& nodes = element.childNodes();
    for (int i = 0, count = nodes.size(); i < count; ++i) {
        const auto& elem = nodes.at(i).toElement();
        groups << ParseGroup(elem);
    }
    return groups;
}

Friends_t ParseFriends(const QDomElement& element)
{
    Friends_t friends;
    const auto& nodes = element.childNodes();
    for (int i = 0, count = nodes.size(); i < count; ++i) {
        const auto& elem = nodes.at(i).toElement();
        friends << ParseFriend(elem);
    }
    return friends;
}

FriendsRequests_t ParseFriendsRequests(const QDomElement& element)
{
    FriendsRequests_t requests;
    const auto& nodes = element.childNodes();
    for (int i = 0, count = nodes.size(); i < count; ++i) {
        const auto& elem = nodes.at(i).toElement();
        requests << ParseFriendRequest(elem);
    }
    return requests;
}

FriendsRecommendations_t ParseFriendsRecommendations(const QDomElement& element)
{
    FriendsRecommendations_t recommendations;
    const auto& nodes = element.childNodes();
    for (int i = 0, count = nodes.size(); i < count; ++i) {
        const auto& elem = nodes.at(i).toElement();
        recommendations << ParseFriendRecommendation(elem);
    }
    return recommendations;
}

Topics_t ParseTopics(const QDomElement& element)
{
    Topics_t topics;
    const auto& nodes = element.childNodes();
    for (int i = 0, count = nodes.size(); i < count; ++i) {
        const auto& elem = nodes.at(i).toElement();
        topics << ParseTopic(elem);
    }
    return topics;
}

Authors_t ParseAuthors(const QDomElement& element)
{
    Authors_t authors;
    const auto& nodes = element.childNodes();
    for (int i = 0, count = nodes.size(); i < count; ++i) {
        const auto& elem = nodes.at(i).toElement();
        authors << ParseAuthor(elem);
    }
    return authors;
}

CountedItems<Comment> ParseComments(const QDomElement& element)
{
    CountedItems<Comment> comments;
    comments.m_BeginIndex = element.attribute("start").toULongLong();
    comments.m_EndIndex = element.attribute("end").toULongLong();
    comments.m_Count = element.attribute("total").toULongLong();
    const auto& nodes = element.childNodes();
    for (int i = 0, count = nodes.size(); i < count; ++i) {
        const auto& elem = nodes.at(i).toElement();
        comments.m_Items << ParseComment(elem);
    }

//    std::reverse(comments.m_Items.begin(), comments.m_Items.end());

    return comments;
}

CountedItems<ReviewPtr> ParseReviews(const QDomElement& element)
{
    CountedItems<ReviewPtr> reviews;
    reviews.m_BeginIndex = element.attribute("start").toULongLong();
    reviews.m_EndIndex = element.attribute("end").toULongLong();
    reviews.m_Count = element.attribute("total").toULongLong();
    const auto& nodes = element.childNodes();
    for (int i = 0, count = nodes.size(); i < count; ++i) {
        const auto& elem = nodes.at(i).toElement();
        reviews.m_Items << ParseReview(elem);
    }
    return reviews;
}

SeriesWorks_t ParseSeriesWorks(const QDomElement& element)
{
    SeriesWorks_t seriesWorks;
    const auto& nodes = element.childNodes();
    for (int i = 0, count = nodes.size(); i < count; ++i) {
        const auto& elem = nodes.at(i).toElement();
        seriesWorks << ParseSeriesWork(elem);
    }
    return seriesWorks;
}

Series_t ParseSeriesFromSeriesWorks(const QDomElement& element)
{
    Series_t series;
    const auto& nodes = element.childNodes();
    for (int i = 0, count = nodes.size(); i < count; ++i) {
        const auto& elem = nodes.at(i).toElement();
        const auto& fieldsList = elem.childNodes();
        for (int i = 0, fieldsCount = fieldsList.size(); i < fieldsCount; ++i) {
            const auto& fieldElement = fieldsList.at (i).toElement ();
            if (fieldElement.tagName() == "series") {
                series << ParseSeries(fieldElement);
                break;
            }
        }
    }
    return series;
}

Books_t ParseBooksList(const QDomElement& element)
{
    const auto& booksList = element.childNodes();
    Books_t books;
    for (int i = 0, cnt = booksList.size(); i < cnt; ++i) {
        books << ParseBook(booksList.at (i).toElement());
    }
    return books;
}

CountedItems<BookPtr> ParseBooks(const QDomElement& element)
{
    CountedItems<BookPtr> books;
    books.m_BeginIndex = element.attribute("start").toULongLong();
    books.m_EndIndex = element.attribute("end").toULongLong();
    books.m_Count = element.attribute("total").toULongLong();
    books.m_Items = ParseBooksList(element);
    return books;
}

Reviews_t ParseReviewsList(const QDomElement& element)
{
    const auto& reviewsList = element.childNodes();
    Reviews_t reviews;
    for (int i = 0, cnt = reviewsList.size(); i < cnt; ++i) {
        reviews << ParseReview(reviewsList.at(i).toElement());
    }
    return reviews;
}

Series_t ParseAuthorSeries(const QDomElement& element)
{
    std::map<quint64, SeriesPtr> id2series;
    QHash<quint64, SeriesWorks_t> seriesId2seriesWorks;
    const auto& seriesWorksList = element.childNodes();
    for (int i = 0, cnt = seriesWorksList.size(); i < cnt; ++i) {
        const auto& seriesWorkElement = seriesWorksList.at(i).toElement();
        const auto& seriesWorkFields = seriesWorkElement.childNodes();
        SeriesWorkPtr seriesWork = std::make_shared<SeriesWork>();
        for (int j = 0, seriesWorkCnt = seriesWorkFields.size(); j < seriesWorkCnt; ++j) {
            const auto& fieldElement = seriesWorkFields.at(j).toElement();
            if (fieldElement.tagName() == "id") {
                seriesWork->SetId(fieldElement.text().toULongLong());
            }
            else if (fieldElement.tagName() == "user_position") {
                seriesWork->SetPosition(fieldElement.text().toInt());
            }
            else if (fieldElement.tagName() == "work") {
                seriesWork->SetWork(ParseWork(fieldElement));
            }
            else if (fieldElement.tagName() == "series") {
                SeriesPtr s = ParseSeries(fieldElement);
                id2series.insert(std::make_pair(s->GetId(), s));
                seriesId2seriesWorks[s->GetId()] << seriesWork;
            }
        }
    }

    Series_t series;
    for (auto pair : id2series) {
        if (seriesId2seriesWorks.contains(pair.first)) {
            pair.second->SetSeriesWorks(seriesId2seriesWorks[pair.first]);
        }
        series << pair.second;
    }

    return series;
}

Messages_t ParseMessagesList(const QDomElement& element)
{
    const auto& messagesList = element.childNodes();
    Messages_t result;
    for (int i = 0, cnt = messagesList.size(); i < cnt; ++i) {
        result << ParseMessage(messagesList.at (i).toElement());
    }
    return result;
}

CountedItems<MessagePtr> ParseMessages(const QDomElement& element)
{
    CountedItems<MessagePtr> messages;
    messages.m_BeginIndex = element.attribute("start").toULongLong();
    messages.m_EndIndex = element.attribute("end").toULongLong();
    messages.m_Count = element.attribute("total").toULongLong();
    messages.m_Items = ParseMessagesList(element);
    return messages;
}

Notifications_t ParseNotifications(const QDomElement& element)
{
    const auto& notificationsList = element.childNodes();
    Notifications_t result;
    for (int i = 0, cnt = notificationsList.size(); i < cnt; ++i) {
        result << ParseNotification(notificationsList.at (i).toElement());
    }
    return result;
}

Users_t ParseUsers(const QDomElement& element)
{
    const auto& usersList = element.childNodes();
    Users_t result;
    for (int i = 0, cnt = usersList.size(); i < cnt; ++i) {
        result << ParseUser(usersList.at (i).toElement());
    }
    return result;
}

Updates_t ParseUpdatesList(const QDomElement& element)
{
    const auto& updatesList = element.childNodes();
    Updates_t result;
    for (int i = 0, cnt = updatesList.size(); i < cnt; ++i) {
        result << ParseUpdate(updatesList.at (i).toElement());
    }
    return result;
}

UserPtr ParseUser(const QDomDocument& doc)
{
    const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
    if (responseElement.isNull()) {
        return UserPtr();
    }

    return ParseUser(responseElement.firstChildElement("user"));
}

CountedItems<BookShelf> ParseBookShelves(const QDomDocument& doc)
{
    const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
    if (responseElement.isNull()) {
        return CountedItems<BookShelf>();
    }

    const auto& shelvesElement = responseElement.firstChildElement("shelves");
    if (shelvesElement.isNull()) {
        return CountedItems<BookShelf>();
    }

    CountedItems<BookShelf> shelves;
    shelves.m_BeginIndex = shelvesElement.attribute("start").toULongLong();
    shelves.m_EndIndex = shelvesElement.attribute("end").toULongLong();
    shelves.m_Count = shelvesElement.attribute("total").toULongLong();
    shelves.m_Items = ParseBookShelves(shelvesElement);
    return shelves;
}

CountedItems<GroupPtr> ParseGroups(const QDomDocument& doc)
{
    const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
    if (responseElement.isNull()) {
        return CountedItems<GroupPtr>();
    }

    const auto& groupsElement = responseElement.firstChildElement("groups");
    if (groupsElement.isNull()) {
        return CountedItems<GroupPtr>();
    }

    const auto& groupsListElement = groupsElement.firstChildElement("list");
    if (groupsListElement.isNull()) {
        return CountedItems<GroupPtr>();
    }

    CountedItems<GroupPtr> groups;
    groups.m_BeginIndex = groupsListElement.attribute("start").toULongLong();
    groups.m_EndIndex = groupsListElement.attribute("end").toULongLong();
    groups.m_Count = groupsListElement.attribute("total").toULongLong();
    groups.m_Items = ParseGroups(groupsListElement);
    return groups;
}

CountedItems<Friend> ParseFriends(const QDomDocument& doc)
{
    const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
    if (responseElement.isNull()) {
        return CountedItems<Friend>();
    }
    const auto& friendsElement = responseElement.firstChildElement("friends");
    if (friendsElement.isNull()) {
        return CountedItems<Friend>();
    }

    CountedItems<Friend> friends;
    friends.m_BeginIndex = friendsElement.attribute("start").toULongLong();
    friends.m_EndIndex = friendsElement.attribute("end").toULongLong();
    friends.m_Count = friendsElement.attribute("total").toULongLong();
    friends.m_Items = ParseFriends(friendsElement);
    return friends;
}

GroupPtr ParseGroup(const QDomDocument& doc)
{
    const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
    if (responseElement.isNull()) {
        return GroupPtr();
    }

    return ParseGroup(responseElement.firstChildElement("group"));
}

GroupMembers_t ParseGroupMembers(const QDomDocument& doc)
{
    const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
    if (responseElement.isNull()) {
        return GroupMembers_t();
    }

    return ParseGroupMembers(responseElement.firstChildElement("group_users"));
}

CountedItems<TopicPtr> ParseGroupFolderTopics(const QDomDocument& doc)
{
    const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
    if (responseElement.isNull()) {
        return CountedItems<TopicPtr>();
    }

    const auto& groupFolderElement = responseElement.firstChildElement("group_folder");
    if (groupFolderElement.isNull()) {
        return CountedItems<TopicPtr>();
    }

    const auto& topicsListElement = groupFolderElement.firstChildElement("topics");
    if (topicsListElement.isNull()) {
        return CountedItems<TopicPtr>();
    }

    CountedItems<TopicPtr> topics;
    topics.m_BeginIndex = topicsListElement.attribute("start").toULongLong();
    topics.m_EndIndex = topicsListElement.attribute("end").toULongLong();
    topics.m_Count = topicsListElement.attribute("total").toULongLong();
    topics.m_Items = ParseTopics(topicsListElement);
    return topics;
}

TopicPtr ParseGroupFolderTopic(const QDomDocument& doc)
{
    const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
    if (responseElement.isNull()) {
        return TopicPtr();
    }

    return ParseTopic(responseElement.firstChildElement("topic"));
}

Comment ParseComment(const QDomDocument& doc)
{
    const auto& commentElement = doc.firstChildElement("comment");
    if (commentElement.isNull()) {
        return Comment();
    }

    return ParseComment(commentElement);
}

QPair<quint64, CountedItems<ReviewPtr>> ParseReviews(const QDomDocument& doc)
{
    const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
    if (responseElement.isNull()) {
        return QPair<quint64, CountedItems<ReviewPtr>>();
    }

    const auto& shelfElement = responseElement.firstChildElement("shelf");
    if (shelfElement.isNull()) {
        return QPair<quint64, CountedItems<ReviewPtr>>();
    }

    const quint64 shelfId = shelfElement.attribute("id").toULong();

    const auto& reviewsListElement = responseElement.firstChildElement("reviews");
    if (reviewsListElement.isNull()) {
        return qMakePair(shelfId, CountedItems<ReviewPtr>());
    }

    return qMakePair(shelfId, ParseReviews(reviewsListElement));
}

CountedItems<ReviewPtr> ParseFoundReviews(const QDomDocument& doc)
{
    const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
    if (responseElement.isNull()) {
        return CountedItems<ReviewPtr>();
    }

    const auto& reviewsListElement = responseElement.firstChildElement("reviews");
    if (reviewsListElement.isNull()) {
        return CountedItems<ReviewPtr>();
    }

    return ParseReviews(reviewsListElement);
}

BookPtr ParseBook(const QDomDocument& doc)
{
    const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
    if (responseElement.isNull()) {
        return BookPtr();
    }

    return ParseBook(responseElement.firstChildElement("book"));
}

SeriesPtr ParseSeries(const QDomDocument& doc)
{
    const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
    if (responseElement.isNull()) {
        return SeriesPtr();
    }

    return ParseSeries(responseElement.firstChildElement("series"));
}

CountedItems<Friend> ParseFollowings(const QDomDocument& doc)
{
    const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
    if (responseElement.isNull()) {
        return CountedItems<Friend>();
    }
    const auto& friendsElement = responseElement.firstChildElement("following");
    if (friendsElement.isNull()) {
        return CountedItems<Friend>();
    }

    CountedItems<Friend> friends;
    friends.m_BeginIndex = friendsElement.attribute("start").toULongLong();
    friends.m_EndIndex = friendsElement.attribute("end").toULongLong();
    friends.m_Count = friendsElement.attribute("total").toULongLong();
    friends.m_Items = ParseFriends(friendsElement);
    return friends;
}

CountedItems<Friend> ParseFollowers(const QDomDocument& doc)
{
    const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
    if (responseElement.isNull()) {
        return CountedItems<Friend>();
    }
    const auto& friendsElement = responseElement.firstChildElement("followers");
    if (friendsElement.isNull()) {
        return CountedItems<Friend>();
    }

    CountedItems<Friend> friends;
    friends.m_BeginIndex = friendsElement.attribute("start").toULongLong();
    friends.m_EndIndex = friendsElement.attribute("end").toULongLong();
    friends.m_Count = friendsElement.attribute("total").toULongLong();
    friends.m_Items = ParseFriends(friendsElement);
    return friends;
}

AuthorPtr ParseAuthor(const QDomDocument& doc)
{
    const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
    if (responseElement.isNull()) {
        return AuthorPtr();
    }

    return ParseAuthor(responseElement.firstChildElement("author"));
}

CountedItems<BookPtr> ParseAuthorBooks(const QDomDocument &doc)
{
    const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
    if (responseElement.isNull()) {
        return CountedItems<BookPtr>();
    }

    const auto& authorElement = responseElement.firstChildElement("author");
    if (authorElement.isNull()) {
        return CountedItems<BookPtr>();
    }

    const auto& booksElement = authorElement.firstChildElement("books");
    if (booksElement.isNull()) {
        return CountedItems<BookPtr>();
    }

    return ParseBooks(booksElement);
}

ReviewPtr ParseReview(const QDomDocument& doc)
{
    const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
    if (responseElement.isNull()) {
        return ReviewPtr();
    }

    return ParseReview(responseElement.firstChildElement("review"));
}

ReviewInfo ParseReviewInfo(const QDomDocument& doc)
{
    const auto& reviewElement = doc.firstChildElement("review");
    if (reviewElement.isNull()) {
        return ReviewInfo();
    }

    return ParseReviewInfo(reviewElement);
}

Series_t ParseAuthorSeries(const QDomDocument& doc)
{
    const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
    if (responseElement.isNull()) {
        return Series_t();
    }

    const auto& seriesWorksElement = responseElement.firstChildElement("series_works");
    if (seriesWorksElement.isNull()) {
        return Series_t();
    }

    return ParseAuthorSeries(seriesWorksElement);
}

CountedItems<BookPtr> ParseBookEditions(const QDomDocument &doc)
{
    const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
    if (responseElement.isNull()) {
        return CountedItems<BookPtr>();
    }

    const auto& editionsElement = responseElement.firstChildElement("editions");
    if (editionsElement.isNull()) {
        return CountedItems<BookPtr>();
    }

    const auto& booksElement = editionsElement.firstChildElement("books");
    if (booksElement.isNull()) {
        return CountedItems<BookPtr>();
    }

    return ParseBooks(booksElement);
}

CountedItems<MessagePtr> ParseMessages(const QDomDocument& doc)
{
    const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
    if (responseElement.isNull()) {
        return CountedItems<MessagePtr>();
    }

    const auto& messageFolderElement = responseElement.firstChildElement("message_folder");
    if (messageFolderElement.isNull()) {
        return CountedItems<MessagePtr>();
    }

    const auto& messagesElement = messageFolderElement.firstChildElement("messages");
    if (messagesElement.isNull()) {
        return CountedItems<MessagePtr>();
    }

    return ParseMessages(messagesElement);
}

MessagePtr ParseMessage(const QDomDocument& doc)
{
    MessagePtr message;
    const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
    if (responseElement.isNull()) {
        return message;
    }

    const auto& messageShowElement = responseElement.firstChildElement("message_show");
    if (messageShowElement.isNull()) {
        return message;
    }

    const auto& messageElement = messageShowElement.firstChildElement("message");
    if (messageElement.isNull()) {
        return message;
    }

    message = ParseMessage(messageElement);
    if (!message) {
        return message;
    }
    const auto& messageHistoryElement = messageShowElement.firstChildElement("message_history");
    if (messageHistoryElement.isNull()) {
        return message;
    }

    Messages_t messageHistory = ParseMessagesList(messageHistoryElement);
    message->SetMessageHistory(messageHistory);
    auto nextMessageElement = messageShowElement.firstChildElement("next_message");
    auto prevMessageElement = messageShowElement.firstChildElement("previous_message");
    if (!nextMessageElement.text().isEmpty()) {
        message->SetNextMessageId(nextMessageElement.text().toULongLong());
    }
    if (!prevMessageElement.text().isEmpty()) {
        message->SetPreviousMessageId(prevMessageElement.text().toULongLong());
    }
    return message;
}

Notifications_t ParseNotifications(const QDomDocument &doc)
{
    const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
    if (responseElement.isNull()) {
        return Notifications_t();
    }

    const auto& notificationsElement = responseElement.firstChildElement("notifications");
    if (notificationsElement.isNull()) {
        return Notifications_t();
    }

    return ParseNotifications(notificationsElement);
}

Updates_t ParseUpdates(const QDomDocument& doc)
{
    const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
    if (responseElement.isNull()) {
        return Updates_t();
    }

    const auto& updatesElement = responseElement.firstChildElement("updates");
    if (updatesElement.isNull()) {
        return Updates_t();
    }

    return ParseUpdatesList(updatesElement);
}

ReadStatusPtr ParseReadStatus(const QDomDocument &doc)
{
    const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
    if (responseElement.isNull()) {
        return ReadStatusPtr();
    }

    const auto& readStatusElement = responseElement.firstChildElement("read_status");
    if (readStatusElement.isNull()) {
        return ReadStatusPtr();
    }

    return ParseReadStatus(readStatusElement);
}

CountedItems<Comment> ParseComments(const QDomDocument& doc)
{
    const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
    if (responseElement.isNull()) {
        return CountedItems<Comment>();
    }

    const auto& commentsElement = responseElement.firstChildElement("comments");
    if (commentsElement.isNull()) {
        return CountedItems<Comment>();
    }

    return ParseComments(commentsElement);
}

UserStatusPtr ParseUserStatus(const QDomDocument& doc)
{
    const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
    if (responseElement.isNull()) {
        return UserStatusPtr();
    }

    const auto& userStatusElement = responseElement.firstChildElement("user_status");
    if (userStatusElement.isNull()) {
        return UserStatusPtr();
    }

    return ParseUserStatus(userStatusElement);
}

RecommendationPtr ParseRecommendation(const QDomDocument& doc)
{
    const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
    if (responseElement.isNull()) {
        return RecommendationPtr();
    }

    const auto& recommendationElement = responseElement.firstChildElement("recommendation");
    if (recommendationElement.isNull()) {
        return RecommendationPtr();
    }

    return ParseRecommendation(recommendationElement);
}

CountedItems<FriendRequest> ParseFriendsRequests(const QDomDocument &doc)
{
    const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
    if (responseElement.isNull()) {
        return CountedItems<FriendRequest>();
    }

    const auto& requestsElement = responseElement.firstChildElement("requests");
    if (requestsElement.isNull()) {
        return CountedItems<FriendRequest>();
    }

    const auto& friendsRequestsElement = requestsElement.firstChildElement("friend_requests");
    if (friendsRequestsElement.isNull()) {
        return CountedItems<FriendRequest>();
    }

    CountedItems<FriendRequest> friendsRequests;
    friendsRequests.m_BeginIndex = friendsRequestsElement.attribute("start").toULongLong();
    friendsRequests.m_EndIndex = friendsRequestsElement.attribute("end").toULongLong();
    friendsRequests.m_Count = friendsRequestsElement.attribute("total").toULongLong();
    friendsRequests.m_Items = ParseFriendsRequests(friendsRequestsElement);
    return friendsRequests;
}

CountedItems<FriendRecommendation> ParseFriendsRecommendations(const QDomDocument& doc)
{
    const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
    if (responseElement.isNull()) {
        return CountedItems<FriendRecommendation>();
    }

    const auto& requestsElement = responseElement.firstChildElement("requests");
    if (requestsElement.isNull()) {
        return CountedItems<FriendRecommendation>();
    }

    const auto& recommendationsElement = requestsElement.firstChildElement("friend_recommendations");
    if (recommendationsElement.isNull()) {
        return CountedItems<FriendRecommendation>();
    }

    CountedItems<FriendRecommendation> friendsRecommendations;
    friendsRecommendations.m_BeginIndex = recommendationsElement.attribute("start").toULongLong();
    friendsRecommendations.m_EndIndex = recommendationsElement.attribute("end").toULongLong();
    friendsRecommendations.m_Count = recommendationsElement.attribute("total").toULongLong();
    friendsRecommendations.m_Items = ParseFriendsRecommendations(recommendationsElement);
    return friendsRecommendations;
}

ReviewPtr ParseBookShelfAddedReview(const QDomDocument& doc)
{
    const auto& shelfElement = doc.firstChildElement("shelf");
    if (shelfElement.isNull()) {
        return ReviewPtr();
    }

    return ParseReviewFromShelf(shelfElement);
}

Reviews_t ParseBookShelfAddedReviews(const QDomDocument& doc)
{
    const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
    if (responseElement.isNull()) {
        return Reviews_t();
    }

    const auto& reviewsElement = responseElement.firstChildElement("reviews");
    if (reviewsElement.isNull()) {
        return Reviews_t();
    }

    return ParseReviewsList(reviewsElement);
}

}
}
}
