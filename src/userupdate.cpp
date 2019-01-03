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

#include "userupdate.h"

namespace Sailreads
{
UserUpdate::UserUpdate()
: m_Type(Type::NoType)
{
}

UserUpdate::Type UserUpdate::GetType() const
{
    return m_Type;
}

void UserUpdate::SetType(UserUpdate::Type type)
{
    m_Type = type;
}

QString UserUpdate::GetActionText() const
{
    return m_ActionText;
}

void UserUpdate::SetActionText(const QString& text)
{
    m_ActionText = text;
}

QUrl UserUpdate::GetUrl() const
{
    return m_Url;
}

void UserUpdate::SetUrl(const QUrl& url)
{
    m_Url = url;
}

QUrl UserUpdate::GetImageUrl() const
{
    return m_Image;
}

void UserUpdate::SetImageUrl(const QUrl& url)
{
    m_Image = url;
}

QDateTime UserUpdate::GetDate() const
{
    return m_Date;
}

void UserUpdate::SetDate(const QDateTime& date)
{
    m_Date = date;
}

UserUpdate::Actor UserUpdate::GetActor() const
{
    return m_Actor;
}

void UserUpdate::SetActor(const UserUpdate::Actor& actor)
{
    m_Actor = actor;
}

QMap<UserUpdate::ActionType, QString> UserUpdate::GetActions() const
{
    return m_Actions;
}

void UserUpdate::SetActions(const QMap<UserUpdate::ActionType, QString>& actions)
{
    m_Actions = actions;
}
} // namespace Sailreads
