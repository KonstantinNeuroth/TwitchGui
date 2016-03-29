#ifndef TWITCHGUI_H
#define TWITCHGUI_H

#include <QtWidgets/QMainWindow>
#include <QTimer>
#include <QLayout>

#include "ui_twitchgui.h"
#include "search.h"
#include "games.h"
#include "qcustomplot.h"
#include "gamesdatabase.h"

class TwitchGui : public QMainWindow
{
	Q_OBJECT

public:
	TwitchGui(QWidget *parent = 0);
	~TwitchGui();

	int updateInteval_ms = 10*60*1000;

public slots:
	void on_pushButtonPlot_clicked();
	void on_pushButtonRe_clicked();
	void on_pushButtonReset_clicked();

	void startMeasurement();

	void paintGraph(QString gameName);
	void deleteGraph(QString gameName);


	
private:
	void initPlot();
	void initControls();

	Ui::TwitchGuiClass ui;
	TwitchApi::Games *games;
	QHBoxLayout * layout;

	int maxViewers = 0;
	QDateTime maxTime;
	QDateTime minTime;


	QCustomPlot * customPlot = NULL;
	QVector<double> customPlot_time;
	QVector<double> customPlot_value;
	QVector<QString> paintedGraphs;

	QTimer * timer;
	QLineEdit *line = NULL;

	TwitchApi::GamesDatabase * db;

	

};

#endif // TWITCHGUI_H
