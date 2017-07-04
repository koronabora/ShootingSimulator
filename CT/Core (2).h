#pragma once

#include <QObject>
#include <QThread>
#include "Defines.h"
#include "ControllerHelper.h"
#include <QDebug>
#include <QTime>
#include <QTimer>

class Core : public QObject
{
	Q_OBJECT
public:
	Core();
	~Core();

private:
	ControllerHelper* _cHeler; // устройства на usb- кнопки оружия
		
public slots:
	void run();
	void targetConnected(QSerialPortInfo inf);
	void targetDisconnected(QSerialPortInfo inf);
signals:
	void logAny(QString message);
	void workFinished();

};
