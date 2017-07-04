#include "ct.h"

CT::CT(QWidget *parent)
	: QMainWindow(parent)
{
	_vals1 = new QVector<qreal>(CORE_DATA_SIZE);
	_vals2 = new QVector<qreal>(CORE_DATA_SIZE);
	_vals3 = new QVector<qreal>(CORE_DATA_SIZE);
	_vals4 = new QVector<qreal>(CORE_DATA_SIZE);

	_sma1 = new QVector<qreal>(CORE_DATA_SIZE);
	_sma2 = new QVector<qreal>(CORE_DATA_SIZE);
	_sma3 = new QVector<qreal>(CORE_DATA_SIZE);
	_sma4 = new QVector<qreal>(CORE_DATA_SIZE);

	/*_i1 = new QVector<qreal>(CORE_DATA_SIZE);
	_i2 = new QVector<qreal>(CORE_DATA_SIZE);
	_i3 = new QVector<qreal>(CORE_DATA_SIZE);
	_i4 = new QVector<qreal>(CORE_DATA_SIZE);

	_q1 = new QVector<qreal>(CORE_DATA_SIZE);
	_q2 = new QVector<qreal>(CORE_DATA_SIZE);
	_q3 = new QVector<qreal>(CORE_DATA_SIZE);
	_q4 = new QVector<qreal>(CORE_DATA_SIZE);*/

	_res1 = new QVector<qreal>(CORE_DATA_SIZE);
	_res2 = new QVector<qreal>(CORE_DATA_SIZE);
	_res3 = new QVector<qreal>(CORE_DATA_SIZE);
	_res4 = new QVector<qreal>(CORE_DATA_SIZE);

	ui.setupUi(this);
	mainLayout = ui.gl;
	this->setUpdatesEnabled(true);
	
	TargetWidget* tar = new TargetWidget(0, this);
	if (tar != Q_NULLPTR)
	{
		connect(this, &CT::writeDot, tar, &TargetWidget::writeDot);
		_targets.append(tar);
	}
	mainLayout->addWidget(tar, 0, 0, 3, 1);

	QCustomPlot* tpr;

	vx = new QVector<qreal>();
	for (quint64 j = 0; j < CORE_DATA_SIZE; j++)
	{
		vx->append(j);
	}

	for (quint64 i = 1; i <= 1/*4*/; i++)
	{
		tpr = new QCustomPlot(this);
		//tpr->yAxis->setVisible(false);
		//tpr->xAxis->setVisible(false);
		tpr->yAxis->setRange(0, COM_ADC_POWER);
		tpr->xAxis->setRange(0, CORE_DATA_SIZE);
		
		// signal
		tpr->addGraph();
		tpr->graph(0)->setAntialiased(CORE_GRAPHS_ALIASED);
		tpr->graph(0)->setLineStyle(QCPGraph::lsStepCenter);
		
		// sma
		tpr->addGraph();
		tpr->graph(1)->setPen(QPen(Qt::red));
		tpr->graph(1)->setAntialiased(CORE_GRAPHS_ALIASED);
		
		// out
		tpr->addGraph();
		tpr->graph(2)->setPen(QPen(Qt::black));
		tpr->graph(2)->setAntialiased(CORE_GRAPHS_ALIASED);

		// i
		/*tpr->addGraph();
		tpr->graph(2)->setPen(QPen(Qt::green));
		tpr->graph(2)->setAntialiased(true);

		// results
		tpr->addGraph();
		tpr->graph(3)->setPen(QPen(Qt::black));
		tpr->graph(3)->setAntialiased(true);

		// q
		tpr->addGraph();
		tpr->graph(4)->setPen(QPen(Qt::magenta));
		tpr->graph(4)->setAntialiased(true);*/

		tpr->replot();
		if (tpr != Q_NULLPTR)
		{
			//connect(this, &CT::writeDot, tpr, &GraphicsWidget::writeDot);
			_graphs.append(tpr);
		}
		mainLayout->addWidget(tpr, 2, 0, 3, 2);
	}
	this->setLayout(mainLayout);

	_timer = new QTimer();
	connect(_timer, &QTimer::timeout, this, &CT::redrawPlots);
	_timer->start(REDRAW_PERIOD);
}

CT::~CT()
{
	//qDeleteAll(vx);
	//qDeleteAll(vy);
	//qDeleteAll(ry);
	if (_timer != Q_NULLPTR)
		_timer->deleteLater();

	vx->clear();
	delete vx;

	_vals1->clear();
	delete _vals1;
	_vals2->clear();
	delete _vals2;
	_vals3->clear();
	delete _vals3;
	_vals4->clear();
	delete _vals4;

	_sma1->clear();
	delete _sma1;
	_sma2->clear();
	delete _sma2;
	_sma3->clear();
	delete _sma3;
	_sma4->clear();
	delete _sma4;

	/*_i1->clear();
	delete _i1;
	_i2->clear();
	delete _i2;
	_i3->clear();
	delete _i3;
	_i4->clear();
	delete _i4;

	_q1->clear();
	delete _q1;
	_q2->clear();
	delete _q2;
	_q3->clear();
	delete _q3;
	_q4->clear();
	delete _q4;*/

	_res1->clear();
	delete _res1;
	_res2->clear();
	delete _res2;
	_res3->clear();
	delete _res3;
	_res4->clear();
	delete _res4;
}

