/*
Copyright (c) 2018 Oleg Linkin <maledictusdemagog@gmail.com>

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

#include "group.h"

#include <QtDebug>

namespace Sailreads
{
Group::Group(QObject *parent)
: QObject(parent)
, m_Id(0)
, m_Flags(NoFlags)
{
#ifdef QT_DEBUG
    qDebug() << this << "CONSTRUCTED";
#endif
}

Group::~Group()
{
#ifdef QT_DEBUG
    qDebug() << this << "DESTRUCTED";
#endif
}

quint64 Group::GetId() const
{
    return m_Id;
}

void Group::SetId(quint64 id)
{
    m_Id = id;
}

QString Group::GetName() const
{
    return m_Name;
}

void Group::SetName(const QString& name)
{
    m_Name = name;
}

bool Group::GetIsPublic() const
{
    return m_IsPublic;
}

void Group::SetIsPublic(bool isPublic)
{
    m_IsPublic = isPublic;
}

quint64 Group::GetUsersCount() const
{
    return m_UsersCount;
}

void Group::SetUsersCount(quint64 count)
{
    m_UsersCount = count;
}

QUrl Group::GetImageUrl() const
{
    return m_ImageUrl;
}

void Group::SetImageUrl(const QUrl& url)
{
    m_ImageUrl = url;
}

QDateTime Group::GetLastActivity() const
{
    return m_LastActivity;
}

void Group::SetLastActivity(const QDateTime& dt)
{
    m_LastActivity = dt;
}

QUrl Group::GetUrl() const
{
    return m_Url;
}

void Group::SetUrl(const QUrl& url)
{
    m_Url = url;
}

QString Group::GetDescription() const
{
    return m_Description;
}

void Group::SetDescription(const QString& description)
{
    m_Description = description;
}

QString Group::GetLocation() const
{
    return m_Location;
}

void Group::SetLocation(const QString& location)
{
    m_Location = location;
}

quint64 Group::GetDisplayFolderCount() const
{
    return m_DisplayFolderCount;
}

void Group::SetDisplayFolderCount(const quint64& displayFolderCount)
{
    m_DisplayFolderCount = displayFolderCount;
}

quint64 Group::GetDisplayTopicsPerFolderCount() const
{
    return m_DisplayTopicsPerFolderCount;
}

void Group::SetDisplayTopicsPerFolderCount(const quint64& displayTopicsPerFolderCount)
{
    m_DisplayTopicsPerFolderCount = displayTopicsPerFolderCount;
}

Group::GroupFlags Group::GetGroupFlags() const
{
    return m_Flags;
}

void Group::SetBookshelvesPublicFlag(bool flag)
{
    m_Flags &= flag ? BookshelvesPublic : ~BookshelvesPublic;
}

void Group::SetAddBookFlag(bool flag)
{
    m_Flags &= flag ? AddBook : ~AddBook;
}

void Group::SetAddEventsFlag(bool flag)
{
    m_Flags &= flag ? AddEvents : ~AddEvents;
}

void Group::SetPollsFlag(bool flag)
{
    m_Flags &= flag ? Polls : ~Polls;
}

void Group::SetDiscussionPublicFlag(bool flag)
{
    m_Flags &= flag ? DiscussionPublic : ~DiscussionPublic;
}

void Group::SetRealWorldFlag(bool flag)
{
    m_Flags &= flag ? RealWorld : ~RealWorld;
}

void Group::SetAcceptingNewMemberFlag(bool flag)
{
    m_Flags &= flag ? AcceptNewMembers : ~AcceptNewMembers;
}

QString Group::GetCategory() const
{
    return m_Category;
}

void Group::SetCategory(const QString& category)
{
    m_Category = category;
}

QString Group::GetSubCategory() const
{
    return m_SubCategory;
}

void Group::SetSubCategory(const QString& subCategory)
{
    m_SubCategory = subCategory;
}

QString Group::GetRules() const
{
    return m_Rules;
}

void Group::SetRules(const QString& rules)
{
    m_Rules = rules;
}

GroupFolders_t Group::GetGroupFolders() const
{
    return m_GroupFolders;
}

void Group::SetGroupFolders(const GroupFolders_t& groupFolders)
{
    m_GroupFolders = groupFolders;
}

GroupMembers_t Group::GetGroupModerators() const
{
    return m_GroupModerators;
}

void Group::SetGroupModerators(const GroupMembers_t& groupModerators)
{
    m_GroupModerators = groupModerators;
}

GroupMembers_t Group::GetGroupMembers() const
{
    return m_GroupMembers;
}

void Group::SetGroupMembers(const GroupMembers_t& groupMembers)
{
    m_GroupMembers = groupMembers;
}
} // namespace Sailreads
