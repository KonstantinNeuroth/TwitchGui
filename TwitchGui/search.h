#ifndef GAME_H
#define GAME_H

#include "TwitchApi.h"

namespace TwitchApi {
	class Search : public QObject
	{
		Q_OBJECT

	public:
		Search(QObject *parent = 0);
		~Search();
		void refresh(QString search);

		std::vector<TwitchApi::channel> channels;

		public slots:
		void networkingFinished(QNetworkReply* Reply);

	private:
		QNetworkAccessManager *manager;
	};
}

#endif // GAME_H
