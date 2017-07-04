#pragma once

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QDebug>
#include <QPixmap>
#include <QGridLayout>
#include <QtUiTools>
#include "Defines.h"

/*struct ShotData
{
	QVector<QGraphicsLineItem*> statsOutside; // ���������� ��������. ���������� ��� ������ �� ������� ������
	QVector<QGraphicsLineItem*> statsInside; // ���������� ��������. ���������� ������ ������
	QVector<QGraphicsLineItem*> statsInTime; // ���������� ��������. ���������� �� ����� ��������
	QVector<QGraphicsLineItem*> resTraectory; // ������� ���������, ����� ������������
	QVector<QGraphicsLineItem*> resLastShot; // ����� �������� ���������
	QVector<QGraphicsLineItem*> resOtherShots; // ����� ������ ���������
};*/

class TargetWidget : public QWidget
{
	Q_OBJECT
public:
	TargetWidget(quint64 id, QWidget* parent=0);
	~TargetWidget();

private:
	QGraphicsScene* _scene;
	QGraphicsView* _view;
	QGridLayout* _layout;
	QUiLoader* _loader;
	quint64 _id;
	//int _x;
	//int _y;
	quint64 _sceneId;
	void _initScene();
	bool _firstCall;
	//bool _shotReceived;

signals:
	void lastScene(quint64 _sceneId, QGraphicsScene* sc); // ���������� � ����� ������� ���� ������ ����� ��� ��������� �����
	void havePracticeInfo(quint64 shotCount, QString fio, QString zvan, QString info);

public slots:
	void writeDot(quint64 type, qreal x, qreal y, qreal x2, qreal y2); // ������ �����
	void saveIntoFile(QString fileName); // ��������� � ���� ����������� �����
	void changeScene(); // ������ ����� �� �����
	void setScene(QGraphicsScene* sc);
	void selectPracticeInfo();
};
