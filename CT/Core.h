#pragma once

#include <QObject>
#include <QThread>
#include "Defines.h"
#include "ControllerHelper.h"
#include "Target.h"
#include "SoundManager.h"
#include "PrintHelper.h"
#include "CT.h"
#include <QSettings>
#include <QDebug>
#include <QTime>
#include <QTimer>
#include <Practice.h>

class Core : public QObject
{
	Q_OBJECT
public:
	Core();
	~Core();

	void init(CT* ui);
private:
	QList<Practice*> _pr;
	QList<QThread*> _prThreads;

	QList<Target*> _targets;
	QList<QSerialPortInfo*> _tInfos;
	QList<QThread*> _tThreads;

	CT* gui;

	ControllerHelper* _cHelper; // устройства на usb- кнопки оружи€
	QThread* _chThread;
	SoundManager* _sm;
	QThread* _smThread;
	PrintHelper* _ph;
	QThread* _phThread;
	void loadSettings();

	QSettings* settings;

public slots:
	void run();
	void targetConnected(QSerialPortInfo* inf); // от контроллера портов
	void targetDisconnectedFromCom(QSerialPortInfo* inf); // от com портов
	void saveCorrection(QList<QList<qreal>> v);

signals:
	void loadCorrection(QList<QList<qreal>> v);
	void workFinished();
	void targetDisconnected(QSerialPortInfo* inf); // к контроллеру портов
	void newPosition(quint64 id, qreal x, qreal y, quint64 time); // перенаправл€ем сигнал экземпл€рам класса задание
	void newShotBegin(quint64 targetId, quint64 time); // к обработчику заданий
	void newShotEnd(quint64 targetId, quint64 time); // к обработчику заданий
	void targetOffline(quint64 id); // к обработчику заданий
	void targetOnline(quint64 id);
	//void changeTargetState(quint64 id, bool state);

	void plotGraph0(qreal x);
	void plotGraph1(qreal x);
	void plotGraph3(qreal x);
	void plotGraph4(qreal x);
};
