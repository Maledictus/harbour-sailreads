/*
Copyright (c) 2014 Oleg Linkin <maledictusdemagog@gmail.com>

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

#include "localstorage.h"
#include <stdexcept>
#include <QCoreApplication>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QtDebug>
#include "util.h"

namespace SailReads
{
	LocalStorage::LocalStorage (QObject *parent)
	: QObject (parent)
	, SailreadsDB_ (QSqlDatabase::addDatabase ("QSQLITE", "sailreads"))
	{
		SailreadsDB_.setDatabaseName (Util::CreateIfNotExists (QStandardPaths::writableLocation (QStandardPaths::DataLocation))
				.filePath (qApp->applicationName () + ".db"));

		if (!SailreadsDB_.open ())
		{
			qWarning () << Q_FUNC_INFO
					<< "unable to open the database";
			throw std::runtime_error ("unable to open Sailreads database");
		}

		{
			QSqlQuery query (SailreadsDB_);
			query.exec ("PRAGMA foreign_keys = ON;");
			query.exec ("PRAGMA synchronous = OFF;");
		}

		try
		{
			CreateTables ();
		}
		catch (const std::runtime_error& e)
		{
			qWarning () << Q_FUNC_INFO
					<< e.what ();
		}
	}

	void LocalStorage::SetValue (const QString& key, const QString& value)
	{
		QSqlQuery query (SailreadsDB_);
		query.prepare ("INSERT OR REPLACE INTO keys (Key, Value) VALUES(:key, :value);");
		query.bindValue (":key", key);
		query.bindValue (":value", value);

		if (!query.exec ())
			throw std::runtime_error ("unable to set value");
	}

	QString LocalStorage::GetValue (const QString& key)
	{
		QSqlQuery query (SailreadsDB_);
		query.prepare ("SELECT * FROM keys WHERE Key=:key;");
		query.bindValue (":key", key);
		if (!query.exec ())
			throw std::runtime_error ("unable to get value");

		if (query.next ())
			return query.value (1).toString ();

		return QString ();
	}

	void LocalStorage::CreateTables ()
	{
		QMap<QString, QString> table2query;
		table2query ["keys"] = "CREATE TABLE IF NOT EXISTS keys ("
				"Key TEXT PRIMARY KEY,"
				"Value TEXT);";

		const auto& tables = SailreadsDB_.tables ();
		Q_FOREACH (const QString& key, table2query.keys ())
		if (!tables.contains (key))
		{
			QSqlQuery q (SailreadsDB_);
			if (!q.exec (table2query [key]))
				throw std::runtime_error ("cannot create required tables");
		}
	}
}
