#include "gamesdatabase.h"
#include <QDebug>

TwitchApi::GamesDatabase::GamesDatabase(QObject *parent)
	: QObject(parent)
{
	if (!connectToDB())
		qDebug() << "TwitchApi::GamesDatabase:   Connection to Database: failed";
}

TwitchApi::GamesDatabase::~GamesDatabase()
{

}

void TwitchApi::GamesDatabase::addGames(const TwitchApi::Games * games)
{
	addGameId(games);
	addGameViewers(games);
}

QList<QString> TwitchApi::GamesDatabase::getGameListName()
{
	QSqlQuery query;
	QList<QString> result;
	query.exec("SELECT name FROM twitchapi." + table_Games + ";");
	while (query.next()) {
		result.push_back(query.value(0).toString());
	}
	
	return result;
}

QList<double> TwitchApi::GamesDatabase::getGameListId()
{
	QSqlQuery query;
	QList<double> result;
	query.exec("SELECT idgames FROM twitchapi." + table_Games + ";");
	while (query.next()) {
		result.push_back(query.value(0).toDouble());
	}
	
	return result;
}

TwitchApi::dataMap2d TwitchApi::GamesDatabase::getDataFromName(QString name)
{
	return getDataFromId(nametoId(name));
}

TwitchApi::dataMap2d TwitchApi::GamesDatabase::getDataFromId(double id)
{
	QSqlQuery query;
	QList<qreal> viewers;
	QList<QDateTime> timestamp;
	query.exec("SELECT viewers, timestamp FROM " + table_GamesViewers + " WHERE gameid = " + QString::number(id));

	while (query.next()) {
		viewers.push_back(query.value(0).toDouble());
		auto timeString = query.value(1).toString();
		auto time = QDateTime::fromString(timeString, "yyyy-MM-dd hh:mm:ss");
		timestamp.push_back(time);
	}

	return dataMap2d(timestamp, viewers);
}

bool TwitchApi::GamesDatabase::isAGame(QString name)
{
	if (this->nametoId(name) == 0)
		return false;
	else
		return true;
}

double TwitchApi::GamesDatabase::nametoId(QString name)
{
	QSqlQuery query;
	QList<double> result;
	query.exec("SELECT idgames FROM " + table_Games + " WHERE name = '" + name + "'");
	
	while (query.next()) {
		result.push_back(query.value(0).toDouble());
	}	

	if (result.size() != 0)
		return result.at(0);
	else
		return 0;
}

QString TwitchApi::GamesDatabase::idToName(double id)
{
	QSqlQuery query;
	QList<QString> result;

	query.exec("SELECT name FROM " + table_Games + " WHERE idgames  = " + QString::number(id));

	while (query.next()) {
		result.push_back(query.value(0).toString());
	}
	
	if (result.size() != 0)
		return result.at(0);
	else
		return 0;
}

bool TwitchApi::GamesDatabase::connectToDB()
{
	db = QSqlDatabase::addDatabase("QMYSQL");

	db.setHostName(hostName);
	db.setPort(port);
	db.setUserName(userName);
	db.setPassword(password);

	db.setDatabaseName(databaseName);


	if (!db.open()) {
		QMessageBox::critical(0, tr("Cannot open database ! ") + databaseName,
			tr("Unable to establish a database connection.\n"
				"Click Cancel to exit."), QMessageBox::Cancel);

		qDebug() << db.lastError();
		qDebug() << db.driver();
		qDebug() << db.drivers();
		return false;
	}
	else
		return true;
}

int TwitchApi::GamesDatabase::addGameId(const TwitchApi::Games * games)
{
	QSqlQuery query;
	qDebug() << "TwitchApi::GamesDatabase:   Finding IDs:" << query.exec("SELECT idgames FROM " + table_Games);
	QList<QVariant> usedIds = query.boundValues().values();

	int numberOfNewGames = 0;
	
	for(auto i : games->savedGames)
	{
		if (usedIds.contains(i.id))	continue;

		QSqlQuery addNewGame;
		addNewGame.prepare("INSERT INTO " + table_Games + " (idgames, name) "
			"VALUES (?, ?)");
		addNewGame.addBindValue(i.id);
		addNewGame.addBindValue(i.name);
		addNewGame.exec();

		numberOfNewGames++;
	}
	qDebug() << "number of new Games" << numberOfNewGames;
	return numberOfNewGames;
}

int TwitchApi::GamesDatabase::addGameViewers(const TwitchApi::Games * games)
{
	qDebug() << "new Data values  " <<  games->savedGames.size();
	for (auto i : games->savedGames)
	{
		QSqlQuery addNewGame;
		addNewGame.prepare("INSERT INTO " + table_GamesViewers + " (gameid, viewers, timestamp) "
			"VALUES (?, ?, ?)");
		addNewGame.addBindValue(i.id);
		addNewGame.addBindValue(i.viewers);
		addNewGame.addBindValue(games->timestamp);
		if (addNewGame.exec() == false)
			qDebug() << "TwitchApi::GamesDatabase:   addGameViewers" << addNewGame.lastError();
	}
	return 0;
}

