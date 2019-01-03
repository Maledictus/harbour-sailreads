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

#include <QObject>
#include <QDateTime>
#include <QString>
#include <QVariantList>

#include "../types.h"

namespace Sailreads
{
class Book : public QObject
{
    Q_OBJECT

    quint64 m_Id;
    QString m_ISBN;
    QString m_ISBN13;
    QString m_ASIN;
    QString m_KindleASIN;
    QString m_MarketplaceId;
    QString m_CountryCode;
    QString m_Language;
    quint64 m_TextReviewsCount;
    QString m_Title;
    QString m_TitleWithoutSeries;
    QUrl m_ImageUrl;
    QUrl m_SmallImageUrl;
    QUrl m_LargeImageUrl;
    QUrl m_Link;
    int m_NumPages;
    QString m_Format;
    QString m_EditionInformation;
    QString m_Publisher;
    int m_PublicationYear;
    int m_PublicationMonth;
    int m_PublicationDay;
    qreal m_AverageRating;
    quint64 m_RatingsCount;
    QString m_Description;
    Authors_t m_Authors;
    int m_PublishedYear;
    bool m_IsEBook;
    QString m_ReviewsWidgetContent;
    Books_t m_SimilarBooks;
    WorkPtr m_Work;
    Series_t m_SeriesList;
    ReviewPtr m_Review;
    Reviews_t m_FriendReviews;

    Q_PROPERTY(quint64 id READ GetId NOTIFY idChanged)
    Q_PROPERTY(QString isbn READ GetISBN NOTIFY isbnChanged)
    Q_PROPERTY(QString isbn13 READ GetISBN13 NOTIFY isbn13Changed)
    Q_PROPERTY(QString asin READ GetASIN NOTIFY asinChanged)
    Q_PROPERTY(QString kindleAsin READ GetKindleASIN NOTIFY kindleAsinChanged)
    Q_PROPERTY(QString marketplaceId READ GetMarketplaceId NOTIFY marketplaceIdChanged)
    Q_PROPERTY(QString countryCode READ GetCountryCode NOTIFY countryCodeChanged)
    Q_PROPERTY(QString language READ GetLanguage NOTIFY languageChanged)
    Q_PROPERTY(quint64 textReviewsCount READ GetTextReviewsCount NOTIFY textReviewsCountChanged)
    Q_PROPERTY(QString title READ GetTitle NOTIFY titleChanged)
    Q_PROPERTY(QString titleWithoutSeries READ GetTitleWithoutSeries NOTIFY titleWithoutSeriesChanged)
    Q_PROPERTY(QUrl imageUrl READ GetImageUrl NOTIFY imageUrlChanged)
    Q_PROPERTY(QUrl smallImageUrl READ GetSmallImageUrl NOTIFY smallImageUrlChanged)
    Q_PROPERTY(QUrl largeImageUrl READ GetLargeImageUrl NOTIFY largeImageUrlChanged)
    Q_PROPERTY(QUrl link READ GetLink NOTIFY linkChanged)
    Q_PROPERTY(int numPages READ GetNumPages NOTIFY numPagesChanged)
    Q_PROPERTY(QString format READ GetFormat NOTIFY formatChanged)
    Q_PROPERTY(QString editionInformation READ GetEditionInformation NOTIFY editionInformationChanged)
    Q_PROPERTY(QString publisher READ GetPublisher NOTIFY publisherChanged)
    Q_PROPERTY(int publicationYear READ GetPublicationYear NOTIFY publicationYearChanged)
    Q_PROPERTY(int publicationMonth READ GetPublicationMonth NOTIFY publicationMonthChanged)
    Q_PROPERTY(int publicationDay READ GetPublicationDay NOTIFY publicationDayChanged)
    Q_PROPERTY(qreal averageRating READ GetAverageRating NOTIFY averageRatingChanged)
    Q_PROPERTY(quint64 ratingsCount READ GetRatingsCount NOTIFY ratingsCountChanged)
    Q_PROPERTY(QString description READ GetDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QObjectList authors READ GetAuthors NOTIFY authorsChanged)
    Q_PROPERTY(QString authorsString READ GetAuthorsString NOTIFY authorsStringChanged)
    Q_PROPERTY(int publishedYear READ GetPublishedYear NOTIFY publishedYearChanged)
    Q_PROPERTY(bool isEBook READ GetIsEBook NOTIFY isEBookChanged)
    Q_PROPERTY(QString reviewsWidgetContent READ GetReviewsWidgetContent NOTIFY reviewsWidgetContentChanged)
    Q_PROPERTY(Work* work READ GetWork NOTIFY workChanged)
    Q_PROPERTY(QObjectList similarBooks READ GetSimilarBooks NOTIFY  similarBooksChanged)
    Q_PROPERTY(QObjectList seriesList READ GetSeriesList NOTIFY seriesListChanged)
    Q_PROPERTY(Review* review READ GetReview NOTIFY reviewChanged)
    Q_PROPERTY(QObjectList friendReviews READ GetFriendReviewsList NOTIFY friendReviewsChanged)

public:
    Book(QObject *parent = nullptr);
    ~Book();

