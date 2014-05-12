/*
    Copyright (c) 2014 Oleg Linkin <MaledictusDeMagog@gmail.com>

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

import QtQuick 2.0
import Sailfish.Silica 1.0

ApplicationWindow
{
    id: mainWindow

    initialPage: friendsUpdatesPage

    signal applicationAuthorized (bool authorized)
    signal refreshUpdates ()
    signal requestUserProfile (string id)
    signal requestNotifications ()
    signal requestFriendsList (string id)
    signal requestGroupsList (string id)
    signal requestShelvesList (string id)
    signal requestBooksList (string id, string shelfId)
    signal requestAddBooksShelf (string name, bool exclusive)
    signal requestEditBooksShelf (string id, string name, bool exclusive)

    AuthManager {
        id: authManager
        anchors.fill: parent

        onAuthorized: applicationAuthorized (success)
    }

    function authorizeApplication(url) {
        authManager.authorizeApplication(url)
    }

    function setUserProfile (profile) {
        profilePage.uid = profile.id
        profilePage.profilePhotoUrl = profile.userProfileUrl
        profilePage.userName = profile.name
        profilePage.userDetails = profile.age + ', ' + profile.gender + ', ' +
                profile.location
        profilePage.userInterests = profile.interests
        profilePage.friendsCount = profile.friendsCount
        profilePage.groupsCount = profile.groupsCount
        profilePage.booksCount = profile.booksCount
        profilePage.privateProfile = profile.privateProfile
    }

    function setUnreadNotificationsCount (count) {
        if (pageStack.currentPage == profilePage &&
                profilePage.self)
            profilePage.notificationsCount = count;
    }

    FriendsUpdatesPage {
        id: friendsUpdatesPage
        loading: manager.requestInProcess

        onRefreshUpdates: mainWindow.refreshUpdates ()
        onSwitchToMyProfile: {
            profilePage.self = true
            pageStack.replace (profilePage)
            requestNotifications ()
        }
    }

    ProfilePage {
        id: profilePage

        onSwitchToNotifications: pageStack.push (notificationsPage)

        onSwitchToMyProfile: {
            profilePage.self = true
            requestUserProfile ("self")
        }

        onSwitchToFriends: {
            pageStack.push (friendsPage)
            friendsPage.uid = self ? "self" : uid
            requestFriendsList (self ? "self" : uid)
        }

        onSwitchToRecentUpdates: {
            pageStack.replace (friendsUpdatesPage)
            refreshUpdates ()
        }

        onSwitchToGroups: {
            pageStack.push (groupsPage)
            groupsPage.uid = self ? "self" : uid
            requestGroupsList (self ? "self" : uid)
        }

        onSwitchToShelves: {
            pageStack.push (shelvesPage)
            shelvesPage.uid = self ? "self" : uid
            //requestShelvesList (self ? "self" : uid)
        }

        onStatusChanged: {
            if (status === PageStatus.Activating) {
                requestUserProfile (self ? "self" : uid)
            }
        }
    }

    NotificationsPage {
        id: notificationsPage
        onRefreshNotifications: requestNotifications ()
    }

    FriendsPage {
        id: friendsPage

        onSwitchToMyProfile: {
            profilePage.self = true
            pageStack.replace (profilePage)
        }

        onShowUserProfile: {
            profilePage.self = false
            profilePage.uid = uid
            pageStack.pop (profilePage)
        }

        onRefreshFriends: {
            requestFriendsList (uid)
        }
    }

    GroupsPage {
        id: groupsPage

        onSwitchToMyProfile: {
            profilePage.self = true
            pageStack.replace (profilePage)
        }

        onRefreshGroups: {
            requestGroupsList (uid)
        }
    }

    ShelvesPage {
        id: shelvesPage

        onSwitchToAddEditShelf: {
            addEditShelfPage.shelfId = shelfId;
            addEditShelfPage.shelfName = name;
            addEditShelfPage.exclusive = exclusive
            pageStack.push (addEditShelfPage)
        }

        onRefreshShelves:
            requestShelvesList (uid)
    }

    AddEditShelfPage {
        id: addEditShelfPage

        onAccepted: {
            if (shelfId !== "")
                requestEditBooksShelf (shelfId, shelfName, exclusive)
            else
                requestAddBooksShelf (shelfName, exclusive)
        }
    }
}
