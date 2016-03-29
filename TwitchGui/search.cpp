#include "search.h"

TwitchApi::Search::Search(QObject *parent)
	: QObject(parent)
{
	refresh("Starcraft");
}

TwitchApi::Search::~Search()
{

}
void TwitchApi::Search::refresh(QString search)
{
	// download file
	manager = new QNetworkAccessManager();

	connect(manager, SIGNAL(finished(QNetworkReply*)),
		this, SLOT(networkingFinished(QNetworkReply*)));

	manager->get(QNetworkRequest(QUrl("https://api.twitch.tv/kraken/search/channels?q=" + search)));
}

void TwitchApi::Search::networkingFinished(QNetworkReply* Reply)
{
	//download finished: read file
	QString strReply = Reply->readAll();

	QJsonDocument document = QJsonDocument::fromJson(strReply.toUtf8());
	QJsonObject games_JSON = document.object();

	auto numberOfGames = games_JSON.value(QString("_total")).toDouble(); 
	qDebug() << "Number of Games:" << numberOfGames;

	QJsonArray gameList = games_JSON.value(QString("channels")).toArray();
	qDebug() << "Channel List:" ;

	std::for_each(gameList.constBegin(), gameList.constEnd(), [] (QJsonValue& val)
	{
		TwitchApi::channel chan;
		chan.followers = val.toObject().value(QString("followers")).toDouble();
		qDebug() << chan.followers;
		chan.game = val.toObject().value(QString("game")).toString();
		chan.id = val.toObject().value(QString("_id")).toDouble();
		chan.language = val.toObject().value(QString("language")).toString();
		chan.mature = val.toObject().value(QString("mature")).toBool();
		chan.name = val.toObject().value(QString("name")).toString();
		chan.partner = val.toObject().value(QString("partner")).toBool();
		chan.status = val.toObject().value(QString("status")).toString();
		chan.views = val.toObject().value(QString("views")).toDouble();
	});
}
