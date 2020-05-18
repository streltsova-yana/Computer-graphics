#include "View.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	View w;
	w.LoadData("..//Streltsova_tomogram_visualizer//Resources//testdata.bin");
	w.show();
	return a.exec();
}
