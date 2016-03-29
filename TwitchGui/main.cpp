#include "twitchgui.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	TwitchGui w;
	w.show();
	return a.exec();
}
