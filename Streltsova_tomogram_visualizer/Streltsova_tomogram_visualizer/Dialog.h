#pragma once
#include <qdialog>
#include <qlineedit>

class Dialog : public QDialog
{
	Q_OBJECT
private:
	QLineEdit* max;
	QLineEdit* min;
public:
	Dialog(QWidget* parent = Q_NULLPTR);
	int getMax() const { return max->text().toInt(); };
	int getMin() const { return min->text().toInt(); };
};