#ifndef GAMES_H
#define GAMES_H

#include "TwitchApi.h"

namespace TwitchApi
{
	class Games : public QObject
	{
		Q_OBJECT

	public:
		Games(int limit = 100, int offset = 0, QObject *parent = 0);
		~Games();
		void refresh(int limit = 100, int offset = 0);
		friend QDebug &operator<<(QDebug os, const TwitchApi::Games * obj);
		friend QDataStream &operator<<(QDataStream os, const TwitchApi::Games * obj);

		double numberOfGames;
		std::vector<TwitchApi::game> savedGames;
		QDateTime timestamp;

	signals:
		void gameDataReady(Games *t);


	public slots:
		void networkingFinished(QNetworkReply* Reply);

	private:
		QNetworkAccessManager *manager;
		QUrl next;
	};
}


#endif // GAMES_H
