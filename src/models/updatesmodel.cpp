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

#include "updatesmodel.h"

#include "../sailreadsmanager.h"
#include "../objects/update.h"
#include "../objects/user.h"

namespace Sailreads
{
UpdatesModel::UpdatesModel(QObject *parent)
: BaseModel<UpdatePtr>(parent)
{
    connect(SailreadsManager::Instance(this), &SailreadsManager::gotUpdates,
            this, &UpdatesModel::handleGotUpdates);
}

bool UpdatesModel::GetHasMore() const
{
    return m_HasMore;
}

void UpdatesModel::SetHasMore(bool has)
{
    if (m_HasMore != has) {
        m_HasMore = has;
        emit hasMoreChanged();
    }
}

QString UpdatesModel::GetUpdateScope() const
{
    return m_UpdateScope;
}

void UpdatesModel::SetUpdateScope(const QString& scope)
{
    if (m_UpdateScope != scope) {
        m_UpdateScope = scope;
        emit updateScopeChanged();
    }
}

QString UpdatesModel::GetUpdateItems() const
{
    return m_UpdateItems;
}

void UpdatesModel::SetUpdateItems(const QString& items)
{
    if (m_UpdateItems != items) {
        m_UpdateItems = items;
        emit updateItemsChanged();
    }
}

QVariant UpdatesModel::data(const QModelIndex& index, int role) const
{
    if (index.row() > m_Items.count() - 1 || index.row() < 0) {
        return QVariant();
    }

    const auto& update = m_Items.at(index.row());
    switch (role) {
    case Body:
        return update->GetBody();
    case Link:
        return update->GetLink();
    case ImageUrl:
        return update->GetImageUrl();
    case Actor:
        return QVariant::fromValue(update->GetActorPtr().get());
    case UpdatedDate:
        return update->GetUpdatedDate();
    case UpdateType:
        return update->GetUpdateType();
    default:
        return QVariant();
    }
    return QVariant();
}

QHash<int, QByteArray> UpdatesModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Body] = "updateBody";
    roles[Link] = "updateLink";
    roles[ImageUrl] = "updateImageUrl";
    roles[Actor] = "updateActor";
    roles[UpdatedDate] = "updateUpdatedDate";
    roles[UpdateType] = "updateUpdateType";
    return roles;
}

void UpdatesModel::fetchMoreContent()
{
    SailreadsManager::Instance(this)->loadUpdates(m_UpdateScope, m_UpdateItems,
            m_Items.size() > 0 ? m_Items.last()->GetUpdatedDate() : QDateTime());
}

void UpdatesModel::refreshUpdates()
{
    SetItems({});
    SailreadsManager::Instance(this)->loadUpdates(m_UpdateScope, m_UpdateItems);
}

void UpdatesModel::handleGotUpdates(const Updates_t& updates)
{
    if (updates.size() > 0) {
        AddItems(updates);
    }
    else {
        SetHasMore(false);
    }
}

} // namespace Sailreads
