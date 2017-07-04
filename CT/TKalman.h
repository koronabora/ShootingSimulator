#pragma once

#include <QObject.h>

class TKalman: public QObject
{
	Q_OBJECT
private:
	qreal q; //process noise covariance
	qreal r; //measurement noise covariance
	int x; //value
	qreal p; //estimation error covariance
	qreal k; //kalman gain

	void KalmanUpdate(int value);

public:
	TKalman();
	~TKalman();

	void KalmanInit(qreal q, qreal r, qreal p, int intial_value);

	int GetResult(int data);

};

