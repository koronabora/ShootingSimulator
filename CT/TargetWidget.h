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
	QVector<QGraphicsLineItem*> statsOutside; // Статистика выстрела. Траектория при выходе за границы мишени
	QVector<QGraphicsLineItem*> statsInside; // Статистика выстрела. Траектория внутри мишени
	QVector<QGraphicsLineItem*> statsInTime; // Статистика выстрела. Траектория во время выстрела
	QVector<QGraphicsLineItem*> resTraectory; // Текущий результат, линия прицеливания
	QVector<QGraphicsLineItem*> resLastShot; // Место текущего попадания
	QVector<QGraphicsLineItem*> resOtherShots; // Место других попаданий
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
	void lastScene(quint64 _sceneId, QGraphicsScene* sc); // отправляем в класс задание свою старую сцену при получении новой
	void havePracticeInfo(quint64 shotCount, QString fio, QString zvan, QString info);

public slots:
	void writeDot(quint64 type, qreal x, qreal y, qreal x2, qreal y2); // рисуем линию
	void saveIntoFile(QString fileName); // сохраняем в файл изображение сцены
	void changeScene(); // меняем сцену на новую
	void setScene(QGraphicsScene* sc);
	void selectPracticeInfo();
};
