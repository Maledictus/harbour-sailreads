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

#include "curlwrapper.h"
#include <sstream>
#include <QDateTime>
#include <QtDebug>
#include <curl/curl.h>

namespace SailReads
{
	CurlWrapper::CurlWrapper (QObject *parent)
	: QObject (parent)
	{
	}

	QByteArray CurlWrapper::Post (const QUrl& url, const QByteArray& data)
	{
		curl_global_init (CURL_GLOBAL_ALL);
		CURL *curl = curl_easy_init ();
		QByteArray result;
		if (curl)
		{
			std::stringstream response;

			curl_easy_setopt (curl, CURLOPT_URL, url.toEncoded ().data ());
			curl_easy_setopt (curl, CURLOPT_POSTFIELDS, data.data ());
			curl_easy_setopt (curl, CURLOPT_HEADER, 0L);
			curl_easy_setopt (curl, CURLOPT_WRITEFUNCTION, CurlWrapper::WriteCallback);
			curl_easy_setopt (curl, CURLOPT_WRITEDATA, &response);

			CURLcode res = curl_easy_perform (curl);
			if (res != CURLE_OK)
				qDebug () << "curl_easy_perform() failed:"
					<< curl_easy_strerror (res);
			else
				result = response.str ().c_str ();

			curl_easy_cleanup (curl);
		}
		curl_global_cleanup ();

		return result;
	}

	QByteArray CurlWrapper::Put (const QUrl& url, const QByteArray& data)
	{
		curl_global_init (CURL_GLOBAL_ALL);
		CURL *curl = curl_easy_init ();
		QByteArray result;
		if (curl)
		{
			std::stringstream response;

			curl_easy_setopt (curl, CURLOPT_URL, url.toEncoded ().data ());
			curl_easy_setopt (curl, CURLOPT_POSTFIELDS, data.data ());
			curl_easy_setopt (curl, CURLOPT_CUSTOMREQUEST, "PUT");
			curl_easy_setopt (curl, CURLOPT_HEADER, 0L);
			curl_easy_setopt (curl, CURLOPT_WRITEFUNCTION, CurlWrapper::WriteCallback);
			curl_easy_setopt (curl, CURLOPT_WRITEDATA, &response);

			CURLcode res = curl_easy_perform (curl);
			if (res != CURLE_OK)
				qDebug () << "curl_easy_perform() failed:"
					<< curl_easy_strerror (res);
			else
				result = response.str ().c_str ();

			curl_easy_cleanup (curl);
		}
		curl_global_cleanup ();

		return result;
	}

	size_t CurlWrapper::WriteCallback (void *ptr, size_t size, size_t nmemb,
			void *data)
	{
		std::string buf = std::string (static_cast<char*> (ptr), size * nmemb);
		std::stringstream *response = static_cast<std::stringstream*> (data);
		response->write (buf.c_str (), static_cast<std::streamsize> (buf.size ()));
		return size * nmemb;
	}
}
