#pragma once

#include <QObject>
#include <QTimer>
#include "Defines.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include "Target.h"
#include <QThread>
#include <time.h>

class ControllerHelper : public QObject
{
	Q_OBJECT
public:
	ControllerHelper();
	~ControllerHelper();

	void init();
private:
	quint64 _portId;
	QTimer* _timer;
	QTimer* _inTimer;
	QStringList _inf;
	QVector<bool> _tarStates;
	//bool keAval = false;
	QVector<bool> _boomStates; // содержит состояния отдачи
	QVector<bool> _relStates; // состояния реле
	QVector<bool> _buttonStates; // состояния спусковых крючко
	QVector<quint64> _lastShots; // время последнего выстрела на каждом порту
	QVector<quint64> _lastBooms; // время последней отдачи на каждом порту

	// KE24R reader - writer
	QSerialPort* _port;
	
	bool contrlConnected = false;
	void controllerConnected(QSerialPortInfo* info);
	void makeBoom(quint64 id); // инициирует отдачу, сверяя необходимость в соответсвии с _boomStates
	void keInit(); //
	quint64 curWPort = 0; //
	void checkInput(QStringList ls);
	QString _lastInput;
	quint64 lastCId = 0; // 0 - требуется проверка, 1 - установить выводы, 2 - прочитать состояние, 3 - включить реле, 4 - отключить реле
	void sendToCam(QString command);
	bool keInUse = true;
	void processShot(quint64 i, bool type);
public slots:
	void run(); // От потока
	void checkNewTargets(); // От таймера проверки новых устройств
	void checkButtonValues(); // От таймера проверки нажатия кнопок
	void readyRead(); // from serial port
	void error(QSerialPort::SerialPortError error); // от порта контроллера
	void contrlDisconnected(); // от порта контроллера
	void targetDisconnected(QSerialPortInfo* inf); // от ядра

	void changeBoomState(quint64 id, bool val); // от ядра
	void bytesWritten(qint64 bytes);

	void targetCon(quint64 index); //  из ядра
	void targetNonCon(quint64 index); //  из ядра
signals:
	void workFinished();
	//void logAny(QString message);
	void targetConnected(QSerialPortInfo* info); // к ядру
	void newShotBegin(quint64 targetId, quint64 time); // к ядру
	void newShotEnd(quint64 targetId, quint64 time); // к ядру
};