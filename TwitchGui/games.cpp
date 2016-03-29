#include "games.h"

#include <qdebug.h>

TwitchApi::Games::Games(int limit, int offset, QObject *parent)
	: QObject(parent)
{
	refresh(limit, offset);
}
void TwitchApi::Games::refresh(int limit, int offset)
{
	// download file
	manager = new QNetworkAccessManager();

	if (limit > 100) limit = 100;
	if (limit < 1) limit = 1;
	if (offset < 0) offset = 0;

	connect(manager, SIGNAL(finished(QNetworkReply*)),
		this, SLOT(networkingFinished(QNetworkReply*)));

	manager->get(QNetworkRequest(QUrl("https://api.twitch.tv/kraken/games/top?limit="
		+ QString::number(limit) + "?offset=" + QString::number(offset))));
}
void TwitchApi::Games::networkingFinished(QNetworkReply* Reply)
{
	//download finished: read file
	qDebug() << "network Finished";
	QString strReply = Reply->readAll();

	QJsonDocument document = QJsonDocument::fromJson(strReply.toUtf8());
	QJsonObject games_JSON = document.object();

	numberOfGames = games_JSON.value(QString("_total")).toDouble();
	timestamp = QDateTime::currentDateTime();
	qDebug() << timestamp;
	next = games_JSON.value(QString("_links")).toObject().value(QString("next")).toString();

	auto gameList = games_JSON.value(QString("top")).toArray();
	std::for_each(gameList.constBegin(), gameList.constEnd(), [this](QJsonValue& val)
	{
		TwitchApi::game game;
		game.channels = val.toObject().value(QString("channels")).toDouble();
		game.viewers = val.toObject().value(QString("viewers")).toDouble();
		game.id = val.toObject().value(QString("game")).toObject().value(QString("_id")).toDouble();
		game.name = val.toObject().value(QString("game")).toObject().value(QString("name")).toString();

		savedGames.push_back(game);
	});

	emit gameDataReady(this);
}

TwitchApi::Games::~Games()
{

}



QDebug &TwitchApi::operator<<(QDebug os, const TwitchApi::Games * obj)
{
	os << obj->timestamp.toString() << '\n';
	os << obj->numberOfGames << '\n';
	os << obj->next.toString() << '\n';
	if (obj->savedGames.size() > 0)
	std::for_each(obj->savedGames.begin(), obj->savedGames.end(), [&os](TwitchApi::game val)
	{
		os << val.id;
		os << val.name;
		os << val.viewers;
		os << val.channels << '\n';
	});
	
	return  os;
}

QDataStream &TwitchApi::operator<<(QDataStream os, const TwitchApi::Games * obj)
{
	os << obj->timestamp.toString() << '\n';
	os << obj->numberOfGames << '\n';
	os << obj->next.toString() << '\n';
	if (obj->savedGames.size() > 0)
		std::for_each(obj->savedGames.begin(), obj->savedGames.end(), [&os](TwitchApi::game val)
	{
		os << val.id;
		os << val.name;
		os << val.viewers;
		os << val.channels << '\n';
	});

	return  os;
}
