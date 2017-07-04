#pragma once

#include <QObject>
#include <QThread>
#include "Defines.h"
#include "ControllerHelper.h"
//#include "TKalman.h"
#include "wavelet2d.h"
#include <QDebug>
#include <QTime>
#include <QTimer>
#include <QSerialPort>
#include <QSerialPortInfo>

class Target : public QObject
{
	Q_OBJECT
public:
	Target();
	~Target();
	void init(QSerialPortInfo* inf);
private:
	QTimer* _timer;
	QTimer* _timer2;
	ControllerHelper* _cn;
	// raw values
	QVector<int>* _vals1;
	QVector<int>* _vals2;
	QVector<int>* _vals3;
	QVector<int>* _vals4;
	// time stamps from start of day for this values
	QVector<quint64>* _t;

	// sma values
	QVector<qreal>* _sma1;
	QVector<qreal>* _sma2;
	QVector<qreal>* _sma3;
	QVector<qreal>* _sma4;
	// I
	QVector<qreal>* _i1;
	QVector<qreal>* _i2;
	QVector<qreal>* _i3;
	QVector<qreal>* _i4;
	// Q
	QVector<qreal>* _q1;
	QVector<qreal>* _q2;
	QVector<qreal>* _q3;
	QVector<qreal>* _q4;
	// results
	QVector<qreal>* _res1;
	QVector<qreal>* _res2;
	QVector<qreal>* _res3;
	QVector<qreal>* _res4;

	QVector<qreal>* _buf;

	qreal SMA(QVector<int>* v, quint64 pos, quint64 per);
	void computeLastPeriod();
	int posOfMax(QVector<qreal>* v, quint64 b, quint64 e);
	int posOfMin(QVector<qreal>* v, quint64 b, quint64 e);
	void I(QVector<int>* in, QVector<qreal>* out);
	void Q(QVector<int>* in, QVector<qreal>* out);
	qreal lastAmp(QVector<qreal>* v, QVector<qreal>* sma);
	void CIC(QVector<qreal>* in, QVector<qreal>* out, int R);
	void FIR(QVector<qreal>* in, QVector<qreal>* out);
	void findthresh(vector<double> vector1, int N, double& t);
	void waveFileter(QVector<int>* in, QVector<double>* out);
	//quint64 stamp = 0; // beginning of carrier period
	//quint64 period;
	bool dataTouched;
	quint64 lastTime;
	int dataCount = 0;
	//TKalman _kalman;



	QSerialPortInfo* _portInfo;
	QSerialPort* _port;
	//quint64 _commandId; // id of command to use with signals

public slots:
	void run();
	void newShot();
	void newValues(quint64 id, quint64 a, quint64 b, quint64 c, quint64 d, quint64 time);
	void processTimer();
	void processTimer2();
	void readyRead(); // from serial port
	void error(QSerialPort::SerialPortError error);
signals:
	void logAny(QString message);
	void workFinished();
	void paintDot(quint64 id, qreal x, qreal y); // from core
	void paintGraphs(quint64 id, qreal values);
};