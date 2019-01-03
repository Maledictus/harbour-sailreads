/*
The MIT License (MIT)

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

#include "accountsettings.h"

#include <mlite5/MDConfGroup>

namespace Sailreads
{
AccountSettings::AccountSettings(QObject *parent)
: QObject(parent)
, m_AccountGroup(new MDConfGroup("/apps/sailreads/account", this))
{
}

AccountSettings* AccountSettings::Instance(QObject *parent)
{
    static AccountSettings *accSettings = nullptr;
    if (!accSettings)
    {
        accSettings = new AccountSettings(parent);
    }
    return accSettings;
}

QVariant AccountSettings::value(const QString& key, const QVariant& def) const
{
    return m_AccountGroup ? m_AccountGroup->value(key, def) : def;
}

void AccountSettings::setValue(const QString& key, const QVariant& value)
{
    if (m_AccountGroup)
    {
        m_AccountGroup->setValue(key, value);
    }
}

void AccountSettings::removeAll()
{
    if (m_AccountGroup)
    {
        m_AccountGroup->clear();
    }
}
} // namespace Sailreads

