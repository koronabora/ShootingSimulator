#include "ct.h"
#include <QtWidgets/QApplication>
#include "Core.h"
#include <QObject>
#include <QThread> 
#include <stdio.h>
#include <stdlib.h>

//#define QCUSTOMPLOT_USE_OPENGL

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	QByteArray localMsg = msg.toLocal8Bit();
	switch (type) {
	case QtDebugMsg:
		//file();
		fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
		break;
	case QtWarningMsg:
		fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
		break;
	case QtCriticalMsg:
		fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
		break;
	case QtFatalMsg:
		fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
		abort();
	}
}

int main(int argc, char *argv[])
{
	qInstallMessageHandler(myMessageOutput);
	qRegisterMetaType<QVector<qreal> >("QVector<qreal>");
	QApplication a(argc, argv);
	CT w;
	Core* c = new Core();
	QThread th;
	c->moveToThread(&th);
	QObject::connect(&th, SIGNAL(started()), c, SLOT(run()));
	QObject::connect(c, SIGNAL(workFinished()), &th, SLOT(quit()));
	QObject::connect(&th, SIGNAL(finished()), c, SLOT(deleteLater()));
	QObject::connect(&th, SIGNAL(finished()), &th, SLOT(deleteLater()));
	if (c != Q_NULLPTR)
	{
		QObject::connect(c, &Core::paintDot, &w, &CT::paintDot);
		QObject::connect(c, &Core::paintGraphs, &w, &CT::paintGraphs);
	}
	th.start();
	w.showMaximized();
	return a.exec();
}
