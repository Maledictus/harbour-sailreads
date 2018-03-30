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

#include "basemodel.h"
#include "../objects/friend.h"

namespace Sailreads
{
class FriendsModel : public BaseModel<Friend>
{
    Q_OBJECT

    Q_ENUMS(FriendsRoles)

    quint64 m_UserId;

    Q_PROPERTY(quint64 userId READ GetUserId WRITE SetUserId NOTIFY userIdChanged)
public:
    enum GroupRoles
    {
        Id = Qt::UserRole + 1,
        Name,
        Url,
        AvatarImage,
        SmallAvatarImage,
        FriendsCount,
        BooksCount,
        CreatedAt
    };

    explicit FriendsModel(QObject *parent = nullptr);

    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    virtual QHash<int, QByteArray> roleNames() const override;
    virtual bool canFetchMore(const QModelIndex& parent) const override;
    virtual void fetchMore(const QModelIndex& parent) override;

    quint64 GetUserId() const;
    void SetUserId(quint64 id);

private slots:
    void handleGotUserFriends(quint64 userId, const Friends_t& friends);

signals:
    void userIdChanged();
};

} // namespace Sailreads
