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

#include <memory>

#include <QDomDocument>
#include <QDomElement>
#include <QPair>

#include "objects/bookshelf.h"
#include "objects/friend.h"
#include "objects/group.h"
#include "objects/review.h"
#include "objects/topic.h"
#include "objects/work.h"
#include "objects/serieswork.h"
#include "types.h"
#include "userupdate.h"
#include "userprofile.h"

namespace Sailreads
{
namespace RpcUtils
{
namespace Parser
{
UserUpdate::Actor ParseActor(const QDomElement& element);
UserPtr ParseUser(const QDomElement& element);
UserUpdate ParseUserUpdate(const QDomElement& element);
BookShelf ParseBookShelf(const QDomElement& element);
FavoriteAuthors_t ParseFavoriteAuthors(const QDomElement& faElement);
GroupMember ParseGroupMember(const QDomElement& element);
GroupFolder ParseGroupFolder(const QDomElement& element);
GroupPtr ParseGroup(const QDomElement& element);
Friend ParseFriend(const QDomElement& element);
Topic ParseTopic(const QDomElement& element);
Comment ParseComment(const QDomElement& element);
BookPtr ParseBook(const QDomElement& element);
ReviewPtr ParseReview(const QDomElement& element);
AuthorPtr ParseAuthor(const QDomElement& element);
Work ParseWork(const QDomElement& element);
Series ParseSeries(const QDomElement& element);
SeriesWork ParseSeriesWork(const QDomElement& element);

GroupMembers_t ParseGroupMembers(const QDomElement& element);
GroupFolders_t ParseGroupFolders(const QDomElement& element);
UserUpdates_t ParseUserUpdates(const QDomElement& element);
BookShelves_t ParseBookShelves(const QDomElement& element);
Groups_t ParseGroups(const QDomElement& element);
Friends_t ParseFriends(const QDomElement& element);
Topics_t ParseTopics(const QDomElement& element);
Authors_t ParseAuthors(const QDomElement& element);
CountedItems<Comment> ParseComments(const QDomElement& element);
CountedItems<ReviewPtr> ParseReviews(const QDomElement& element);
SeriesWorks_t ParseSeriesWorks(const QDomElement& element);

UserPtr ParseUser(const QDomDocument& doc);
BookShelves_t ParseBookShelves(const QDomDocument& doc);
CountedItems<GroupPtr> ParseGroups(const QDomDocument& doc);
Friends_t ParseFriends(const QDomDocument& doc);
GroupPtr ParseGroup(const QDomDocument& doc);
GroupMembers_t ParseGroupMembers(const QDomDocument& doc);
CountedItems<Topic> ParseGroupFolderTopics(const QDomDocument& doc);
Topic ParseGroupFolderTopic(const QDomDocument& doc);
Comment ParseComment(const QDomDocument& doc);
QPair<quint64, CountedItems<ReviewPtr>> ParseReviews(const QDomDocument& doc);
BookPtr ParseBook(const QDomDocument& doc);
}
}
}
