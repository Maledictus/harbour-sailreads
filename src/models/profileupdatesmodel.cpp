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
#include "profileupdatesmodel.h"

#include "../sailreadsmanager.h"

namespace Sailreads
{
ProfileUpdatesModel::ProfileUpdatesModel(QObject *parent)
: UpdatesModel(parent)
{
}

quint64 ProfileUpdatesModel::GetUserId() const
{
    return m_UserId;
}

void ProfileUpdatesModel::SetUserId(quint64 userId)
{
    if (m_UserId != userId) {
        m_UserId = userId;
        emit userIdChanged();
    }
}

void ProfileUpdatesModel::classBegin()
{
    connect(SailreadsManager::Instance(this), &SailreadsManager::gotUserUpdates,
            this, &ProfileUpdatesModel::handleGotUpdates);
}

void ProfileUpdatesModel::componentComplete()
{
}

void ProfileUpdatesModel::handleGotUpdates(quint64 userId, const Updates_t& updates)
{
    if (m_UserId != userId) {
        return;
    }

    SetItems(updates);
    SetHasMore(false);
}

} // namespace Sailreads
