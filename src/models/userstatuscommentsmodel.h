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

#pragma once

#include "basecommentsmodel.h"

namespace Sailreads
{
class UserStatusCommentsModel: public BaseCommentsModel
{
    Q_OBJECT

    QString m_UserStatusId;
    Q_PROPERTY(QString userStatusId READ GetUserStatusId WRITE SetUserStatusId NOTIFY userStatusIdChanged)

public:
    explicit UserStatusCommentsModel(QObject *parent = nullptr);

    virtual void classBegin() override;
    virtual void componentComplete() override;

    QString GetUserStatusId() const;
    void SetUserStatusId(const QString& id);

private slots:
    void handleGotUserStatus(const UserStatusPtr& userStatus);
    void handleNewCommentAdded(const QString& resourceId, const Comment& comment);
public slots:
    void fetchMoreContent();

signals:
    void userStatusIdChanged();
};

} // namespace Sailusers
