#include "RTMainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	RTMainWindow w;
	w.show();

	return a.exec();
}
