#include "ShaderWidget.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ShaderWidget w(NULL);
	w.show();
	return a.exec();
}
