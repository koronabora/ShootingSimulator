#pragma once

#include <QObject>
#include <QThread>
#include "Defines.h"
//#include "ControllerHelper.h"
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
	/*QTimer* _timer;
	QTimer* _timer2;
	//ControllerHelper* _cn;
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
	*/
	//qreal SMA(QVector<int>* v, quint64 pos, quint64 per);
	//void computeLastPeriod();
	//int posOfMax(QVector<qreal>* v, quint64 b, quint64 e);
	//int posOfMin(QVector<qreal>* v, quint64 b, quint64 e);
	//void I(QVector<int>* in, QVector<qreal>* out);
	//void Q(QVector<int>* in, QVector<qreal>* out);
	//qreal lastAmp(QVector<qreal>* v, QVector<qreal>* sma);
	//void CIC(QVector<qreal>* in, QVector<qreal>* out, int R);
	//void FIR(QVector<qreal>* in, QVector<qreal>* out);
	///void findthresh(vector<double> vector1, int N, double& t);
	//void waveFileter(QVector<int>* in, QVector<double>* out);
	//void newValues(quint64 id, quint64 a, quint64 b, quint64 c, quint64 d, quint64 time);
	//quint64 stamp = 0; // beginning of carrier period
	//quint64 period;
	//bool dataTouched;
	//quint64 lastTime;
	//int dataCount = 0;
	//TKalman _kalman;

	//quint64 minVal = 0;
	//bool rMin = true;
	//quint64 maxVal = COM_ADC_POWER;
	//bool rMax = true;

	QList<QList<qreal>> CORR = {
		{ 1.0, 1.0, 1.0, 1.0, 3.0 },
		{ 1.0, 1.0, 1.0, 1.0, 3.0 },
		{ 1.0, 1.0, 1.0, 1.0, 3.0 },
		{ 1.0, 1.0, 1.0, 1.0, 3.0 },
		{ 1.0, 1.0, 1.0, 1.0, 3.0 },
		{ 1.0, 1.0, 1.0, 1.0, 3.0 }
	};

	quint64 smaPer = CORE_SMA_PERIOD;

	void newValues(quint64 id, quint64 a, quint64 b, quint64 c, quint64 d, quint64 time);
	quint16 sma(QVector<quint16>* v, quint16 newV);
	qreal sma2f(QVector<qreal>* v, qreal newV);
	QSerialPortInfo* _portInfo;
	QSerialPort* _port;
	QVector<quint16> sma0;
	QVector<quint16> sma1;
	QVector<quint16> sma2;
	QVector<quint16> sma3;
	QVector<qreal> smaX;
	QVector<qreal> smaY;
	//quint64 _commandId; // id of command to use with signals
	quint64 counter;
	quint64 _id;
	quint64 userSmaPer = CORE_SMA2;
	qreal multic = 3.0;

public slots:
	void run();
	//void processTimer();
	//void processTimer2();
	void readyRead(); // from serial port
	void error(QSerialPort::SerialPortError error);
	void closed(); // от порт
	void setCorrection(QList<QList<qreal>> v);
	void setSmaPer(quint64 sma);
	void setUserSmaPer(quint64 sma);
	void setMulti(qreal val, quint64 id);
signals:
	void logAny(QString message);
	void workFinished();
	//void paintDot(quint64 id, qreal x, qreal y); // from core
	void newPosition(quint64 id, qreal x, qreal y, quint64 time); // to core
	void paintGraphs(quint64 id, qreal values);
	void connected(quint64 id);
	void disconnected(quint64 id);
	void disconnectedInf(QSerialPortInfo* inf);


	void plotGraph0(quint64 id, qreal x);
	void plotGraph1(quint64 id, qreal x);
	void plotGraph2(quint64 id, qreal x);
	void plotGraph3(quint64 id, qreal x);
};