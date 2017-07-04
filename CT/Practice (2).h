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
	QVector<shotPoint*> before; // ������ ����� �� ������������
	QVector<shotPoint*> in; // ������ ����� �� ����� �������� � �� ������������ ������ + ��������� ���������� ����������
	QVector<shotPoint*> after; // ������ ����� ����� ��������

	quint64 startTime;
	quint64 shotTime; // ����� ����������� ������� �� ��������� ������
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