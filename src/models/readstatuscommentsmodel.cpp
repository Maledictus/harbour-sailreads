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
#include "readstatuscommentsmodel.h"

#include "../objects/readstatus.h"
#include "../sailreadsmanager.h"

namespace Sailreads
{
ReadStatusCommentsModel::ReadStatusCommentsModel(QObject *parent)
: BaseCommentsModel(parent)
{
}

void ReadStatusCommentsModel::classBegin()
{
    auto sm = SailreadsManager::Instance();
    connect(sm, &SailreadsManager::gotReadStatus,
            this, &ReadStatusCommentsModel::handleGotReadStatus);
    connect(sm, &SailreadsManager::newCommentAdded,
            this, &ReadStatusCommentsModel::handleNewCommentAdded);
}

void ReadStatusCommentsModel::componentComplete()
{
}

QString ReadStatusCommentsModel::GetReadStatusId() const
{
    return m_ReadStatusId;
}

void ReadStatusCommentsModel::SetReadStatusId(const QString& id)
{
    if (m_ReadStatusId != id) {
        m_ReadStatusId = id;
        readStatusIdChanged();
    }
}

void ReadStatusCommentsModel::fetchMoreContent()
{
    if (!rowCount()) {
        return;
    }

    SailreadsManager::Instance()->loadReadStatus(this, m_ReadStatusId, m_CurrentPage);
}

void ReadStatusCommentsModel::handleGotReadStatus(const ReadStatusPtr& readStatus)
{
    if (!readStatus || readStatus->GetId() != m_ReadStatusId) {
        return;
    }
    handleGotComments(readStatus->GetComments());
}

void ReadStatusCommentsModel::handleNewCommentAdded(const QString& resourceId, const Comment& comment)
{
    if (resourceId != m_ReadStatusId) {
        return;
    }
    if (!m_HasMore) {
        AddItems(comment);
    }
}
} // namespace Sailreads
