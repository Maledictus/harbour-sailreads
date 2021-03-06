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
#include "readstatusitem.h"

#include "../objects/readstatus.h"
#include "../sailreadsmanager.h"

namespace Sailreads
{
ReadStatusItem::ReadStatusItem(QObject *parent)
: ItemRequestCanceler(parent)
{
    connect(SailreadsManager::Instance(this), &SailreadsManager::gotReadStatus,
            this, &ReadStatusItem::handleGotReadStatus);
    connect(SailreadsManager::Instance(this), &SailreadsManager::likeAdded,
            this, &ReadStatusItem::handleLikeAdded);
    connect(SailreadsManager::Instance(this), &SailreadsManager::likeRemoved,
            this, &ReadStatusItem::handleLikeRemoved);
}

QString ReadStatusItem::GetReadStatusId() const
{
    return m_ReadStatusId;
}

void ReadStatusItem::SetReadStatusId(const QString& readStatusId)
{
    if(m_ReadStatusId != readStatusId) {
        m_ReadStatusId = readStatusId;
        loadReadStatus(true);
        emit readStatusIdChanged();
    }
}

ReadStatus* ReadStatusItem::GetReadStatus() const
{
    return m_ReadStatus.get();
}

void ReadStatusItem::SetReadStatus(const ReadStatusPtr& readStatus)
{
    m_ReadStatus = readStatus;
    emit readStatusChanged();
}

void ReadStatusItem::handleGotReadStatus(const ReadStatusPtr& readStatus)
{
    if (!readStatus || m_ReadStatusId != readStatus->GetId()) {
        return;
    }
    SetReadStatus(readStatus);
}

void ReadStatusItem::handleLikeAdded(const QString& resourceId, quint64 ratingId)
{
    if (m_ReadStatus && resourceId != m_ReadStatusId) {
        return;
    }

    m_ReadStatus->SetRatingId(ratingId);
    m_ReadStatus->SetLikesCount(m_ReadStatus->GetLikesCount() + 1);
    emit readStatusChanged();
}

void ReadStatusItem::handleLikeRemoved(const QString& resourceId)
{
    if (m_ReadStatus && resourceId != m_ReadStatusId) {
        return;
    }

    m_ReadStatus->SetRatingId(0);
    m_ReadStatus->SetLikesCount(m_ReadStatus->GetLikesCount() - 1);
    emit readStatusChanged();
}

void ReadStatusItem::loadReadStatus(bool useCache)
{
    SailreadsManager::Instance(this)->loadReadStatus(this, m_ReadStatusId, useCache);
}

} // namespace Sailreads