    quint64 GetId() const;
    void SetId(quint64 id);
    QString GetISBN() const;
    void SetISBN(const QString& isbn);
    QString GetISBN13() const;
    void SetISBN13(const QString& isbn13);
    QString GetASIN() const;
    void SetASIN(const QString& asin);
    QString GetKindleASIN() const;
    void SetKindleASIN(const QString& kindleAsin);
    QString GetMarketplaceId() const;
    void SetMarketplaceId(const QString& marketplaceId);
    QString GetCountryCode() const;
    void SetCountryCode(const QString& countryCode);
    QString GetLanguage() const;
    void SetLanguage(const QString& lang);
    quint64 GetTextReviewsCount() const;
    void SetTextReviewsCount(const quint64& textReviewsCount);
    QString GetTitle() const;
    void SetTitle(const QString& title);
    QString GetTitleWithoutSeries() const;
    void SetTitleWithoutSeries(const QString& titleWithoutSeries);
    QUrl GetImageUrl() const;
    void SetImageUrl(const QUrl& imageUrl);
    QUrl GetSmallImageUrl() const;
    void SetSmallImageUrl(const QUrl& smallImageUrl);
    QUrl GetLargeImageUrl() const;
    void SetLargeImageUrl(const QUrl& largeImageUrl);
    QUrl GetLink() const;
    void SetLink(const QUrl& link);
    int GetNumPages() const;
    void SetNumPages(int numPages);
    QString GetFormat() const;
    void SetFormat(const QString& format);
    QString GetEditionInformation() const;
    void SetEditionInformation(const QString& info);
    QString GetPublisher() const;
    void SetPublisher(const QString& publisher);
    int GetPublicationYear() const;
    void SetPublicationYear(int publicationYear);
    int GetPublicationMonth() const;
    void SetPublicationMonth(int publicationMonth);
    int GetPublicationDay() const;
    void SetPublicationDay(int publicationDay);
    qreal GetAverageRating() const;
    void SetAverageRating(const qreal& averageRating);
    quint64 GetRatingsCount() const;
    void SetRatingsCount(const quint64& ratingsCount);
    QString GetDescription() const;
    void SetDescription(const QString& description);
    QObjectList GetAuthors() const;
    Authors_t GetAuthorsPtr() const;
    void SetAuthors(const Authors_t& authors);
    QString GetAuthorsString() const;
    int GetPublishedYear() const;
    void SetPublishedYear(int publishedYear);
    bool GetIsEBook() const;
    void SetIsEBook(bool ebook);
    QString GetReviewsWidgetContent() const;
    void SetReviewsWidgetContent(const QString& content);
    QObjectList GetSimilarBooks() const;
    Books_t GetSimilarBooksPtr() const;
    void SetSimilarBooks(const Books_t& books);
    Work* GetWork() const;
    WorkPtr GetWorkPtr() const;
    void SetWork(const WorkPtr& work);
    QObjectList GetSeriesList() const;
    Series_t GetSeriesListPtr() const;
    void SetSeriesList(const Series_t& seriesList);
    Review* GetReview() const;
    ReviewPtr GetReviewPtr() const;
    void SetReview(const ReviewPtr& review);
    QObjectList GetFriendReviewsList() const;
    Reviews_t GetFriendReviewsPtr() const;
    void SetFriendReviews(const Reviews_t& reviews);

    void Update(Book *newBook);
    void Update(const BookPtr& newBook);

signals:
    void idChanged();
    void isbnChanged();
    void isbn13Changed();
    void asinChanged();
    void kindleAsinChanged();
    void marketplaceIdChanged();
    void countryCodeChanged();
    void languageChanged();
    void textReviewsCountChanged();
    void titleChanged();
    void titleWithoutSeriesChanged();
    void imageUrlChanged();
    void smallImageUrlChanged();
    void largeImageUrlChanged();
    void linkChanged();
    void numPagesChanged();
    void formatChanged();
    void editionInformationChanged();
    void publisherChanged();
    void publicationYearChanged();
    void publicationMonthChanged();
    void publicationDayChanged();
    void averageRatingChanged();
    void ratingsCountChanged();
    void descriptionChanged();
    void authorsChanged();
    void authorsStringChanged();
    void publishedYearChanged();
    void isEBookChanged();
    void reviewsWidgetContentChanged();
    void workChanged();
    void similarBooksChanged();
    void seriesListChanged();
    void reviewChanged();
    void friendReviewsChanged();
};
} // namespace Sailreads
