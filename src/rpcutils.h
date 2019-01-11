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

#include <memory>

#include <QDomDocument>
#include <QDomElement>
#include <QPair>

#include "objects/bookshelf.h"
#include "objects/comment.h"
#include "objects/friend.h"
#include "objects/friendrequest.h"
#include "objects/groupfolder.h"
#include "objects/groupmember.h"
#include "types.h"

namespace Sailreads
{
namespace RpcUtils
{
namespace Parser
{
//UserUpdate::Actor ParseActor(const QDomElement& element);
UserPtr ParseUser(const QDomElement& element);
//UserUpdate ParseUserUpdate(const QDomElement& element);
BookShelf ParseBookShelf(const QDomElement& element);
FavoriteAuthors_t ParseFavoriteAuthors(const QDomElement& faElement);
GroupMember ParseGroupMember(const QDomElement& element);
GroupFolder ParseGroupFolder(const QDomElement& element);
GroupPtr ParseGroup(const QDomElement& element);
Friend ParseFriend(const QDomElement& element);
FriendRequest ParseFriendRequest(const QDomElement& element);
TopicPtr ParseTopic(const QDomElement& element);
Comment ParseComment(const QDomElement& element);
BookPtr ParseBook(const QDomElement& element);
ReviewPtr ParseReview(const QDomElement& element);
AuthorPtr ParseAuthor(const QDomElement& element);
WorkPtr ParseWork(const QDomElement& element);
SeriesPtr ParseSeries(const QDomElement& element);
SeriesWorkPtr ParseSeriesWork(const QDomElement& element);
MessagePtr ParseMessage(const QDomElement& element);

GroupMembers_t ParseGroupMembers(const QDomElement& element);
GroupFolders_t ParseGroupFolders(const QDomElement& element);
//UserUpdates_t ParseUserUpdates(const QDomElement& element);
BookShelves_t ParseBookShelves(const QDomElement& element);
Groups_t ParseGroups(const QDomElement& element);
Friends_t ParseFriends(const QDomElement& element);
Topics_t ParseTopics(const QDomElement& element);
Authors_t ParseAuthors(const QDomElement& element);
CountedItems<Comment> ParseComments(const QDomElement& element);
CountedItems<ReviewPtr> ParseReviews(const QDomElement& element);
SeriesWorks_t ParseSeriesWorks(const QDomElement& element);
Series_t ParseSeriesFromSeriesWorks(const QDomElement& element);
Books_t ParseBooksList(const QDomElement& element);
CountedItems<BookPtr> ParseBooks(const QDomElement& element);
Reviews_t ParseReviewsList(const QDomElement& element);
Series_t ParseAuthorSeries(const QDomElement& element);
Messages_t ParseMessagesList(const QDomElement& element);
CountedItems<MessagePtr> ParseMessages(const QDomElement& element);


UserPtr ParseUser(const QDomDocument& doc);
CountedItems<BookShelf> ParseBookShelves(const QDomDocument& doc);
CountedItems<GroupPtr> ParseGroups(const QDomDocument& doc);
CountedItems<Friend> ParseFriends(const QDomDocument& doc);
CountedItems<Friend> ParseFollowings(const QDomDocument& doc);
CountedItems<Friend> ParseFollowers(const QDomDocument& doc);
GroupPtr ParseGroup(const QDomDocument& doc);
GroupMembers_t ParseGroupMembers(const QDomDocument& doc);
CountedItems<TopicPtr> ParseGroupFolderTopics(const QDomDocument& doc);
TopicPtr ParseGroupFolderTopic(const QDomDocument& doc);
Comment ParseComment(const QDomDocument& doc);
QPair<quint64, CountedItems<ReviewPtr>> ParseReviews(const QDomDocument& doc);
BookPtr ParseBook(const QDomDocument& doc);
SeriesPtr ParseSeries(const QDomDocument& doc);
AuthorPtr ParseAuthor(const QDomDocument& doc);
CountedItems<BookPtr> ParseAuthorBooks(const QDomDocument& doc);
ReviewPtr ParseReview(const QDomDocument& doc);
Series_t ParseAuthorSeries(const QDomDocument& doc);
CountedItems<BookPtr> ParseBookEditions(const QDomDocument& doc);
CountedItems<MessagePtr> ParseMessages(const QDomDocument& doc);
MessagePtr ParseMessage(const QDomDocument& doc);
}
}
}
