#include "TargetWidget.h"

TargetWidget::TargetWidget(quint64 id, QWidget* parent)
{
	//this->setParent(parent);
	//TARGET_WIND_X_SIZE = parent->geometry().width();
	//TARGET_WIND_Y_SIZE = parent->geometry().height();
	//TARGET_X_SIZE = TARGET_WIND_Y_SIZE - 10;
	//TARGET_Y_SIZE = TARGET_WIND_Y_SIZE - 10;
	_id = id;
	this->setGeometry(0, 0, TARGET_WIND_X_SIZE, TARGET_WIND_Y_SIZE);
	_x = TARGET_WIND_X_SIZE / 2;
	_y = TARGET_WIND_X_SIZE / 2;
	/*_scene = new QGraphicsScene(this);
	_scene->setSceneRect(0, 0, TARGET_X_SIZE, TARGET_Y_SIZE);
	this->setGeometry(0, 0, TARGET_WIND_X_SIZE, TARGET_WIND_Y_SIZE);
	if (_scene != Q_NULLPTR)
	{
		_view = new QGraphicsView(_scene, this);
		if (_view != Q_NULLPTR)
		{
			//_view->setGeometry(0, 0, TARGET_WIND_X_SIZE, TARGET_WIND_X_SIZE);
			int mod = TARGET_X_SIZE/10;
			QPen p;
			for (quint64 i=0; i<10; i++)
				_scene->addEllipse(TARGET_X_SIZE /2 - i*mod/2, TARGET_Y_SIZE /2- i*mod / 2, i*mod, i*mod, QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap));

			el = new QGraphicsEllipseItem(TARGET_X_SIZE / 2 - THE_TARGET_SIZE / 2, TARGET_Y_SIZE / 2 - THE_TARGET_SIZE / 2, THE_TARGET_SIZE, THE_TARGET_SIZE);
			el->setPen(QPen(Qt::red, 10, Qt::SolidLine, Qt::RoundCap));
			_scene->addItem(el);
		}
		else
			emit logAny(tr("Error while creating new GraphicsView for TargetWidget!"));
	}
	else
		emit logAny(tr("Error while creating new GraphicsScene for TargetWidget!"));
		*/

	setBackgroundRole(QPalette::Base);
	setAutoFillBackground(true);
}

TargetWidget::~TargetWidget()
{}

void TargetWidget::writeDot(quint64 id, qreal x, qreal y)
{
	if (_id == id)
	{
		/*if (id == _id && _scene != Q_NULLPTR && _view != Q_NULLPTR)
		{
			qreal newX = TARGET_X_SIZE / 2 - x - THE_TARGET_SIZE/2;
			qreal newY = TARGET_Y_SIZE / 2 - x - THE_TARGET_SIZE / 2;
			el->setPos(QPointF(newX, newY));
		}*/
		_x = x;
		_y = y;
		update();
	}
}

void TargetWidget::saveIntoFile(QString fileName)
{

}

void TargetWidget::paintEvent(QPaintEvent *event)
{
	QWidget::paintEvent(event);
	QPainter painter(this);
	painter.setPen(Qt::black);
	//painter.setBrush(Qt::BrushStyle::CrossPattern);
	int step = TARGET_X_SIZE / 10;
	for (quint64 i = 0; i < 10; i++)
		painter.drawEllipse(TARGET_WIND_X_SIZE / 2-step*i/2, TARGET_WIND_Y_SIZE / 2-step*i / 2, step*i, step*i);
	painter.setPen(Qt::red);
	painter.setBrush(Qt::BrushStyle::CrossPattern);
	painter.drawEllipse(TARGET_WIND_X_SIZE / 2 - _x - THE_TARGET_SIZE/2, TARGET_WIND_X_SIZE / 2 -  _y - THE_TARGET_SIZE/2, THE_TARGET_SIZE, THE_TARGET_SIZE);
	//qDebug() << "x = " << _x << " y = " << _y << " -> (" << TARGET_WIND_X_SIZE / 2 - _x - THE_TARGET_SIZE / 2 << ", " << TARGET_WIND_Y_SIZE / 2 - _y - THE_TARGET_SIZE / 2 <<")";
}