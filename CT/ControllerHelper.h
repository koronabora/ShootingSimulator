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
	QVector<bool> _boomStates; // �������� ��������� ������
	QVector<bool> _relStates; // ��������� ����
	QVector<bool> _buttonStates; // ��������� ��������� ������
	QVector<quint64> _lastShots; // ����� ���������� �������� �� ������ �����
	QVector<quint64> _lastBooms; // ����� ��������� ������ �� ������ �����

	// KE24R reader - writer
	QSerialPort* _port;
	
	bool contrlConnected = false;
	void controllerConnected(QSerialPortInfo* info);
	void makeBoom(quint64 id); // ���������� ������, ������ ������������� � ����������� � _boomStates
	void keInit(); //
	quint64 curWPort = 0; //
	void checkInput(QStringList ls);
	QString _lastInput;
	quint64 lastCId = 0; // 0 - ��������� ��������, 1 - ���������� ������, 2 - ��������� ���������, 3 - �������� ����, 4 - ��������� ����
	void sendToCam(QString command);
	bool keInUse = true;
	void processShot(quint64 i, bool type);
public slots:
	void run(); // �� ������
	void checkNewTargets(); // �� ������� �������� ����� ���������
	void checkButtonValues(); // �� ������� �������� ������� ������
	void readyRead(); // from serial port
	void error(QSerialPort::SerialPortError error); // �� ����� �����������
	void contrlDisconnected(); // �� ����� �����������
	void targetDisconnected(QSerialPortInfo* inf); // �� ����

	void changeBoomState(quint64 id, bool val); // �� ����
	void bytesWritten(qint64 bytes);

	void targetCon(quint64 index); //  �� ����
	void targetNonCon(quint64 index); //  �� ����
signals:
	void workFinished();
	//void logAny(QString message);
	void targetConnected(QSerialPortInfo* info); // � ����
	void newShotBegin(quint64 targetId, quint64 time); // � ����
	void newShotEnd(quint64 targetId, quint64 time); // � ����
};