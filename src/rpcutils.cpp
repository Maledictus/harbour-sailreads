/*
The MIT License(MIT)

Copyright (c) 2018 Oleg Linkin <maledictusdemagog@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
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

#include "rpcutils.h"

#include <QtDebug>

namespace Sailreads
{
namespace RpcUtils
{
namespace Parser
{
UserUpdate::Actor ParseActor(const QDomElement& element)
{
    UserUpdate::Actor actor;
    const auto& fieldsList = element.childNodes();
    for (int i = 0, fieldsCount = fieldsList.size(); i < fieldsCount; ++i) {
        const auto& fieldElement = fieldsList.at (i).toElement ();
        if (fieldElement.tagName() == "id") {
            actor.m_Id = fieldElement.text().toULongLong();
        }
        else if (fieldElement.tagName() == "name") {
            actor.m_Name = fieldElement.text();
        }
        else if (fieldElement.tagName() == "image_url") {
            actor.m_AvatarUrl = QUrl(fieldElement.text());
        }
        else if (fieldElement.tagName() == "link") {
            actor.m_ProfileUrl = QUrl(fieldElement.text());
        }
    }

    return actor;
}

UserUpdate ParseUserUpdate(const QDomElement&)
{
    //TODO
    return UserUpdate();
}


UserUpdate::Type GetType(const QString& typeStr)
{
    if (typeStr == "readstatus") {
        return UserUpdate::Type::ReadStatus;
    }

    if (typeStr == "userstatus") {
        return UserUpdate::Type::UserStatus;
    }

    if (typeStr == "review") {
        return UserUpdate::Type::Review;
    }

    if (typeStr == "userchallenge") {
        return UserUpdate::Type::UserChallenge;
    }

    return UserUpdate::Type::NoType;
}

UserUpdate::ActionType GetActionType(const QString& typeStr)
{
    if (typeStr == "rating") {
        return UserUpdate::ActionType::Rating;
    }

    return UserUpdate::ActionType::NoType;
}

BookShelf ParseBookShelf(const QDomElement& element)
{
    BookShelf shelf;
    const auto& fieldsList = element.childNodes();
    for (int i = 0, fieldsCount = fieldsList.size(); i < fieldsCount; ++i) {
        const auto& fieldElement = fieldsList.at (i).toElement ();
        if (fieldElement.tagName() == "id") {
            shelf.SetId(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "name") {
            shelf.SetName(fieldElement.text());
        }
        else if (fieldElement.tagName() == "book_count") {
            shelf.SetBooksCount(fieldElement.text().toUInt());
        }
        else if (fieldElement.tagName() == "description") {
            shelf.SetDescription(fieldElement.text());
        }
        else if (fieldElement.tagName() == "exclusive_flag") {
            shelf.SetExclusive(fieldElement.text() == "true");
        }
        else if (fieldElement.tagName() == "featured") {
            shelf.SetFeatured(fieldElement.text() == "true");
        }
        else if (fieldElement.tagName() == "sort") {
            shelf.SetSortable(fieldElement.text() == "true");
        }
    }

    return shelf;
}

FavoriteAuthors_t ParseFavoriteAuthors(const QDomElement& faElement)
{
    FavoriteAuthors_t auths;
    const auto& nodes = faElement.childNodes();
    for (int i = 0, count = nodes.size(); i < count; ++i) {
        const auto& elem = nodes.at(i).toElement();
        const auto& ids = elem.elementsByTagName("id");
        const auto& names = elem.elementsByTagName("name");
        if (!ids.isEmpty() && !names.isEmpty()) {
            auths.push_back({ ids.at(0).toElement().text().toULongLong(),
                    names.at(0).toElement().text() });
        }
    }
    return auths;
}

UserUpdates_t ParseUserUpdates(const QDomElement& element)
{
    UserUpdates_t updates;
    const auto& nodes = element.childNodes();
    for (int i = 0, count = nodes.size(); i < count; ++i) {
        const auto& elem = nodes.at(i).toElement();
        updates << ParseUserUpdate(elem);
    }
    return updates;
}

BookShelves_t ParseBookShelves(const QDomElement& element)
{
    BookShelves_t shelves;
    const auto& nodes = element.childNodes();
    for (int i = 0, count = nodes.size(); i < count; ++i) {
        const auto& elem = nodes.at(i).toElement();
        shelves << ParseBookShelf(elem);
    }
    return shelves;
}

std::shared_ptr<UserProfile> ParseUserProfile(const QDomDocument &doc)
{
    std::shared_ptr<UserProfile> profile;
    const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
    if (responseElement.isNull()) {
        return profile;
    }

    profile.reset(new UserProfile());
    const auto& userElement = responseElement.firstChildElement("user");
    const auto& fieldsList = userElement.childNodes();
    for (int i = 0, count = fieldsList.size(); i < count; ++i) {
        const auto& fieldElement = fieldsList.at(i).toElement();
        if (fieldElement.tagName() == "id") {
            profile->SetUserID(fieldElement.text().toULongLong());
        }
        else if (fieldElement.tagName() == "name") {
            profile->SetUserName(fieldElement.text());
        }
        else if (fieldElement.tagName() == "user_name") {
            profile->SetNickName(fieldElement.text());
        }
        else if (fieldElement.tagName() == "link") {
            profile->SetWebUrl(QUrl(fieldElement.text()));
        }
        else if (fieldElement.tagName() == "image_url") {
            profile->SetAvatarUrl(QUrl(fieldElement.text()));
        }
        else if (fieldElement.tagName() == "small_image_url") {
            profile->SetSmallAvatarUrl(QUrl(fieldElement.text()));
        }
        else if (fieldElement.tagName() == "about") {
            profile->SetAbout(fieldElement.text());
        }
        else if (fieldElement.tagName() == "age") {
            profile->SetAge(fieldElement.text().toUInt());
        }
        else if (fieldElement.tagName() == "gender") {
            profile->SetGender(fieldElement.text());
        }
        else if (fieldElement.tagName() == "location") {
            profile->SetLocation(fieldElement.text());
        }
        else if (fieldElement.tagName() == "website") {
            profile->SetWebSite(QUrl(fieldElement.text()));
        }
        else if (fieldElement.tagName() == "joined") {
            profile->SetJoinedDate(fieldElement.text());
        }
        else if (fieldElement.tagName() == "last_active") {
            profile->SetLastUpdateDate(fieldElement.text());
        }
        else if (fieldElement.tagName() == "interests") {
            profile->SetInterests(fieldElement.text ());
        }
        else if (fieldElement.tagName() == "favorite_books") {
            profile->SetFavoriteBooksDesc(fieldElement.text());
        }
        else if (fieldElement.tagName() == "favorite_authors") {
            profile->SetFavoriteAuthors(ParseFavoriteAuthors(fieldElement));
        }
        else if (fieldElement.tagName() == "updates_rss_url") {
            profile->SetUpdateRSSUrl(QUrl(fieldElement.text()));
        }
        else if (fieldElement.tagName() == "reviews_rss_url") {
            profile->SetReviewRSSUrl(QUrl(fieldElement.text()));
        }
        else if (fieldElement.tagName() == "friends_count") {
            profile->SetFriendsCount(fieldElement.text().toUInt());
        }
        else if (fieldElement.tagName() == "groups_count") {
            profile->SetGroupsCount(fieldElement.text().toUInt());
        }
        else if (fieldElement.tagName() == "reviews_count") {
            profile->SetBooksCount(fieldElement.text().toUInt());
        }
        else if (fieldElement.tagName() == "private") {
            profile->SetPrivate(fieldElement.text() == "true");
        }
        else if (fieldElement.tagName() == "user_shelves") {
            profile->SetBookShelves(ParseBookShelves(fieldElement));
        }
        //TODO updates later
//        else if (fieldElement.tagName() == "updates") {
//            profile->SetUserUpdates(ParseUserUpdates(fieldElement));
//        }
     }

    return profile;
}

BookShelves_t ParseBookShelves(const QDomDocument& doc)
{
    const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
    if (responseElement.isNull()) {
        return BookShelves_t();
    }

    return ParseBookShelves(responseElement.firstChildElement("shelves"));
}
}
}
}
