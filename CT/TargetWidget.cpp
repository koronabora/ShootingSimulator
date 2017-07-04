#include "TargetWidget.h"

TargetWidget::TargetWidget(quint64 id, QWidget* parent)
{
	_loader = new QUiLoader();
	_layout = new QGridLayout(this);
	//this->setParent(parent);
	//TARGET_WIND_X_SIZE = parent->geometry().width();
	//TARGET_WIND_Y_SIZE = parent->geometry().height();
	//TARGET_X_SIZE = TARGET_WIND_Y_SIZE - 10;
	//TARGET_Y_SIZE = TARGET_WIND_Y_SIZE - 10;
	_id = id;
	//this->setGeometry(0, 0, TARGET_WIND_X_SIZE, TARGET_WIND_Y_SIZE);

	this->setLayout(_layout);
	//_x = TARGET_WIND_X_SIZE / 2;
	//_y = TARGET_WIND_X_SIZE / 2;
	_initScene();
	setBackgroundRole(QPalette::Base);
	setAutoFillBackground(true);
	_sceneId = 0;
	_firstCall = true;
}

TargetWidget::~TargetWidget()
{
	if (_scene != Q_NULLPTR)
		_scene->deleteLater();
	if (_view != Q_NULLPTR)
		_view->deleteLater();
}

void TargetWidget::writeDot(quint64 id, qreal x, qreal y, qreal x2, qreal y2)
{
	if (_scene != Q_NULLPTR && _view != Q_NULLPTR)
	{
		if (id >= 0 && id < COLOR_VALUES.size())
		{
			qreal newX = TARGET_X_SIZE / 2 - x - THE_TARGET_SIZE / 2;
			qreal newY = TARGET_Y_SIZE / 2 - y - THE_TARGET_SIZE / 2;
			qreal newX2 = TARGET_X_SIZE / 2 - x2 - THE_TARGET_SIZE / 2;
			qreal newY2 = TARGET_Y_SIZE / 2 - y2 - THE_TARGET_SIZE / 2;

			//QColor c = COLOR_VALUES.at(id);

			if (_firstCall)
			{
				_firstCall = false;
			}

			QGraphicsLineItem* line = new QGraphicsLineItem(newX, newY, newX2, newY2);
			line->setPen(QPen(COLOR_VALUES.at(id), THIN_VALUES.at(id), Qt::SolidLine));
			_scene->addItem(line);

			/*if (shots.size() > 0 && shots.last()!=Q_NULLPTR)
				switch (id)
				{
				case COLORS::BEFORE_SHOT: // ƒанные дл€ текущего отображени€ перед выстрелом, вклюючают себ€
					shots.last()->resTraectory.append(line);
					break;
				case COLORS::RES_OTHER_SHOTS: //
					shots.last()->resOtherShots.append(line);
					break;
				case COLORS::RES_SHOT: //
					_shotReceived = true;
					shots.last()->resLastShot.append(line);
					shots.last()->statsInTime.append(line);
					break;
				case COLORS::STAT_AFTER_SHOT_INSIDE: //
					shots.last()->statsInside.append(line);
					break;
				case COLORS::STAT_AFTER_SHOT_OUTSIDE: //
					shots.last()->statsOutside.append(line);
					break;
				case COLORS::STAT_IN_TIME_OF_SHOT: //
					shots.last()->statsInTime.append(line);
					break;
				default:
					qDebug() << "Could not recognize point id = " << id;
				}
			else
				qDebug() << "Could not add point to storage because storage is null!!";

			_x = x;
			_y = y;*/
			update();
		}
		else
			qDebug() << "Could not draw point, unknown ID = " << id;
	}
	else
		qDebug() << "Could not paint dot on Graphics Scene because it is null!";
}

void TargetWidget::saveIntoFile(QString fileName)
{
	if (_view != Q_NULLPTR)
	{
		QPixmap pixMap = _view->grab();
		pixMap.save(fileName);
	}
}

void TargetWidget::changeScene()
{
	emit lastScene(_sceneId, _scene);
	_sceneId++;
	_initScene();
}
/*
void TargetWidget::_initScene()
{
	
	// инициализирование хранилища точек
	//ShotData* dat = new ShotData();
	//shots.append(dat);

	//_shotReceived = false;

	_scene = new QGraphicsScene();
	//_scene->setSceneRect(0, 0, TARGET_X_SIZE, TARGET_Y_SIZE);
	_layout->addWidget(_scene, 0, 0, 2, 3);
	this->setGeometry(0, 0, TARGET_WIND_X_SIZE, TARGET_WIND_Y_SIZE);
	if (_scene != Q_NULLPTR)
	{
		//_view = new QGraphicsView(_scene, this);
		_view->setScene(_scene);
		if (_view != Q_NULLPTR)
		{
			//_view->setGeometry(0, 0, TARGET_WIND_X_SIZE, TARGET_WIND_X_SIZE);
			int mod = TARGET_X_SIZE / 10;
			QPen p;
			for (quint64 i = 0; i < 10; i++)
				_scene->addEllipse(TARGET_X_SIZE / 2 - i*mod / 2, TARGET_Y_SIZE / 2 - i*mod / 2, i*mod, i*mod, QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap));
			//el = new QGraphicsEllipseItem(TARGET_X_SIZE / 2 - THE_TARGET_SIZE / 2, TARGET_Y_SIZE / 2 - THE_TARGET_SIZE / 2, THE_TARGET_SIZE, THE_TARGET_SIZE);
			//el->setPen(QPen(Qt::red, 10, Qt::SolidLine, Qt::RoundCap));
			//_scene->addItem(el);
		}
		else
			qDebug() << "Error while creating new GraphicsView for TargetWidget!";
	}
	else
		qDebug() << "Error while creating new GraphicsScene for TargetWidget!";
}*/

/*
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

void TargetWidget::selectShotCount()
{
	emit havePracticeInfo(quint64 shotCount, QString fio, QString zvan, QString info);
}*/