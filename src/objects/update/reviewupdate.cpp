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

#include "reviewupdate.h"

#include "../book.h"

namespace Sailreads
{
ReviewUpdate::ReviewUpdate(QObject *parent)
: QObject(parent)
, m_Rating(0)
{
}

QString ReviewUpdate::GetBody() const
{
    return m_Body;
}

void ReviewUpdate::SetBody(const QString& body)
{
    if (m_Body != body) {
        m_Body = body;
        emit bodyChanged();
    }
}

BookPtr ReviewUpdate::GetBookPtr() const
{
    return m_Book;
}

Book* ReviewUpdate::GetBook() const
{
    return m_Book.get();
}

void ReviewUpdate::SetBook(const BookPtr& book)
{
    if (m_Book != book) {
        m_Book = book;
        emit bookChanged();
    }
}

int ReviewUpdate::GetRating() const
{
    return m_Rating;
}

void ReviewUpdate::SetRating(int rating)
{
    if (m_Rating != rating) {
        m_Rating = rating;
        emit ratingChanged();
    }
}

} // namespace Sailreads
