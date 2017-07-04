#pragma once

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QDebug>
#include "Defines.h"

class TargetWidget : public QWidget
{
	Q_OBJECT
public:
	TargetWidget(quint64 id, QWidget* parent=0);
	~TargetWidget();

private:
	//QGraphicsScene* _scene;
	//QGraphicsView* _view;
	quint64 _id;
	//QGraphicsEllipseItem* el;
	int _x;
	int _y;
signals:
	void logAny(QString data);
protected:
	void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
public slots:
	void writeDot(quint64 id, qreal x, qreal y);
	void saveIntoFile(QString fileName);
};

/*
int sampleRate = 8000;
int baud = 50;
int sampPerSym = sampleRate / baud;

// this is half the value of RMS for a sine wave 
int threshold = ((sqrt(2) / 2)*pow(2, 15)) / 2;
int sign = -1;

short int output = 0;
short int rsample = 0;
int numBits = 0;
int sampsLeft = 0;
int counter = 0;

short int movAvg[sampPerSym + 1];
int movAvgI = 0;
int sumMovAvg = 0;

char bitBuffer = 0;
char bitCounter = 0;
int i = 0;

const int ID = 0;
const unsigned long step = sampPerSym;//sampleRate / baud;
const float per = 10.0f;
int d0v;
int d1v;
int d2v;
int d3v;
int d4v;

unsigned long t;
unsigned long lt;

void setup() {
	// put your setup code here, to run once:
	Serial.begin(115200);
	memset(&movAvg, 0, sizeof(movAvg));
	t = micros();
	lt = t;
}

void loop() {
	t = micros();
	if ((lt<t && (t - lt) >= step) || (lt>t) && t >= step)
	{
		lt = t;
		d0v = analogRead(A0); // diod
		d1v = analogRead(A1);// diod
		d2v = analogRead(A2);// diod
		d3v = analogRead(A3);// diod
	}
}

int func(int sample)
{
	// rectify by taking abs
	rsample = abs(sample);

	// moving average
	movAvg[movAvgI] = rsample / sampPerSym;
	sumMovAvg = sumMovAvg + (movAvg[movAvgI]) - (movAvg[(movAvgI + 1) % sampPerSym]);
	movAvgI++;
	if (movAvgI >= sampPerSym) movAvgI = 0;

	// hard limit the samples
	if (sumMovAvg >= threshold) {
		output = 1;
	}
	else {
		output = -1;
	}

	// make bits out of the data
	if (output == sign) {
		counter++;
	}
	else {
		// count number of bits we have adding a partal if its good enough
		numBits = (int)counter / sampPerSym;
		sampsLeft = counter%sampPerSym;
		if (sampsLeft > (0.7*sampPerSym)) {
			numBits++;
		}

		// add the bits to our buffer
		for (i = 0; i < numBits; i++) {
			bitBuffer <<= 1;

			// put a 1 or zero in the lsb after shifting the buffer to the left
			if (sign == 1) {
				bitBuffer ^= (0x01);
			}
			else {
				bitBuffer &= (0xfe);
			}
			bitCounter++;

			// when we have enough to write a char out do so
			if (bitCounter > 7) {
				bitCounter = 0;
				fwrite(&bitBuffer, sizeof(char), 1, fileout);
			}
		}

		counter = 0;
		sign *= -1;
}*/