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

#include "basecommentsmodel.h"

#include "../objects/user.h"
#include "../sailreadsmanager.h"

namespace Sailreads
{
BaseCommentsModel::BaseCommentsModel(QObject *parent)
: BaseModel<Comment>(parent)
, m_HasMore(true)
, m_CurrentPage(1)
{
}

QVariant BaseCommentsModel::data(const QModelIndex& index, int role) const
{
    if (index.row() > m_Items.count() - 1 || index.row() < 0) {
        return QVariant();
    }

    const auto& comment = m_Items.at(index.row());
    switch (role) {
    case Id:
        return comment.GetId();
    case Body:
        return comment.GetBody();
    case UpdateDate:
        return comment.GetUpdateAtDate();
    case Author:
        return QVariant::fromValue(comment.GetAuthor());
    case CanDelete:
        return comment.GetCanDelete();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> BaseCommentsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Id] = "commentId";
    roles[Body] = "commentBody";
    roles[UpdateDate] = "commentUpdateDate";
    roles[Author] = "commentAuthor";
    roles[CanDelete] = "commentCanDelete";
    return roles;
}

bool BaseCommentsModel::GetHasMore() const
{
    return m_HasMore;
}

void BaseCommentsModel::SetHasMore(bool has)
{
    if (m_HasMore != has) {
        m_HasMore = has;
        emit hasMoreChanged();
    }
}

void BaseCommentsModel::cancelRequest()
{
    SailreadsManager::Instance(this)->abortRequest(this);
}

void BaseCommentsModel::handleGotComments(const CountedItems<Comment>& comments)
{
    if (comments.m_BeginIndex == 1) {
        m_CurrentPage = 1;
        SetItems(comments.m_Items);
    }
    else {
        AddItems(comments.m_Items);
    }

    SetHasMore(comments.m_EndIndex != comments.m_Count);
    if (m_HasMore) {
        ++m_CurrentPage;
    }
}
} // namespace Sailreads
