﻿/*
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

UserUpdate::Actor ParseActor(const QDomElement& element)
{
    UserUpdate::Actor actor;
    const auto& fieldsList = element.childNodes();
    for (int i = 0, fieldsCount = fieldsList.size(); i < fieldsCount; ++i) {
        const auto& fieldElement = fieldsList.at (i).toElement ();
        if (fieldElement.tagName() == "id") {
            actor.m_Id = fieldElement.text().toULongLong();
        }
        else if (fieldElement.tagName() == "name") {
            actor.m_Name = fieldElement.text();
        }
        else if (fieldElement.tagName() == "image_url") {
            actor.m_AvatarUrl = QUrl(fieldElement.text());
        }
        else if (fieldElement.tagName() == "link") {
            actor.m_ProfileUrl = QUrl(fieldElement.text());
        }
    }

    return actor;
}

User ParseUser(const QDomElement& element)
{
    User user;
    const auto& fieldsList = element.childNodes();
    for (int i = 0, fieldsCount = fieldsList.size(); i < fieldsCount; ++i) {
        const auto& fieldElement = fieldsList.at (i).toElement ();
        if (fieldElement.tagName() == "id") {
            user.SetId(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "first_name" ||
                fieldElement.tagName() == "name") {
            user.SetFirstName(fieldElement.text());
        }
        else if (fieldElement.tagName() == "last_name") {
            user.SetLastName(fieldElement.text());
        }
        else if (fieldElement.tagName() == "user_name") {
            user.SetNickName(fieldElement.text());
        }
        else if (fieldElement.tagName() == "p2_image_url" ||
                fieldElement.tagName() == "small_image_url") {
            user.SetAvatar(QUrl(fieldElement.text()));
        }
    }

    return user;
}

UserUpdate ParseUserUpdate(const QDomElement&)
{
    //TODO
    return UserUpdate();
}


UserUpdate::Type GetType(const QString& typeStr)
{
    if (typeStr == "readstatus") {
        return UserUpdate::Type::ReadStatus;
    }

    if (typeStr == "userstatus") {
        return UserUpdate::Type::UserStatus;
    }

    if (typeStr == "review") {
        return UserUpdate::Type::Review;
    }

    if (typeStr == "userchallenge") {
        return UserUpdate::Type::UserChallenge;
    }

    return UserUpdate::Type::NoType;
}

UserUpdate::ActionType GetActionType(const QString& typeStr)
{
    if (typeStr == "rating") {
        return UserUpdate::ActionType::Rating;
    }

    return UserUpdate::ActionType::NoType;
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
            shelf.SetName(fieldElement.text());
        }
        else if (fieldElement.tagName() == "book_count") {
            shelf.SetBooksCount(fieldElement.text().toUInt());
        }
        else if (fieldElement.tagName() == "description") {
            shelf.SetDescription(fieldElement.text());
        }
        else if (fieldElement.tagName() == "exclusive_flag") {
            shelf.SetExclusive(fieldElement.text() == "true");
        }
        else if (fieldElement.tagName() == "featured") {
            shelf.SetFeatured(fieldElement.text() == "true");
        }
        else if (fieldElement.tagName() == "sort") {
            shelf.SetSortable(fieldElement.text() == "true");
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
            gm.SetTitle(fieldElement.text());
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
            gf.SetId(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "name") {
            gf.SetName(fieldElement.text());
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

Group ParseGroup(const QDomElement& element)
{
    Group group;
    const auto& fieldsList = element.childNodes();
    for (int i = 0, fieldsCount = fieldsList.size(); i < fieldsCount; ++i) {
        const auto& fieldElement = fieldsList.at (i).toElement ();
        if (fieldElement.tagName() == "id") {
            group.SetId(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "title") {
            group.SetName(fieldElement.text());
        }
        else if (fieldElement.tagName() == "access") {
            group.SetIsPublic(fieldElement.text().toLower() == "public");
        }
        else if (fieldElement.tagName() == "users_count" ||
                fieldElement.tagName() == "group_users_count") {
            group.SetUsersCount(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "image_url" || fieldElement.tagName() == "p1_image_url") {
            group.SetImageUrl(QUrl(fieldElement.text()));
        }
        else if (fieldElement.tagName() == "last_activity_at" && !fieldElement.text().isEmpty()) {
            group.SetLastActivity(QDateTime::fromString(PrepareDateTimeString(fieldElement.text()),
                    Qt::ISODate));
        }
        else if (fieldElement.tagName() == "link") {
            group.SetUrl(QUrl(fieldElement.text()));
        }
        else if (fieldElement.tagName() == "description") {
            group.SetDescription(fieldElement.text());
        }
        else if (fieldElement.tagName() == "location") {
            group.SetLocation(fieldElement.text());
        }
        else if (fieldElement.tagName() == "display_folder_count") {
            group.SetDisplayFolderCount(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "display_topics_per_folder") {
            group.SetDisplayTopicsPerFolderCount(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "bookshelves_public_flag") {
            group.SetBookshelvesPublicFlag(fieldElement.text() == "true");
        }
        else if (fieldElement.tagName() == "add_book_flag") {
            group.SetAddBookFlag(fieldElement.text() == "true");
        }
        else if (fieldElement.tagName() == "add_events_flag") {
            group.SetAddEventsFlag(fieldElement.text() == "true");
        }
        else if (fieldElement.tagName() == "polls_flag") {
            group.SetPollsFlag(fieldElement.text() == "true");
        }
        else if (fieldElement.tagName() == "discussion_public_flag") {
            group.SetDiscussionPublicFlag(fieldElement.text() == "true");
        }
        else if (fieldElement.tagName() == "real_world_flag") {
            group.SetRealWorldFlag(fieldElement.text() == "true");
        }
        else if (fieldElement.tagName() == "accepting_new_members_flag") {
            group.SetAcceptingNewMemberFlag(fieldElement.text() == "true");
        }
        else if (fieldElement.tagName() == "category") {
            group.SetCategory(fieldElement.text());
        }
        else if (fieldElement.tagName() == "subcategory") {
            group.SetSubCategory(fieldElement.text());
        }
        else if (fieldElement.tagName() == "rules") {
            group.SetRules(fieldElement.text());
        }
        else if (fieldElement.tagName() == "folders") {
            group.SetGroupFolders(ParseGroupFolders(fieldElement));
        }
        else if (fieldElement.tagName() == "moderators") {
            group.SetGroupModerators(ParseGroupMembers(fieldElement));
        }
        else if (fieldElement.tagName() == "members") {
            group.SetGroupMembers(ParseGroupMembers(fieldElement));
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
            fr.SetName(fieldElement.text());
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
    }

    return fr;
}

Topic ParseTopic(const QDomElement& element)
{
    Topic topic;
    const auto& fieldsList = element.childNodes();
    for (int i = 0, fieldsCount = fieldsList.size(); i < fieldsCount; ++i) {
        const auto& fieldElement = fieldsList.at (i).toElement ();
        if (fieldElement.tagName() == "id") {
            topic.SetId(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "title") {
            topic.SetTitle(fieldElement.text());
        }
        else if (fieldElement.tagName() == "comments_count") {
            topic.SetCommentsCount(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "last_comment_at" && !fieldElement.text().isEmpty()) {
            topic.SetLastCommentDate(QDateTime::fromString(PrepareDateTimeString(fieldElement.text()),
                    Qt::ISODate));
        }
        else if (fieldElement.tagName() == "context_type") {
            topic.SetContextType(fieldElement.text());
        }
        else if (fieldElement.tagName() == "context_id") {
            topic.SetContextId(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "author_user") {
            topic.SetAuthor(ParseUser(fieldElement));
        }
        else if (fieldElement.tagName() == "folder") {
            topic.SetGroupFolder(ParseGroupFolder(fieldElement));
        }
        else if (fieldElement.tagName() == "author_user_id") {
            User author = topic.GetAuthor();
            author.SetId(fieldElement.text().toULongLong());
            topic.SetAuthor(author);
        }
        else if (fieldElement.tagName() == "author_user_name") {
            User author = topic.GetAuthor();
            author.SetFirstName(fieldElement.text());
            topic.SetAuthor(author);
        }
        else if (fieldElement.tagName() == "new_comments_count") {
            topic.SetNewCommentsCount(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "comments_per_page") {
            topic.SetCommentsPerPage(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "updated_at" && !fieldElement.text().isEmpty()) {
            topic.SetUpdatedAt(QDateTime::fromString(PrepareDateTimeString(fieldElement.text()),
                    Qt::ISODate));
        }
        else if (fieldElement.tagName() == "subject_type") {
           topic.SetSubjectType(fieldElement.text());
        }
        else if (fieldElement.tagName() == "group") {
            topic.SetGroup(ParseGroup(fieldElement));
        }
        else if (fieldElement.tagName() == "comments") {
            topic.SetComments(ParseComments(fieldElement));
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
            comment.SetBody(fieldElement.text());
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

Book ParseBook(const QDomElement& element)
{
    Book book;
    const auto& fieldsList = element.childNodes();
    for (int i = 0, fieldsCount = fieldsList.size(); i < fieldsCount; ++i) {
        const auto& fieldElement = fieldsList.at (i).toElement ();
        if (fieldElement.tagName() == "id") {
            book.SetId(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "isbn") {
            book.SetISBN(fieldElement.text());
        }
        else if (fieldElement.tagName() == "isbn13") {
            book.SetISBN13(fieldElement.text());
        }
        else if (fieldElement.tagName() == "text_reviews_count") {
            book.SetTextReviewsCount(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "title") {
            book.SetTitle(fieldElement.text());
        }
        else if (fieldElement.tagName() == "title_without_series") {
            book.SetTitleWithoutSeries(fieldElement.text());
        }
        else if (fieldElement.tagName() == "image_url") {
            book.SetImageUrl(QUrl(fieldElement.text()));
        }
        else if (fieldElement.tagName() == "small_image_url") {
            book.SetSmallImageUrl(QUrl(fieldElement.text()));
        }
        else if (fieldElement.tagName() == "large_image_url") {
            book.SetLargeImageUrl(QUrl(fieldElement.text()));
        }
        else if (fieldElement.tagName() == "link") {
            book.SetLink(QUrl(fieldElement.text()));
        }
        else if (fieldElement.tagName() == "num_pages") {
            book.SetNumPages(fieldElement.text().toInt());
        }
        else if (fieldElement.tagName() == "format") {
            book.SetFormat(fieldElement.text());
        }
        else if (fieldElement.tagName() == "edition_information") {
            book.SetEditionInformation(fieldElement.text());
        }
        else if (fieldElement.tagName() == "publisher") {
            book.SetPublisher(fieldElement.text());
        }
        else if (fieldElement.tagName() == "publication_day") {
            book.SetPublicationDay(fieldElement.text().toInt());
        }
        else if (fieldElement.tagName() == "publication_year") {
            book.SetPublicationYear(fieldElement.text().toInt());
        }
        else if (fieldElement.tagName() == "publication_month") {
            book.SetPublicationMonth(fieldElement.text().toInt());
        }
        else if (fieldElement.tagName() == "average_rating") {
            book.SetAverageRating(fieldElement.text().toDouble());
        }
        else if (fieldElement.tagName() == "ratings_count") {
            book.SetRatingsCount(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "description") {
            book.SetDescription(fieldElement.text());
        }
        else if (fieldElement.tagName() == "published") {
            book.SetPublishedYear(fieldElement.text().toInt());
        }
        else if (fieldElement.tagName() == "authors") {
            book.SetAuthors(ParseAuthors(fieldElement));
        }
        else if (fieldElement.tagName() == "work") {
            const auto& workList = fieldElement.childNodes();
            for (int i = 0, cnt = workList.size(); i < cnt; ++i) {
                const auto& workElement = workList.at (i).toElement ();
                if (workElement.tagName() == "id") {
                    book.SetWorkId(workElement.text().toULongLong());
                    break;
                }
            }
        }
    }

    return book;
}

Review ParseReview(const QDomElement& element)
{
    Review review;
    const auto& fieldsList = element.childNodes();
    for (int i = 0, fieldsCount = fieldsList.size(); i < fieldsCount; ++i) {
        const auto& fieldElement = fieldsList.at (i).toElement ();
        if (fieldElement.tagName() == "id") {
            review.SetId(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "book") {
            review.SetBook(ParseBook(fieldElement));
        }
        else if (fieldElement.tagName() == "rating") {
            review.SetRating(fieldElement.text().toInt());
        }
        else if (fieldElement.tagName() == "votes") {
            review.SetVotes(fieldElement.text().toInt());
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
            review.SetShelves(shelves);
        }
        else if (fieldElement.tagName() == "date_added" && !fieldElement.text().isEmpty()) {
            review.SetAddedDate(QDateTime::fromString(PrepareDateTimeString(fieldElement.text()),
                    Qt::ISODate));
        }
        else if (fieldElement.tagName() == "date_updated" && !fieldElement.text().isEmpty()) {
            review.SetUpdatedDate(QDateTime::fromString(PrepareDateTimeString(fieldElement.text()),
                    Qt::ISODate));
        }
        else if (fieldElement.tagName() == "date_read" && !fieldElement.text().isEmpty()) {
            review.SetReadDate(QDateTime::fromString(PrepareDateTimeString(fieldElement.text()),
                    Qt::ISODate));
        }
        else if (fieldElement.tagName() == "started_at" && !fieldElement.text().isEmpty()) {
            review.SetStartedDate(QDateTime::fromString(PrepareDateTimeString(fieldElement.text()),
                    Qt::ISODate));
        }
        else if (fieldElement.tagName() == "read_count") {
            review.SetReadCount(fieldElement.text().toInt());
        }
        else if (fieldElement.tagName() == "body") {
            review.SetBody(fieldElement.text());
        }
        else if (fieldElement.tagName() == "comments_count") {
            review.SetCommentsCount(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "url") {
            review.SetUrl(fieldElement.text());
        }
        else if (fieldElement.tagName() == "owned") {
            review.SetOwned(fieldElement.text().toULongLong());
        }
    }

    return review;
}

Author ParseAuthor(const QDomElement& element)
{
    Author author;
    const auto& fieldsList = element.childNodes();
    for (int i = 0, fieldsCount = fieldsList.size(); i < fieldsCount; ++i) {
        const auto& fieldElement = fieldsList.at (i).toElement ();
        if (fieldElement.tagName() == "id") {
            author.SetId(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "name") {
            author.SetName(fieldElement.text());
        }
        else if (fieldElement.tagName() == "image_url") {
            author.SetImageUrl(QUrl(fieldElement.text()));
        }
        else if (fieldElement.tagName() == "small_image_url") {
            author.SetSmallImageUrl(QUrl(fieldElement.text()));
        }
        else if (fieldElement.tagName() == "link") {
            author.SetLink(QUrl(fieldElement.text()));
        }
        else if (fieldElement.tagName() == "average_rating") {
            author.SetAverageRating(fieldElement.text().toDouble());
        }
        else if (fieldElement.tagName() == "ratings_count") {
            author.SetRatingsCount(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "text_reviews_count") {
            author.SetTextReviewsCount(fieldElement.text().toULongLong());
        }
    }

    return author;
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

UserUpdates_t ParseUserUpdates(const QDomElement& element)
{
    UserUpdates_t updates;
    const auto& nodes = element.childNodes();
    for (int i = 0, count = nodes.size(); i < count; ++i) {
        const auto& elem = nodes.at(i).toElement();
        updates << ParseUserUpdate(elem);
    }
    return updates;
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

CountedItems<Review> ParseReviews(const QDomElement& element)
{
    CountedItems<Review> reviews;
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

std::shared_ptr<UserProfile> ParseUserProfile(const QDomDocument &doc)
{
    std::shared_ptr<UserProfile> profile;
    const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
    if (responseElement.isNull()) {
        return profile;
    }

    profile.reset(new UserProfile());
    const auto& userElement = responseElement.firstChildElement("user");
    const auto& fieldsList = userElement.childNodes();
    for (int i = 0, count = fieldsList.size(); i < count; ++i) {
        const auto& fieldElement = fieldsList.at(i).toElement();
        if (fieldElement.tagName() == "id") {
            profile->SetUserID(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "name") {
            profile->SetUserName(fieldElement.text());
        }
        else if (fieldElement.tagName() == "user_name") {
            profile->SetNickName(fieldElement.text());
        }
        else if (fieldElement.tagName() == "link") {
            profile->SetWebUrl(QUrl(fieldElement.text()));
        }
        else if (fieldElement.tagName() == "image_url") {
            profile->SetAvatarUrl(QUrl(fieldElement.text()));
        }
        else if (fieldElement.tagName() == "small_image_url") {
            profile->SetSmallAvatarUrl(QUrl(fieldElement.text()));
        }
        else if (fieldElement.tagName() == "about") {
            profile->SetAbout(fieldElement.text());
        }
        else if (fieldElement.tagName() == "age") {
            profile->SetAge(fieldElement.text().toUInt());
        }
        else if (fieldElement.tagName() == "gender") {
            profile->SetGender(fieldElement.text());
        }
        else if (fieldElement.tagName() == "location") {
            profile->SetLocation(fieldElement.text());
        }
        else if (fieldElement.tagName() == "website") {
            profile->SetWebSite(QUrl(fieldElement.text()));
        }
        else if (fieldElement.tagName() == "joined") {
            profile->SetJoinedDate(fieldElement.text());
        }
        else if (fieldElement.tagName() == "last_active") {
            profile->SetLastUpdateDate(fieldElement.text());
        }
        else if (fieldElement.tagName() == "interests") {
            profile->SetInterests(fieldElement.text ());
        }
        else if (fieldElement.tagName() == "favorite_books") {
            profile->SetFavoriteBooksDesc(fieldElement.text());
        }
        else if (fieldElement.tagName() == "favorite_authors") {
            profile->SetFavoriteAuthors(ParseFavoriteAuthors(fieldElement));
        }
        else if (fieldElement.tagName() == "updates_rss_url") {
            profile->SetUpdateRSSUrl(QUrl(fieldElement.text()));
        }
        else if (fieldElement.tagName() == "reviews_rss_url") {
            profile->SetReviewRSSUrl(QUrl(fieldElement.text()));
        }
        else if (fieldElement.tagName() == "friends_count") {
            profile->SetFriendsCount(fieldElement.text().toUInt());
        }
        else if (fieldElement.tagName() == "groups_count") {
            profile->SetGroupsCount(fieldElement.text().toUInt());
        }
        else if (fieldElement.tagName() == "reviews_count") {
            profile->SetBooksCount(fieldElement.text().toUInt());
        }
        else if (fieldElement.tagName() == "private") {
            profile->SetPrivate(fieldElement.text() == "true");
        }
        else if (fieldElement.tagName() == "user_shelves") {
            profile->SetBookShelves(ParseBookShelves(fieldElement));
        }
        //TODO updates later
//        else if (fieldElement.tagName() == "updates") {
//            profile->SetUserUpdates(ParseUserUpdates(fieldElement));
//        }
     }

    return profile;
}

BookShelves_t ParseBookShelves(const QDomDocument& doc)
{
    const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
    if (responseElement.isNull()) {
        return BookShelves_t();
    }

    return ParseBookShelves(responseElement.firstChildElement("shelves"));
}

CountedItems<Group> ParseGroups(const QDomDocument& doc)
{
    const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
    if (responseElement.isNull()) {
        return CountedItems<Group>();
    }

    const auto& groupsElement = responseElement.firstChildElement("groups");
    if (groupsElement.isNull()) {
        return CountedItems<Group>();
    }

    const auto& groupsListElement = groupsElement.firstChildElement("list");
    if (groupsListElement.isNull()) {
        return CountedItems<Group>();
    }

    CountedItems<Group> groups;
    groups.m_BeginIndex = groupsListElement.attribute("start").toULongLong();
    groups.m_EndIndex = groupsListElement.attribute("end").toULongLong();
    groups.m_Count = groupsListElement.attribute("total").toULongLong();
    groups.m_Items = ParseGroups(groupsListElement);
    return groups;
}

Friends_t ParseFriends(const QDomDocument& doc)
{
    const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
    if (responseElement.isNull()) {
        return Friends_t();
    }

    return ParseFriends(responseElement.firstChildElement("friends"));
}

Group ParseGroup(const QDomDocument& doc)
{
    const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
    if (responseElement.isNull()) {
        return Group();
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

CountedItems<Topic> ParseGroupFolderTopics(const QDomDocument& doc)
{
    const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
    if (responseElement.isNull()) {
        return CountedItems<Topic>();
    }

    const auto& groupFolderElement = responseElement.firstChildElement("group_folder");
    if (groupFolderElement.isNull()) {
        return CountedItems<Topic>();
    }

    const auto& topicsListElement = groupFolderElement.firstChildElement("topics");
    if (topicsListElement.isNull()) {
        return CountedItems<Topic>();
    }

    CountedItems<Topic> topics;
    topics.m_BeginIndex = topicsListElement.attribute("start").toULongLong();
    topics.m_EndIndex = topicsListElement.attribute("end").toULongLong();
    topics.m_Count = topicsListElement.attribute("total").toULongLong();
    topics.m_Items = ParseTopics(topicsListElement);
    return topics;
}

Topic ParseGroupFolderTopic(const QDomDocument& doc)
{
    const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
    if (responseElement.isNull()) {
        return Topic();
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

QPair<quint64, CountedItems<Review>> ParseReviews(const QDomDocument& doc)
{
    const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
    if (responseElement.isNull()) {
        return QPair<quint64, CountedItems<Review>>();
    }

    const auto& shelfElement = responseElement.firstChildElement("shelf");
    if (shelfElement.isNull()) {
        return QPair<quint64, CountedItems<Review>>();
    }

    const quint64 shelfId = shelfElement.attribute("id").toULong();

    const auto& reviewsListElement = responseElement.firstChildElement("reviews");
    if (reviewsListElement.isNull()) {
        return qMakePair(shelfId, CountedItems<Review>());
    }

    return qMakePair(shelfId, ParseReviews(reviewsListElement));
}
}
}
}