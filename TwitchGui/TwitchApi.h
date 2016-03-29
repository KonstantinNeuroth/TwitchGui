#pragma once

#include <QObject>
#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonValue>
#include <QDateTime>
#include <QDataStream>
#include <QVector>


namespace TwitchApi
{
	typedef std::pair<QList<QDateTime>, QList<qreal>> dataMap2d;
	struct channel
	{
		bool mature, partner;
		QString status, language, name,  game;
		double id, views, followers;
	};

	struct game
	{
		QString name;
		double id, viewers, channels;
	};


};


