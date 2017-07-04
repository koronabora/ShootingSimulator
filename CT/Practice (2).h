#pragma once

#include <QObject>
#include <QVector>

struct shotPoint
{
	qreal x; //
	qreal y; //
	quint64 time; //
};

struct shotResults
{
	quint8 res; //
	quint64 shotTime; //
	qreal dispAll; //
	qreal dispVert; //
	qreal dispHor; //
	qreal kren; //
};

struct shot
{
	QVector<shotPoint*> before; // Массив точек до прицеливания
	QVector<shotPoint*> in; // Массив точек во время выстрела и до срабатывания отдачи + некоторое количество милесекунд
	QVector<shotPoint*> after; // Массив точек после выстрела

	quint64 startTime;
	quint64 shotTime; // Время регистрации нажатия на спусковой крючек
	quint64 endTime;
	quint64 sceneId;
	shotResults res;
};

class Practice : public QObject
{
	Q_OBJECT
public:
	Practice();
	~Practice();
private:
	quint64 _targetId;
	QVector<shot*> _shots;
	void _calcResults();
signals:
	void plotPoint(quint64 id, qreal x, qreal y, quint64 type);
	void plotMoveToPoint(quint64 id, qreal x, qreal y, quint64 type);
public slots:
	void newValues(quint64 targetId, qreal x, qreal y);
	void newShot(quint64 targetId, qreal x, qreal y, quint64 time);
};