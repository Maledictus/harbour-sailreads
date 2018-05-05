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

#pragma once

#include <QString>
#include <QObject>

namespace Sailreads
{
class Series : public QObject
{
    Q_OBJECT

    quint64 m_Id;
    QString m_Title;
    QString m_Description;
    QString m_Note;
    int m_SeriesWorksCount;
    int m_PrimaryWorkCount;
    bool m_Numbered;

    Q_PROPERTY(quint64 id READ GetId NOTIFY idChanged)
    Q_PROPERTY(QString title READ GetTitle NOTIFY titleChanged)
    Q_PROPERTY(QString description READ GetDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString note READ GetNote NOTIFY noteChanged)
    Q_PROPERTY(int seriesWorksCount READ GetSeriesWorksCount NOTIFY seriesWorksCountChanged)
    Q_PROPERTY(int primaryWorkCount READ GetPrimaryWorkCount NOTIFY primaryWorkCountChanged)
    Q_PROPERTY(bool numbered READ GetNumbered NOTIFY numberedChanged)

public:
    Series(QObject *parent = nullptr);

    quint64 GetId() const;
    void SetId(quint64 id);
    QString GetTitle() const;
    void SetTitle(const QString& title);
    QString GetDescription() const;
    void SetDescription(const QString& description);
    QString GetNote() const;
    void SetNote(const QString& note);
    int GetSeriesWorksCount() const;
    void SetSeriesWorksCount(int seriesWorksCount);
    int GetPrimaryWorkCount() const;
    void SetPrimaryWorkCount(int primaryWorkCount);
    bool GetNumbered() const;
    void SetNumbered(bool numbered);

signals:
    void idChanged();
    void titleChanged();
    void descriptionChanged();
    void noteChanged();
    void seriesWorksCountChanged();
    void primaryWorkCountChanged();
    void numberedChanged();
};
} // namespace Sailreads
