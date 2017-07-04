#pragma once

#include <QObject>
#include "Defines.h"

class PrintHelper : public QObject
{
	Q_OBJECT
public:
	PrintHelper();
	~PrintHelper();
	void init();
private:
	QString _printerName;
	public slots :
	void printFile(QString fileName);
	void run();
signals:
	void workFinished();
	void logAny(QString message);
};