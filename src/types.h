/*
Copyright (c) 2018-2019 Oleg Linkin <maledictusdemagog@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
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

#pragma once

#include <memory>

#include <QDateTime>
#include <QString>
#include <QUrl>

namespace Sailreads
{
struct FavoriteAuthor
{
    quint64 m_Id;
    QString m_Name;
    bool operator ==(const FavoriteAuthor& author)
    {
        return m_Id == author.m_Id && m_Name == author.m_Name;
    }
};
typedef QList<FavoriteAuthor> FavoriteAuthors_t;

template<typename T>
struct CountedItems
{
    quint64 m_BeginIndex;
    quint64 m_EndIndex;
    quint64 m_Count;
    QList<T> m_Items;

    CountedItems()
    : m_BeginIndex(0)
    , m_EndIndex(0)
    , m_Count(0)
    {}
};

template<typename T>
struct PageCountedItems
{
    int m_Page;
    int m_PagesCount;
    QList<T> m_Items;

    PageCountedItems()
    : m_Page(0)
    , m_PagesCount(0)
    {}
};

struct ReviewInfo
{
    QString m_ReviewId;
    QString m_UserId;
    QString m_BookId;
    int m_Rating;
    QString m_Review;
    QDateTime m_ReadDate;
    QDateTime m_StartedDate;
    QDateTime m_UpdateDate;
    QDateTime m_CreateDate;
    quint64 m_WorkId;
    int m_ReadCount;
    QString m_ReadStatus;
};

class User;
typedef std::shared_ptr<User> UserPtr;
typedef QList<UserPtr> Users_t;

class Author;
typedef std::shared_ptr<Author> AuthorPtr;
typedef QList<AuthorPtr> Authors_t;

class Book;
typedef std::shared_ptr<Book> BookPtr;
typedef QList<BookPtr> Books_t;

class Group;
typedef std::shared_ptr<Group> GroupPtr;
typedef QList<GroupPtr> Groups_t;

class Review;
typedef std::shared_ptr<Review> ReviewPtr;
typedef QList<ReviewPtr> Reviews_t;

class Series;
typedef std::shared_ptr<Series> SeriesPtr;
typedef QList<SeriesPtr> Series_t;

class SeriesWork;
typedef std::shared_ptr<SeriesWork> SeriesWorkPtr;
typedef QList<SeriesWorkPtr> SeriesWorks_t;

class Topic;
typedef std::shared_ptr<Topic> TopicPtr;
typedef QList<TopicPtr> Topics_t;

class Work;
typedef std::shared_ptr<Work> WorkPtr;
typedef QList<WorkPtr> Works_t;

class Notification;
typedef std::shared_ptr<Notification> NotificationPtr;
typedef QList<NotificationPtr> Notifications_t;

class Message;
typedef std::shared_ptr<Message> MessagePtr;
typedef QList<MessagePtr> Messages_t;

class Update;
typedef std::shared_ptr<Update> UpdatePtr;
typedef QList<UpdatePtr> Updates_t;

typedef std::shared_ptr<QObject> QObjectPtr;

class ReadStatus;
typedef std::shared_ptr<ReadStatus> ReadStatusPtr;
typedef QList<ReadStatusPtr> ReadStatuses_t;

class UserStatus;
typedef std::shared_ptr<UserStatus> UserStatusPtr;
typedef QList<UserStatusPtr> UserStatuses_t;

class Recommendation;
typedef std::shared_ptr<Recommendation> RecommendationPtr;
}
