#include "twitchgui.h"

#include <QDebug>
#include <QLineEdit>
#include <QLayout>
#include <QCompleter>



TwitchGui::TwitchGui(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	db = new TwitchApi::GamesDatabase();
	srand(QDateTime::currentDateTime().toTime_t());
	maxTime = QDateTime::currentDateTime();
	minTime = QDateTime::currentDateTime();

	games = new TwitchApi::Games();
	connect(games, &TwitchApi::Games::gameDataReady, db, &TwitchApi::GamesDatabase::addGames);
	
	initPlot();
	initControls();

	//update Function: get new values
	timer = new QTimer();
	connect(timer, SIGNAL(timeout()), this, SLOT(startMeasurement()));
	timer->start();		
}

TwitchGui::~TwitchGui()
{

}

void TwitchGui::on_pushButtonPlot_clicked()
{
	auto name = ui.chooseGraph->text();
	if (db->isAGame(name))
		paintGraph(name);
	else
		qDebug() << "badInput";
}

void TwitchGui::on_pushButtonRe_clicked()
{
	auto name = ui.chooseGraph->text();
	if (db->isAGame(name))
		deleteGraph(name);
	else
		qDebug() << "badInput";
}

void TwitchGui::on_pushButtonReset_clicked()
{
	customPlot->clearGraphs();
	customPlot->replot();
	paintedGraphs.clear();
	maxViewers = 0;
	maxTime = QDateTime::currentDateTime();
	minTime = QDateTime::currentDateTime();
}

void TwitchGui::startMeasurement()
{
	games->refresh();
	timer->start(updateInteval_ms);
}

void TwitchGui::paintGraph(QString gameName)
{
	//check input
	if (!db->isAGame(gameName))return;

	//check if Graph is already painted
	if (paintedGraphs.contains(gameName))return;

	//add Graph
	customPlot->addGraph();
	QPen pen;

	//Graph Settings an Colours
	pen.setColor(QColor(rand() % 245 + 10, rand() % 245 + 10, rand() % 245 + 10));
	customPlot->graph()->setPen(pen);
	customPlot->graph()->setLineStyle(QCPGraph::lsLine);
	customPlot->graph()->setBrush(QBrush(QColor(rand() % 245 + 10, rand() % 245 + 10, rand() % 245 + 10, 150)));
	customPlot->graph()->setName(QString(gameName));

	//get new Data
	auto values = db->getDataFromName(gameName);

	//check data for max values
	auto max = *std::max_element(values.second.begin(), values.second.end());
	auto minTimeLocal = *std::min_element(values.first.begin(), values.first.end());
	if (static_cast<int>(max) > maxViewers) maxViewers = static_cast<int>(max);
	if (minTime > minTimeLocal) minTime = minTimeLocal;

	//set view on Plot
	customPlot->yAxis->setRange(0, maxViewers + 1000);
	customPlot->xAxis->setRange(minTime.toTime_t(), maxTime.toTime_t());

	//convert Data
	QVector<qreal> time;
	for (auto i : values.first)
	{
		time.push_back(i.toTime_t());
	}

	//Paint Data into Graph
	customPlot->graph()->setData(time, values.second.toVector());
	customPlot->graph()->setVisible(true);
	customPlot->replot();

	//save Data
	paintedGraphs.push_back(gameName);
}

void TwitchGui::deleteGraph(QString gameName)
{
	
	//check input
	if (!db->isAGame(gameName))return;
	
	//search Graph
	auto position = std::find(paintedGraphs.begin(), paintedGraphs.end(), gameName);
	auto index = position - paintedGraphs.begin();
	
	//Graph not painted
	if (position == paintedGraphs.end())return;

	//delete Graph
	
	qDebug() << index;
	qDebug() << customPlot->graph(index)->name();
	customPlot->removeGraph(index); 
	paintedGraphs.remove(index);
	customPlot->replot();
}

void TwitchGui::initPlot()
{
	//add Plot Widget
	customPlot = new QCustomPlot(ui.Painting);
	QHBoxLayout * layout = new QHBoxLayout(ui.Painting);
	layout->addWidget(customPlot);

	//Settings
	customPlot->setLocale(QLocale(QLocale::German, QLocale::Germany));

	customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
	customPlot->xAxis->setDateTimeFormat("d.M.yyyy\n H:m:s");

	customPlot->xAxis->setLabel("Date");
	customPlot->yAxis->setLabel("Viewers");

	customPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
	customPlot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));	
}

void TwitchGui::initControls()
{
	auto wordList = db->getGameListName();
	QCompleter *completer = new QCompleter(wordList, this);
	completer->setCaseSensitivity(Qt::CaseInsensitive);
	completer->setCompletionMode(QCompleter::InlineCompletion);
	ui.chooseGraph->setCompleter(completer);
}



