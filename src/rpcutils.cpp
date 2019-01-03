/*
The MIT License(MIT)

Copyright (c) 2018 Oleg Linkin <maledictusdemagog@gmail.com>

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

#include <QRegularExpression>
#include <QtDebug>

#include "objects/author.h"
#include "objects/book.h"
#include "objects/group.h"
#include "objects/review.h"
#include "objects/series.h"
#include "objects/serieswork.h"
#include "objects/topic.h"
#include "objects/user.h"
#include "objects/work.h"

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

//UserUpdate::Actor ParseActor(const QDomElement& element)
//{
//    UserUpdate::Actor actor;
//    const auto& fieldsList = element.childNodes();
//    for (int i = 0, fieldsCount = fieldsList.size(); i < fieldsCount; ++i) {
//        const auto& fieldElement = fieldsList.at (i).toElement ();
//        if (fieldElement.tagName() == "id") {
//            actor.m_Id = fieldElement.text().toULongLong();
//        }
//        else if (fieldElement.tagName() == "name") {
//            actor.m_Name = fieldElement.text();
//        }
//        else if (fieldElement.tagName() == "image_url") {
//            actor.m_AvatarUrl = QUrl(fieldElement.text().trimmed());
//        }
//        else if (fieldElement.tagName() == "link") {
//            actor.m_ProfileUrl = QUrl(fieldElement.text().trimmed());
//        }
//    }

//    return actor;
//}

UserPtr ParseUser(const QDomElement& element)
{
    auto user = std::make_shared<User>();
    const auto& fieldsList = element.childNodes();
    for (int i = 0, count = fieldsList.size(); i < count; ++i) {
        const auto& fieldElement = fieldsList.at(i).toElement();
        if (fieldElement.tagName() == "id") {
            user->SetId(fieldElement.text().toULongLong());
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
            //TODOuser->SetUserUpdates(ParseUserUpdates(fieldElement));
        }
        else if (fieldElement.tagName() == "user_statuses") {
            // No any sense to implement this
        }
     }
    return user;
}

//UserUpdate ParseUserUpdate(const QDomElement&)
//{
//    //TODO
//    return UserUpdate();
//}


//UserUpdate::Type GetType(const QString& typeStr)
//{
//    if (typeStr == "readstatus") {
//        return UserUpdate::Type::ReadStatus;
//    }

//    if (typeStr == "userstatus") {
//        return UserUpdate::Type::UserStatus;
//    }

//    if (typeStr == "review") {
//        return UserUpdate::Type::Review;
//    }

//    if (typeStr == "userchallenge") {
//        return UserUpdate::Type::UserChallenge;
//    }

//    return UserUpdate::Type::NoType;
//}

//UserUpdate::ActionType GetActionType(const QString& typeStr)
//{
//    if (typeStr == "rating") {
//        return UserUpdate::ActionType::Rating;
//    }

//    return UserUpdate::ActionType::NoType;
//}

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
            group->SetIsPublic(fieldElement.text().toLower() == "public");
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
            fr.SetId(fieldElement.text().toULongLong());
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

TopicPtr ParseTopic(const QDomElement& element)
{
    TopicPtr topic = std::make_shared<Topic>();
    const auto& fieldsList = element.childNodes();
    for (int i = 0, fieldsCount = fieldsList.size(); i < fieldsCount; ++i) {
        const auto& fieldElement = fieldsList.at (i).toElement ();
        if (fieldElement.tagName() == "id") {
            topic->SetId(fieldElement.text().toULongLong());
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
            author->SetId(fieldElement.text().toULongLong());
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
            book->SetId(fieldElement.text().toULongLong());
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
    const auto& fieldsList = element.childNodes();
    for (int i = 0, fieldsCount = fieldsList.size(); i < fieldsCount; ++i) {
        const auto& fieldElement = fieldsList.at (i).toElement ();
        if (fieldElement.tagName() == "id") {
            review->SetId(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "book") {
            review->SetBook(ParseBook(fieldElement));
        }
        else if (fieldElement.tagName() == "rating") {
            review->SetRating(fieldElement.text().toInt());
        }
        else if (fieldElement.tagName() == "votes") {
            review->SetVotes(fieldElement.text().toInt());
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

    return review;
}

AuthorPtr ParseAuthor(const QDomElement& element)
{
    AuthorPtr author = std::make_shared<Author>();
    const auto& fieldsList = element.childNodes();
    for (int i = 0, fieldsCount = fieldsList.size(); i < fieldsCount; ++i) {
        const auto& fieldElement = fieldsList.at (i).toElement ();
        if (fieldElement.tagName() == "id") {
            author->SetId(fieldElement.text().toULongLong());
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
                author->SetUserId(idElem.text().toULongLong());
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
            work->SetRatingCount(fieldElement.text().toULongLong());
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
    }

    return seriesWork;
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

//UserUpdates_t ParseUserUpdates(const QDomElement& element)
//{
//    UserUpdates_t updates;
//    const auto& nodes = element.childNodes();
//    for (int i = 0, count = nodes.size(); i < count; ++i) {
//        const auto& elem = nodes.at(i).toElement();
//        updates << ParseUserUpdate(elem);
//    }
//    return updates;
//}

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
        reviews << ParseReview(reviewsList.at (i).toElement());
    }
    return reviews;
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

}
}
}
