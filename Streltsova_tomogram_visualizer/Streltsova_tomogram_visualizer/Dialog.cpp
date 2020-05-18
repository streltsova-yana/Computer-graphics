#include "Dialog.h"
#include <qlabel>
#include <qgridlayout>
#include <qpushbutton>

Dialog::Dialog(QWidget* parent) : QDialog(parent)
{
	setWindowTitle("Min Max values");

	max = new QLineEdit;
	min = new QLineEdit;

	QLabel* maxLabel = new QLabel("Max value:");
	QLabel* minLabel = new QLabel("Min value:");

	maxLabel->setBuddy(max);
	minLabel->setBuddy(min);

	QPushButton* ok = new QPushButton("Ok");
	QPushButton* cancel = new QPushButton("Cancel");

	connect(ok, SIGNAL(clicked()), SLOT(accept()));
	connect(cancel, SIGNAL(clicked()), SLOT(reject()));

	QGridLayout* layout = new QGridLayout;
	layout->addWidget(maxLabel, 0, 0);
	layout->addWidget(minLabel, 1, 0);
	layout->addWidget(max, 0, 1);
	layout->addWidget(min, 1, 1);
	layout->addWidget(ok, 2, 0);
	layout->addWidget(cancel, 2, 1);
	setLayout(layout);
};