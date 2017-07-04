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
	QList<QSerialPortInfo> _inf;
	//QList<ComReader*> _coms;
	
	// KE24R reader - writer
	QSerialPort* _port;
	bool contrlConnected = false;
	void controllerConnected(QSerialPortInfo* info);
	void makeBoom(quint64 id); // ���������� ������, ������ ������������� � ����������� � _boomStates
	void keInit(); //
	quint64 curWPort = 0; //
	
	QVector<bool> _boomStates; // �������� ��������� ������
	quint64 lastCId = 0; // 0 - ��������� ��������, 1 - ���������� ������, 2 - ��������� ���������, 3 - �������� ����, 4 - ��������� ����
	void sendToCam(QString command);
	bool keInUse = true;

public slots:
	void run(); // �� ������
	void checkNewTargets(); // �� ������� �������� ����� ���������
	void checkButtonValues(); // �� ������� �������� ������� ������


	void readyRead(); // from serial port
	void error(QSerialPort::SerialPortError error); //
	
	void contrlDisconnected(); //
	void targetDisconnected(); // 

	void changeBoomState(quint64 id, bool val); //
	//void emulate();
signals:
	void workFinished();
	//void logAny(QString message);
	void targetConnected(QSerialPortInfo* info);
};