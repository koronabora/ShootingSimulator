#ifndef CT_H
#define CT_H

#include <QtWidgets/QMainWindow>
#include <QGridLayout>
#include <QObject>
#include <QTimer>
#include "ui_ct.h"
#include "TargetWidget.h"
#include "GraphicsWidget.h"
#include "qcustomplot.h"

class CT : public QMainWindow
{
	Q_OBJECT

public:
	CT(QWidget *parent = 0);
	~CT();
	QList<TargetWidget*> _targets;
	QList<QCustomPlot*> _graphs;
private:
	Ui::CTClass ui;
	QGridLayout* mainLayout;
	QVector<qreal>* vx;
	QTimer* _timer;
	bool dataTouched;

	QVector<qreal>* _vals1;
	QVector<qreal>* _vals2;
	QVector<qreal>* _vals3;
	QVector<qreal>* _vals4;


	QVector<qreal>* _sma1;
	QVector<qreal>* _sma2;
	QVector<qreal>* _sma3;
	QVector<qreal>* _sma4;
	// I
	//QVector<qreal>* _i1;
	//QVector<qreal>* _i2;
	//QVector<qreal>* _i3;
	//QVector<qreal>* _i4;
	// Q
	//QVector<qreal>* _q1;
	//QVector<qreal>* _q2;
	//QVector<qreal>* _q3;
	//QVector<qreal>* _q4;
	// results
	QVector<qreal>* _res1;
	QVector<qreal>* _res2;
	QVector<qreal>* _res3;
	QVector<qreal>* _res4;

signals:
	void writeDot(quint64 id, qreal x, qreal y); // to targets

public slots:
	void paintDot(quint64 id, qreal x, qreal y); // from core
	void paintGraphs(quint64 id, qreal val);
	void redrawPlots();
};

#endif // CT_H
