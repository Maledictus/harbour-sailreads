/*
Copyright (c) 2018 Oleg Linkin <maledictusdemagog@gmail.com>

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

#include "bookshelvesmodel.h"

namespace Sailreads
{
BookShelvesModel::BookShelvesModel(QObject *parent)
: BaseModel<BookShelf>(parent)
{
}

BookShelvesModel::~BookShelvesModel()
{
}

QVariant BookShelvesModel::data(const QModelIndex& index, int role) const
{
    if (index.row() > m_Items.count() - 1 || index.row() < 0) {
        return QVariant ();
    }

    const auto& booksShelf = m_Items.at(index.row());
    switch (role) {
    case BSRId:
        return booksShelf.GetId();
    case BSRName:
        return booksShelf.GetName();
    case BSRBooksCount:
        return booksShelf.GetBooksCount();
    case BSRDescription:
        return booksShelf.GetDescription();
    case BSRExclusive:
        return booksShelf.GetExclusive();
    case BSRFeatured:
        return booksShelf.GetFeatured();
    case BSRSortable:
        return booksShelf.GetSortable();
    default:
        return QVariant ();
    }
}

QHash<int, QByteArray> BookShelvesModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[BSRId] = "bookShelfId";
    roles[BSRName] = "bookShelfName";
    roles[BSRBooksCount] = "bookShelfBooksCount";
    roles[BSRDescription] = "bookShelfDescription";
    roles[BSRExclusive] = "bookShelfExclusive";
    roles[BSRFeatured] = "bookShelfFeatured";
    roles[BSRSortable] = "bookShelfSortable";
    return roles;
}

QVariantMap BookShelvesModel::get(int index) const
{
    QVariantMap map;
    if (index > m_Items.count() - 1 || index < 0) {
        return map;
    }
    const auto& booksShelf = m_Items.at(index);
    map["bookShelfId"] = booksShelf.GetId();
    map["bookShelfName"] = booksShelf.GetName();
    map["bookShelfBooksCount"] = booksShelf.GetBooksCount();
    map["bookShelfDescription"] = booksShelf.GetDescription();
    map["bookShelfExclusive"] = booksShelf.GetExclusive();
    map["bookShelfFeatured"] = booksShelf.GetFeatured();
    map["bookShelfSortable"] = booksShelf.GetSortable();
    return map;
}

} // namespace Sailreads
