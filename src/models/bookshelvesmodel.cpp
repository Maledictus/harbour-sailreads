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

#include "bookshelvesmodel.h"

#include "../objects/user.h"
#include "../sailreadsmanager.h"

namespace Sailreads
{
BookShelvesModel::BookShelvesModel(QObject *parent)
: BaseModel<BookShelf>(parent)
, m_UserId("")
, m_HasMore(true)
, m_CurrentPage(1)
{
    auto sm = SailreadsManager::Instance();
    connect(sm, &SailreadsManager::gotUserBookShelves,
            this, &BookShelvesModel::handleGotUserBookShelves);
    connect(sm, &SailreadsManager::bookShelfAdded,
            this, &BookShelvesModel::handleBookShelfAdded);
    connect(sm, &SailreadsManager::bookShelfEdited,
            this, &BookShelvesModel::handleBookShelfEdited);
}

QVariant BookShelvesModel::data(const QModelIndex& index, int role) const
{
    if (index.row() > m_Items.count() - 1 || index.row() < 0) {
        return QVariant();
    }

    const auto& booksShelf = m_Items.at(index.row());
    switch (role) {
    case Id:
        return booksShelf.GetId();
    case Name:
        return booksShelf.GetName();
    case BooksCount:
        return booksShelf.GetBooksCount();
    case Description:
        return booksShelf.GetDescription();
    case Exclusive:
        return booksShelf.GetExclusive();
    case Featured:
        return booksShelf.GetFeatured();
    case Sortable:
        return booksShelf.GetSortable();
    case RecommendFor:
        return booksShelf.GetRecommendFor();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> BookShelvesModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Id] = "bookShelfId";
    roles[Name] = "bookShelfName";
    roles[BooksCount] = "bookShelfBooksCount";
    roles[Description] = "bookShelfDescription";
    roles[Exclusive] = "bookShelfExclusive";
    roles[Featured] = "bookShelfFeatured";
    roles[Sortable] = "bookShelfSortable";
    roles[RecommendFor] = "bookShelfRecommendFor";
    return roles;
}

QString BookShelvesModel::GetUserId() const
{
    return m_UserId;
}

void BookShelvesModel::SetUserId(const QString& id)
{
    if (m_UserId != id) {
        m_UserId = id;
        emit userIdChanged();
    }
}

bool BookShelvesModel::GetHasMore() const
{
    return m_HasMore;
}

void BookShelvesModel::SetHasMore(bool has)
{
    if (m_HasMore != has) {
        m_HasMore = has;
        emit hasMoreChanged();
    }
}

void BookShelvesModel::fetchMoreContent()
{
    SailreadsManager::Instance()->loadBookShelves(this, m_UserId, m_CurrentPage);
}

void BookShelvesModel::loadBookShelves()
{
    if (m_UserId.isEmpty()) {
        return;
    }
    SailreadsManager::Instance()->loadBookShelves(this, m_UserId);
}

void BookShelvesModel::handleGotUserBookShelves(const QString& userId,
        const CountedItems<BookShelf> bookshelves)
{
    if (m_UserId != userId) {
        return;
    }

    if (!bookshelves.m_BeginIndex && !bookshelves.m_EndIndex)
    {
        Clear();
    }
    else if (bookshelves.m_BeginIndex == 1) {
        m_CurrentPage = 1;
        SetItems(bookshelves.m_Items);
    }
    else {
        AddItems(bookshelves.m_Items);
    }

    SetHasMore(bookshelves.m_EndIndex != bookshelves.m_Count);
    if (m_HasMore) {
        ++m_CurrentPage;
    }
}

void BookShelvesModel::handleBookShelfAdded(const BookShelf& shelf)
{
    AddItems(shelf);
}

void BookShelvesModel::handleBookShelfEdited(const BookShelf& shelf)
{
    auto it = std::find_if(m_Items.begin(), m_Items.end(),
            [shelf](decltype(m_Items.front()) oldShelf)
            { return oldShelf.GetId() == shelf.GetId(); });
    if (it != m_Items.end())
    {
        int pos = std::distance(m_Items.begin(), it);
        m_Items[pos] = shelf;
        emit dataChanged(index(pos), index(pos));
    }
}
} // namespace Sailreads
