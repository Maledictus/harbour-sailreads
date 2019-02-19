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

#include "quote.h"

#include <QtDebug>

namespace Sailreads
{
Quote::Quote()
{
#ifdef QT_DEBUG
    qDebug() << this << "CONSTRUCTED";
#endif
}

Quote::~Quote()
{
#ifdef QT_DEBUG
    qDebug() << this << "DESTRUCTED";
#endif
}

QString Quote::GetQuote() const
{
    return m_Quote;
}

void Quote::SetQuote(const QString& quote)
{
    m_Quote = quote;
}

QString Quote::GetAuthor() const
{
    return m_Author;
}

void Quote::SetAuthor(const QString& author)
{
    m_Author = author;
}

QUrl Quote::GetAuthorLink() const
{
    return m_AuthorLink;
}

void Quote::SetAuthorLink(const QUrl& authorLink)
{
    m_AuthorLink = authorLink;
}

QString Quote::GetBook() const
{
    return m_Book;
}

void Quote::SetBook(const QString& book)
{
    m_Book = book;
}

QUrl Quote::GetBookLink() const
{
    return m_BookLink;
}

void Quote::SetBookLink(const QUrl& bookLink)
{
    m_BookLink = bookLink;
}

} // namespace Sailreads
