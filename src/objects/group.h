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

#pragma once

#include <QDateTime>
#include <QList>
#include <QUrl>

#include "groupfolder.h"
#include "groupmember.h"

namespace Sailreads
{
class Group
{
    Q_GADGET

    quint64 m_Id;
    QString m_Name;
    bool m_IsPublic;
    quint64 m_UsersCount;
    QUrl m_ImageUrl;
    QDateTime m_LastActivity;
    QUrl m_Url;
    QString m_Description;
    QString m_Location;
    quint64 m_DisplayFolderCount;
    quint64 m_DisplayTopicsPerFolderCount;
    QString m_Category;
    QString m_SubCategory;
    QString m_Rules;
    GroupFolders_t m_GroupFolders;
    GroupMembers_t m_GroupModerators;
    GroupMembers_t m_GroupMembers;
public:
    enum GroupFlag
    {
        NoFlags = 0,
        BookshelvesPublic = 1,
        AddBook = 2,
        AddEvents = 4,
        Polls = 8,
        DiscussionPublic = 16,
        RealWorld = 32,
        AcceptNewMembers = 64
    };
    Q_DECLARE_FLAGS(GroupFlags, GroupFlag)
private:
    GroupFlags m_Flags;

    Q_PROPERTY(quint64 id READ GetId)
    Q_PROPERTY(QString name READ GetName)
    Q_PROPERTY(bool isPublic READ GetIsPublic)
    Q_PROPERTY(quint64 usersCount READ GetUsersCount)
    Q_PROPERTY(QUrl imageUrl READ GetImageUrl)
    Q_PROPERTY(QDateTime lastActivity READ GetLastActivity)
    Q_PROPERTY(QUrl url READ GetUrl)
    Q_PROPERTY(QString description READ GetDescription)
    Q_PROPERTY(QString location READ GetLocation)
    Q_PROPERTY(quint64 displayFolderCount READ GetDisplayFolderCount)
    Q_PROPERTY(quint64 displayTopicsPerFolderCount READ GetDisplayTopicsPerFolderCount)
    Q_PROPERTY(QString category READ GetCategory)
    Q_PROPERTY(QString subCategory READ GetSubCategory)
    Q_PROPERTY(QString rules READ GetRules)
    Q_PROPERTY(GroupFlags flags READ GetGroupFlags)
public:
    Group();

    quint64 GetId() const;
    void SetId(quint64 id);
    QString GetName() const;
    void SetName(const QString& name);
    bool GetIsPublic() const;
    void SetIsPublic(bool isPublic);
    quint64 GetUsersCount() const;
    void SetUsersCount(quint64 count);
    QUrl GetImageUrl() const;
    void SetImageUrl(const QUrl& url);
    QDateTime GetLastActivity() const;
    void SetLastActivity(const QDateTime& dt);
    QUrl GetUrl() const;
    void SetUrl(const QUrl& url);
    QString GetDescription() const;
    void SetDescription(const QString& description);
    QString GetLocation() const;
    void SetLocation(const QString& location);
    quint64 GetDisplayFolderCount() const;
    void SetDisplayFolderCount(const quint64& displayFolderCount);
    quint64 GetDisplayTopicsPerFolderCount() const;
    void SetDisplayTopicsPerFolderCount(const quint64& displayTopicsPerFolderCount);
    GroupFlags GetGroupFlags() const;
    void SetBookshelvesPublicFlag(bool flag);
    void SetAddBookFlag(bool flag);
    void SetAddEventsFlag(bool flag);
    void SetPollsFlag(bool flag);
    void SetDiscussionPublicFlag(bool flag);
    void SetRealWorldFlag(bool flag);
    void SetAcceptingNewMemberFlag(bool flag);
    QString GetCategory() const;
    void SetCategory(const QString& category);
    QString GetSubCategory() const;
    void SetSubCategory(const QString& subCategory);
    QString GetRules() const;
    void SetRules(const QString& rules);
    GroupFolders_t GetGroupFolders() const;
    void SetGroupFolders(const GroupFolders_t& groupFolders);
    GroupMembers_t GetGroupModerators() const;
    void SetGroupModerators(const GroupMembers_t& groupModerators);
    GroupMembers_t GetGroupMembers() const;
    void SetGroupMembers(const GroupMembers_t& groupMembers);
};
Q_DECLARE_OPERATORS_FOR_FLAGS(Group::GroupFlags)

typedef QList<Group> Groups_t;

struct Groups
{
    quint64 m_BeginIndex;
    quint64 m_EndIndex;
    quint64 m_Count;
    Groups_t m_Groups;

    Groups()
    : m_BeginIndex(0)
    , m_EndIndex(0)
    , m_Count(0)
    {}
};

} // namespace Sailreads
