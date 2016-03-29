#ifndef GAMESDATABASE_H
#define GAMESDATABASE_H

#include <QObject>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

#include "games.h"
#include "TwitchApi.h"


namespace TwitchApi
{
	class GamesDatabase : public QObject
	{
		Q_OBJECT

	public:
		GamesDatabase(QObject *parent = 0);
		~GamesDatabase();

		void addGames(const TwitchApi::Games * games);
		QList<double> getGameListId();
		QList<QString> getGameListName();
		TwitchApi::dataMap2d getDataFromName(QString name);
		TwitchApi::dataMap2d getDataFromId(double id);

		bool isAGame(QString name);
		bool isAnId(double id);

		double nametoId(QString name);
		QString idToName(double id);

	private:
		bool connectToDB();
		int addGameId(const TwitchApi::Games * games);
		int addGameViewers(const TwitchApi::Games * games);

		QSqlDatabase db;
		QString hostName = "127.0.0.1";
		int port = 3461;
		QString userName = "qt";
		QString password = "qt123";
		
		QString databaseName = "twitchapi";

		QString table_Games = "games";
		QString table_GamesViewers = "viewernumbers";

	};
}

#endif // GAMESDATABASE_H
