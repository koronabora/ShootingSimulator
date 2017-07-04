#include "ct.h"
#include <QtWidgets/QApplication>
#include "Core.h"
#include <QObject>
#include <QThread> 
#include <QMutex>
#include <QTextStream>
#include <QWidget>
#include <QUiLoader>
#include <stdio.h>
#include <stdlib.h>

//#define QCUSTOMPLOT_USE_OPENGL
static QMutex mutex;

QFile outFile;
QTextStream stream(&outFile);
QDateTime ld;
QDir dir;

bool checkDir(QString sub)
{
	if (sub.size() > 0)
	{
		if (dir.dirName().compare(LOG_DIR)!=0 && dir.path().contains(LOG_DIR))
			if (!dir.cdUp())
				return false;
		if (!dir.cd(sub))
		{
			if (!dir.mkdir(sub))
				return false;
			if (!dir.cd(sub))
				return false;
		}
	}
	else
		if (!dir.cd(LOG_DIR))
		{
			if (!dir.mkdir(LOG_DIR))
				return false;
			if (!dir.cd(LOG_DIR))
				return false;
		}
	return true;
}


void rotateLog()
{
	outFile.close();
	QDateTime t = QDateTime::currentDateTime();
	if (checkDir(t.date().toString(LOG_DATE_FORMAT)))
	{
		QString fName = dir.absolutePath() + '//' + t.time().toString(LOG_TIME_FORMAT) + ".log";
		outFile.setFileName(fName);
	}
	bool ok = outFile.open(QIODevice::WriteOnly | QIODevice::Append);
}

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	mutex.lock();

	QDateTime dateTime(QDateTime::currentDateTime());
	if (ld.time().hour() != dateTime.time().hour())
		rotateLog();
	ld = dateTime;
	QString timeStr(dateTime.toString("mm:ss:zzz"));
	QString contextString(QString("(%1, %2)").arg(context.file).arg(context.line));
	stream << timeStr << " " << contextString << ": " << msg << endl;
	stream.flush();
	outFile.flush();
	mutex.unlock();
}

int main(int argc, char *argv[])
{
	QCoreApplication::setOrganizationName(COMPANY_NAME);
	QCoreApplication::setApplicationName(SOFT_NAME);
	qRegisterMetaType<QVector<qreal>>("QVector<qreal>");
	qRegisterMetaType<QList<QList<qreal>>>("QList<QList<qreal>>");
	QApplication a(argc, argv);
	QDateTime t = QDateTime::currentDateTime();
	ld = t;
	if (checkDir("") && checkDir(t.date().toString(LOG_DATE_FORMAT)))
	{
		QString fName = dir.absolutePath() + '//' + t.time().toString(LOG_TIME_FORMAT) + ".log";
		outFile.setFileName(fName);
		bool ok = outFile.open(QIODevice::WriteOnly | QIODevice::Append);
		if (ok)
		{
#ifndef TEST_MODE
			//qInstallMessageHandler(myMessageOutput);
#endif
		}
		qDebug() << "Opening log file: " << ok;
	}
	CT w;
	Core* c = new Core();
	c->init(&w);
	QThread th;
	c->moveToThread(&th);
	QObject::connect(&th, SIGNAL(started()), c, SLOT(run()));
	QObject::connect(c, SIGNAL(workFinished()), &th, SLOT(quit()));
	QObject::connect(&th, SIGNAL(finished()), c, SLOT(deleteLater()));
	QObject::connect(&th, SIGNAL(finished()), &th, SLOT(deleteLater()));
	if (c != Q_NULLPTR)
	{
		//QObject::connect(c, &Core::paintDot, &w, &CT::paintDot);
		//QObject::connect(c, &Core::paintGraphs, &w, &CT::paintGraphs);
	}
	th.start();
	w.setWindowFlags(Qt::FramelessWindowHint);
	w.showMaximized();
	outFile.flush();
	outFile.close();
	return a.exec();
}