void CT::paintDot(quint64 id, qreal x, qreal y)
{
	if (id == 0)
	{
		emit writeDot(id, x, y);
	}
	/*else
	{
		if (id < 100)
		{
			id--;
			//qDebug() << " id " << id << " (" << x << ", " << y << ")";
			if (id < vx.size())
			{
				vy[id]->pop_front();
				vy[id]->append(y);
				//_graphs.at(id)->graph(0)->setData(*vx.at(id), *vy.at(id));
				//_graphs.at(id)->rescaleAxes();
				//_graphs.at(id)->replot();
			}
		}
		else
		{
			//qDebug() << " id " << id << " (" << x << ", " << y << ")";
			id = id - 101;
			ry[id]->pop_front();
			ry[id]->append(y);
			//_graphs.at(id)->graph(1)->setData(*vx.at(id), *ry.at(id));
			//_graphs.at(id)->rescaleAxes();
			//_graphs.at(id)->replot();
		}
	}*/
}


void CT::redrawPlots()
{
	if (true)//dataTouched)
	{
		qDebug() << "GUI: redraw, data touched" << QTime::currentTime().msecsSinceStartOfDay();
		_graphs.at(0)->graph(0)->setData(*vx, *_vals1);
		//_graphs.at(1)->graph(0)->setData(*vx, *_vals2);
		//_graphs.at(2)->graph(0)->setData(*vx, *_vals3);
		//_graphs.at(3)->graph(0)->setData(*vx, *_vals4);

		_graphs.at(0)->graph(1)->setData(*vx, *_sma1);
		//_graphs.at(1)->graph(1)->setData(*vx, *_sma2);
		//_graphs.at(2)->graph(1)->setData(*vx, *_sma3);
		//_graphs.at(3)->graph(1)->setData(*vx, *_sma4);

		_graphs.at(0)->graph(2)->setData(*vx, *_res1);
		//_graphs.at(1)->graph(2)->setData(*vx, *_res2);
		//_graphs.at(2)->graph(2)->setData(*vx, *_res3);
		//_graphs.at(3)->graph(2)->setData(*vx, *_res4);

		/*_graphs.at(0)->graph(2)->setData(*vx, *_i1);
		_graphs.at(1)->graph(2)->setData(*vx, *_i2);
		_graphs.at(2)->graph(2)->setData(*vx, *_i3);
		_graphs.at(3)->graph(2)->setData(*vx, *_i4);

		_graphs.at(0)->graph(3)->setData(*vx, *_res1);
		_graphs.at(1)->graph(3)->setData(*vx, *_res2);
		_graphs.at(2)->graph(3)->setData(*vx, *_res3);
		_graphs.at(3)->graph(3)->setData(*vx, *_res4);

		_graphs.at(0)->graph(4)->setData(*vx, *_q1);
		_graphs.at(1)->graph(4)->setData(*vx, *_q2);
		_graphs.at(2)->graph(4)->setData(*vx, *_q3);
		_graphs.at(3)->graph(4)->setData(*vx, *_q4);*/

		for (quint64 i = 0; i < 1/*4*/; i++)
		{
			_graphs.at(i)->rescaleAxes();
			_graphs.at(i)->replot();
		}
		dataTouched = false;
	}
	else
		qDebug() << "GUI: redraw, data NOT touched" << QTime::currentTime().msecsSinceStartOfDay();
}

void CT::paintGraphs(quint64 id, qreal val)
{
	qDebug() << "GUI: chenge data "<<QTime::currentTime().msecsSinceStartOfDay();
	dataTouched = true;
	if (id < 100)
	{
		id = id - 1;
		if (id == 0)
		{
			_vals1->append(val);
			_vals1->pop_front();
		}
		else if (id == 1)
		{
			_vals2->append(val);
			_vals2->pop_front();
		}
		else if (id == 2)
		{
			_vals3->append(val);
			_vals3->pop_front();
		}
		else if (id == 3)
		{
			_vals4->append(val);
			_vals4->pop_front();
		}
	}
	else if (id < 200)
	{
		id = id - 101;
		if (id == 0)
		{
			_sma1->append(val);
			_sma1->pop_front();
		}
		else if (id == 1)
		{
			_sma2->append(val);
			_sma2->pop_front();
		}
		else if (id == 2)
		{
			_sma3->append(val);
			_sma3->pop_front();
		}
		else if (id == 3)
		{
			_sma4->append(val);
			_sma4->pop_front();
		}
	}
	else if (id < 300)
	{
		id = id - 201;
		if (id == 0)
		{
			_res1->append(val);
			_res1->pop_front();
		}
		else if (id == 1)
		{
			_res2->append(val);
			_res2->pop_front();
		}
		else if (id == 2)
		{
			_res3->append(val);
			_res3->pop_front();
		}
		else if (id == 3)
		{
			_res4->append(val);
			_res4->pop_front();
		}
	}
}