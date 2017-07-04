#include "Target.h"

Target::Target()
{}

Target::~Target()
{
	/*_t->clear();
	delete _t;

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

	_i1->clear();
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
	delete _q4;

	_res1->clear();
	delete _res1;
	_res2->clear();
	delete _res2;
	_res3->clear();
	delete _res3;
	_res4->clear();
	delete _res4;
	*/
	if (_port != Q_NULLPTR)
		if (_port->isOpen())
			_port->close();
	//_port->deleteLater();

	emit workFinished();
}

void Target::init(QSerialPortInfo* inf)
{
	// com port init
	if (inf != Q_NULLPTR)
	{
		_portInfo = inf;
		_port = new QSerialPort(this);
		_port->setPort(*_portInfo);
		if (_port != Q_NULLPTR)
		{
			_port->setBaudRate(COM_TARGET_BAUDRATE);
			//_port->setFlowControl(QSerialPort::FlowControl::NoFlowControl);
			bool ok = _port->open(QSerialPort::ReadOnly);
			qDebug() << "Opening port = " << ok;
		}
		else
			emit logAny("Could not create COM port using info: " + inf->description());
	}
	else
		emit logAny("Serial port info is null while initialiazing COM port object!");
}

void Target::run()
{
	smaX.append(0);
	smaY.append(0);
	counter = 0;
	if (_port != Q_NULLPTR)
	{
		connect(_port, &QSerialPort::readyRead, this, &Target::readyRead);
		connect(_port, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error), this, &Target::error);
		connect(_port, &QSerialPort::aboutToClose, this, &Target::closed);
	}
}

void Target::readyRead()
{
	quint64 time = QTime::currentTime().msecsSinceStartOfDay();
	//if (ok && _port != Q_NULLPTR)
	if (_port != Q_NULLPTR)
	{
		while (_port->canReadLine())
		{
			QString dat = _port->readLine();
			//qDebug() << dat;
			QStringList ls = dat.split(COM_TARGET_DIVIDER);
			ls.removeAll("");
			if (ls.size() >= 5)
			{
				bool ok2, ok3, ok4, ok5, ok6;
				quint64 id = ls.at(0).toULongLong(&ok2);
				quint64 v0 = ls.at(1).toULongLong(&ok3);
				quint64 v1 = ls.at(2).toULongLong(&ok4);
				quint64 v2 = ls.at(3).toULongLong(&ok5);
				quint64 v3 = ls.at(4).toULongLong(&ok6);
				if (ok2 && ok3 && ok4 && ok5 && ok6)
					newValues(id, v0, v1, v2, v3, time);
				else
					emit logAny("Something went wrong while parsing input data from COM port:" + _port->portName());
			}
			else
				emit logAny("Wrong data count for COM port: " + _port->portName());
		}
	}
}

void Target::newValues(quint64 id, quint64 a, quint64 b, quint64 c, quint64 d, quint64 time)
{
	if (_id != id)
	{
		qDebug() << "New target id = " << id << " for COM port: " << _portInfo->portName();
		_id = id;
		qDebug() << "Start reading data from target #" << id << ", port = " << _portInfo->portName();
		emit connected(_id);
	}

	counter++;

	a = sma(&sma0, a) * CORR.at(_id).at(0);
	b = sma(&sma1, b) * CORR.at(_id).at(1);
	c = sma(&sma2, c) * CORR.at(_id).at(2);
	d = sma(&sma3, d) * CORR.at(_id).at(3);

	qreal sum = (a + b + c + d)*1.0;
	if (sum >= CORE_TARGET_THRESHOLD && counter % (smaPer) == 0)
	{
		counter = 0;
		qreal x = 0;
		qreal y = 0;
		x = ((c*1.0 + b*1.0) - (a*1.0 + d*1.0)) / sum;
		y = ((c*1.0 + d*1.0) - (a*1.0 + b*1.0)) / sum;
		x = sma2f(&smaX, x);
		y = sma2f(&smaY, y);
		emit newPosition(_id, x, y, time);
	}
	emit plotGraph0(_id, a);
	emit plotGraph1(_id, b);
	emit plotGraph2(_id, c);
	emit plotGraph3(_id, d);
}

qreal Target::sma2f(QVector<qreal> * v, qreal newV)
{
	qreal res = 0;
	if (v != Q_NULLPTR)
	{
		if (v->size() >= userSmaPer)
			v->pop_front();
		v->append(newV);

		for (quint64 k = 0; k < v->size(); k++)
			res += v->at(k);
		res /= v->size()*1.0;
	}
	return res;
}

quint16 Target::sma(QVector<quint16> * v, quint16 newV)
{
	quint64 res = 0;
	if (v != Q_NULLPTR)
	{
		if (v->size() >= smaPer)
			v->pop_front();
		v->append(newV);

		for (quint64 k = 0; k < v->size(); k++)
			res += v->at(k);
		res /= v->size();
	}
	return res;
}

void Target::error(QSerialPort::SerialPortError error)
{
	qDebug() << error;
}

void Target::closed()
{
	emit disconnected(_id);
	emit disconnectedInf(_portInfo);
}

void Target::setCorrection(QList<QList<qreal>> v)
{
	for (quint64 i = 0; i<6; i++)
		for (quint64 j = 0; j < 5; j++)
		{
			CORR[i][j] = v[i][j];
		}

};

void Target::setSmaPer(quint64 sma)
{
	smaPer = sma;
}

void Target::setUserSmaPer(quint64 sma)
{
	userSmaPer = sma;
}

void Target::setMulti(qreal val, quint64 id)
{
	if (id == _id)
		multic = val;
}