#include "Target.h"

Target::Target()
{}

Target::~Target()
{
emit workFinished();
_t->clear();
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
}

void Target::init(QSerialPortInfo* inf)
{
	// com port init
	if (inf != Q_NULLPTR)
	{
		_portInfo = inf;
		_port = new QSerialPort(*_portInfo);
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

	// filter init
	lastTime = 0;

	_timer2 = new QTimer();
	_timer = new QTimer();

	_vals1 = new QVector<int>(CORE_DATA_SIZE);
	_vals2 = new QVector<int>(CORE_DATA_SIZE);
	_vals3 = new QVector<int>(CORE_DATA_SIZE);
	_vals4 = new QVector<int>(CORE_DATA_SIZE);

	_t = new QVector<quint64>(CORE_DATA_SIZE);

	_sma1 = new QVector<qreal>(CORE_DATA_SIZE);
	_sma2 = new QVector<qreal>(CORE_DATA_SIZE);
	_sma3 = new QVector<qreal>(CORE_DATA_SIZE);
	_sma4 = new QVector<qreal>(CORE_DATA_SIZE);

	_i1 = new QVector<qreal>(CORE_DATA_SIZE*CORE_IQGETER_MULT);
	_i2 = new QVector<qreal>(CORE_DATA_SIZE*CORE_IQGETER_MULT);
	_i3 = new QVector<qreal>(CORE_DATA_SIZE*CORE_IQGETER_MULT);
	_i4 = new QVector<qreal>(CORE_DATA_SIZE*CORE_IQGETER_MULT);

	_q1 = new QVector<qreal>(CORE_DATA_SIZE*CORE_IQGETER_MULT);
	_q2 = new QVector<qreal>(CORE_DATA_SIZE*CORE_IQGETER_MULT);
	_q3 = new QVector<qreal>(CORE_DATA_SIZE*CORE_IQGETER_MULT);
	_q4 = new QVector<qreal>(CORE_DATA_SIZE*CORE_IQGETER_MULT);

	_res1 = new QVector<qreal>(CORE_DATA_SIZE);
	_res2 = new QVector<qreal>(CORE_DATA_SIZE);
	_res3 = new QVector<qreal>(CORE_DATA_SIZE);
	_res4 = new QVector<qreal>(CORE_DATA_SIZE);

	_buf = new QVector<qreal>(CORE_DATA_SIZE*CORE_IQGETER_MULT);

	//_kalman.KalmanInit(10, 1, 10, 0);
	
}

void Target::run()
{
	connect(_timer, &QTimer::timeout, this, &Target::processTimer);
	connect(_timer2, &QTimer::timeout, this, &Target::processTimer2);
	if (_port != Q_NULLPTR)
	{
		connect(_port, &QSerialPort::readyRead, this, &Target::readyRead);
		connect(_port, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error), this, &Target::error);
	}
	_timer->start(CORE_TARGET_TIMER);
	_timer2->start(CORE_PLOT_TIMER);
}

void Target::newShot()
{}

void Target::newValues(quint64 id, quint64 a, quint64 b, quint64 c, quint64 d, quint64 time)
{
	//qDebug() << time - lastTime;
	//lastTime = time;
	//qDebug() << time << " " << a << " " << b << " " << c << " " << d;
	if (dataCount <= CORE_DATA_SIZE)
		dataCount++;

	dataTouched = true;

	//if (lastTime == 0)
		lastTime = time;
	//else
		//lastTime += COM_READ_TIMER;
	_t->append(lastTime);
	//if (_t->size() > CORE_DATA_SIZE)
	_t->pop_front();

	_vals1->append(a);
	//if (_vals1->size() > CORE_DATA_SIZE)
	_vals1->pop_front();

	_vals2->append(b);
	//if (_vals2->size() > CORE_DATA_SIZE)
	_vals2->pop_front();

	_vals3->append(c);
	//if (_vals3->size() > CORE_DATA_SIZE)
	_vals3->pop_front();

	_vals4->append(d);
	//if (_vals4->size() > CORE_DATA_SIZE)
	_vals4->pop_front();

	// Calc sma and carrier
	_sma1->append(SMA(_vals1, _vals1->size() - 1, CORE_SMA_PER));
	_sma1->pop_front();

	_sma2->append(SMA(_vals2, _vals2->size() - 1, CORE_SMA_PER));
	_sma2->pop_front();

	_sma3->append(SMA(_vals3, _vals3->size() - 1, CORE_SMA_PER));
	_sma3->pop_front();

	_sma4->append(SMA(_vals4, _vals4->size() - 1, CORE_SMA_PER));
	_sma4->pop_front();

	//computeLastPeriod();
	//if (stamp != 0)
	//{
	// results
	/*		i = I(time);
	q = Q(time);

	_q1->append(q*a); /// COM_ADC_POWER);
	_q1->pop_front();

	_q2->append(q*b / COM_ADC_POWER);
	_q2->pop_front();

	_q3->append(q*c / COM_ADC_POWER);
	_q3->pop_front();

	_q4->append(q*d / COM_ADC_POWER);
	_q4->pop_front();

	_i1->append(i*a);// / COM_ADC_POWER);
	_i1->pop_front();

	_i2->append(i*b / COM_ADC_POWER);
	_i2->pop_front();

	_i3->append(i*c / COM_ADC_POWER);
	_i3->pop_front();

	_i4->append(i*d / COM_ADC_POWER);
	_i4->pop_front();*/

	//_res1->append(sqrt(i*i*a*a + q*q*a*a));

	/*I(_vals1, _i1);
	I(_vals2, _i2);
	I(_vals3, _i3);
	I(_vals4, _i4);

	Q(_vals1, _q1);
	Q(_vals2, _q2);
	Q(_vals3, _q3);
	Q(_vals4, _q4);
	*/

	//_res1->append(_kalman.GetResult(_vals1->last()));
	//_res1->append(_vals1->last());
	//_res1->pop_front();

	//_res2->append(sqrt(i*i*b*b + q*q*b*b));
	//_res2->append(_kalman.GetResult(_vals2->last()));
	//_res2->append(_vals2->last());
	//_res2->pop_front();

	//_res3->append(sqrt(i*i*c*c + q*q*c*c));
	//_res3->append(_kalman.GetResult(_vals3->last()));
	//_res3->append(_vals4->last());
	//_res3->pop_front();

	//_res4->append(sqrt(i*i*d*d + q*q*d*d));
	//_res4->append(_kalman.GetResult(_vals4->last()));
	//_res4->append(_vals4->last());
	//_res4->pop_front();

	//qDebug() << " V = " << _vals1->last() << " I = " << _i1->last() << " Q = "<<_q1->last() << " RES = " << _res1->last();

	// using wavelet
	/*string nm = CORE_WAVELET_NAME; // nm will store the name of Wavelet Family
	vector<double> sig;
	sig.clear();
	for (quint64 i=0; i<_vals1->size(); i++)
	sig.push_back(_vals1->at(i)*1.0); //input values

	vector<double> original;
	original = sig;

	int J = CORE_WAVELET_STAGES;

	vector<double> dwt_output, flag;

	vector<int> length;
	length.clear();
	length.reserve(dwt_output.size());
	//Perform J-Level DWT
	dwt_sym(sig, J, nm, dwt_output, flag, length);

	//Perform J-Level IDWT
	vector<double> output;
	output.clear();
	output.reserve(dwt_output.size());
	idwt_sym(dwt_output, flag, nm, output, length);

	_res1->clear();
	for (quint64 i = 0; i < output.size(); i++)
	_res1->push_back(output.at(i));
	//}

	string nm = CORE_WAVELET_NAME;
	vector<double> sig;
	sig.clear();
	for (quint64 i = 0; i<_vals1->size(); i++)
	sig.push_back(_vals1->at(i)*1.0); //input values

	vector<double> original;
	original = sig;

	int J = CORE_WAVELET_STAGES;
	vector<double> dwt_output, flag;
	vector<int> length1;

	// perform J-Level DWT
	dwt(sig, J, nm, dwt_output, flag, length1);

	// Performing Linear Approximation by using only first 100 coefficients
	// Coefficients in dwt_output are stored as following
	// dwt_output =[ Appx(J-1) Detail(J-1) Detail(J-2) .... Detail(0)]

	int n_coef = 100; // Number of significant coefficients

	int n_non_sig = dwt_output.size() - n_coef; // Number of Coefficients that will
	// be set to zero
	dwt_output.erase(dwt_output.end() - n_non_sig, dwt_output.end());
	// Deleting last n_non_sig coefficients and replacing them with zeros
	dwt_output.insert(dwt_output.end(), n_non_sig, 0);
	// Finding IDWT with approximated coefficients
	vector<double> output;
	idwt(dwt_output, flag, nm, output, length1);

	// Performing Non Linear Approximation by using only most
	// significant coefficients

	vector<double> dwt_output2, flag2;
	vector<int> length2;

	// perform J-Level DWT
	dwt(sig, J, nm, dwt_output2, flag2, length2);

	double thresh = 0.0;

	vector<double> temp_dwtoutput;
	for (unsigned int i = 0; i < dwt_output2.size(); i++) {
	double temp = abs(dwt_output2[i]);
	temp_dwtoutput.push_back(temp);
	}
	/*
	for (unsigned int i =0; i < temp_dwtoutput.size(); i++){
	cout << temp_dwtoutput[i] << endl;
	}


	findthresh(temp_dwtoutput, n_coef, thresh);

	for (unsigned int i = 0; i < dwt_output2.size(); i++) {
	double temp = abs(dwt_output2[i]);
	if (temp < thresh) {
	dwt_output2.at(i) = 0.0;

	}
	}
	/*
	for (unsigned int i =0; i < dwt_output2.size(); i++){
	cout << dwt_output2[i] << endl;
	}
	*/
	/*
	// Finding IDWT with approximated coefficients

	vector<double> output2;
	idwt(dwt_output2, flag2, nm, output2, length2);
	_res1->clear();
	for (quint64 i = 0; i < output2.size(); i++)
	_res1->push_back(output2.at(i));
	*/

	waveFileter(_vals1, _res1);
	waveFileter(_vals2, _res2);
	waveFileter(_vals3, _res3);
	waveFileter(_vals4, _res4);
}

void Target::processTimer()
{
	/*
	a b
	c d
	*/
	qreal a = lastAmp(_res1, _sma1);
	qreal b = lastAmp(_res2, _sma2);
	qreal c = lastAmp(_res3, _sma3);
	qreal d = lastAmp(_res4, _sma4);
	qreal x = 0;
	qreal y = 0;
	if (a + b + c + d != 0)
	{
		x = ((b + c) - (b + d)) / (a + b + c + d);
		y = ((a + b) - (c + d)) / (a + b + c + d);
		//qDebug() << "(" << x*COM_ADC_POWER << ", " << y*COM_ADC_POWER << ")";
		emit paintDot(0, x*COM_ADC_POWER, y*COM_ADC_POWER);
	}
	//emit paintDot(0, x, y, 0);
}

/*int Target::_lastAmp(QVector<int>* _vals)
{
int max1 = 0;
int pos = 0;
//int max2 = 0;
int min1 = 0;
//int min2 = 0;
bool ok;

if (_vals != Q_NULLPTR && _vals->size() > CORE_CHECK_BUF * 10)
{
ok = false;
// найдем первые min и max
pos = _vals->size() - CORE_CHECK_BUF - 2;
max1 = _vals->at(pos);
while (!ok && pos > CORE_CHECK_BUF)
{
// max
ok = true;
for (quint64 i = 1; i <= CORE_CHECK_BUF; i++)
{
//if (!((_vals->at(pos - i) < _vals->at(pos) && _vals->at(pos + i) < _vals->at(pos)) && (_vals->at(pos - i) < (_vals->at(pos - i + 1) + CORE_CHECK_NOISE_MARGIN) && _vals->at(pos + i) < (_vals->at(pos + i - 1)+CORE_CHECK_NOISE_MARGIN))))
if (!(_vals->at(pos) - _vals->at(pos - i) > CORE_CHECK_NOISE_MARGIN && _vals->at(pos) - _vals->at(pos + i) > CORE_CHECK_NOISE_MARGIN))
ok = false;
}
if (!ok)
{
pos--;
max1 = _vals->at(pos);
}
}
if (pos <= CORE_CHECK_BUF)
//qDebug() << "Error while found max";

// min
pos = _vals->size() - CORE_CHECK_BUF - 2;
min1 = _vals->at(pos);
while (!ok && pos > CORE_CHECK_BUF)
{
// max
ok = true;
for (quint64 i = 1; i <= CORE_CHECK_BUF; i++)
{
//if (!((_vals->at(pos - i) > _vals->at(pos) && _vals->at(pos + i) > _vals->at(pos)) && (_vals->at(pos - i) > (_vals->at(pos - i + 1) - CORE_CHECK_NOISE_MARGIN) && _vals->at(pos + i) > (_vals->at(pos + i - 1) - CORE_CHECK_NOISE_MARGIN))))
if (!(_vals->at(pos - i) - _vals->at(pos) > CORE_CHECK_NOISE_MARGIN && _vals->at(pos + i) - _vals->at(pos) > CORE_CHECK_NOISE_MARGIN))
ok = false;
}
if (!ok)
{
pos--;
min1 = _vals->at(pos);
}
}
//if (pos <= CORE_CHECK_BUF)
//qDebug() << "Error while found min";
}
return max1 - min1;
}*/

void Target::processTimer2()
{
	if (true)//dataTouched && _vals1->size() > 0)
	{
		//qDebug() << "Core: redraw, data touched" << QTime::currentTime().msecsSinceStartOfDay();
		emit paintGraphs(1, _vals1->last());
		emit paintGraphs(2, _vals2->last());
		emit paintGraphs(3, _vals3->last());
		emit paintGraphs(4, _vals4->last());
		// sma

		emit paintGraphs(101, _sma1->last());
		emit paintGraphs(102, _sma2->last());
		emit paintGraphs(103, _sma3->last());
		emit paintGraphs(104, _sma4->last());

		/*/ i
		emit paintGraphs(201, _i1->last());
		emit paintGraphs(202, _i2->last());
		emit paintGraphs(203, _i3->last());
		emit paintGraphs(204, _i4->last());*/

		// res

		emit paintGraphs(201, _res1->last());
		emit paintGraphs(202, _res2->last());
		emit paintGraphs(203, _res3->last());
		emit paintGraphs(204, _res4->last());

		/*	// q
		emit paintGraphs(401, _q1->last());
		emit paintGraphs(402, _q2->last());
		emit paintGraphs(403, _q3->last());
		emit paintGraphs(404, _q4->last());*/

		dataTouched = false;
	}
	else
		qDebug() << "Core: redraw, data NOT touched" << QTime::currentTime().msecsSinceStartOfDay();
}

qreal Target::SMA(QVector<int>* v, quint64 pos, quint64 per)
{
	qreal res = 0;
	if (v != Q_NULLPTR && v->size() > pos)
	{
		for (quint64 i = pos - per + 1; i <= pos; i++)
		{
			res += v->at(i);
		}
		res = res / per;
	}
	//qDebug() <<QTime::currentTime().msecsSinceStartOfDay() << " SMA = " << res << " | " << res*per;
	return res;
}

void Target::computeLastPeriod()
{
	/*if (period == 0 || stamp == 0)
	{
	quint64 pmax = 0; // local max
	quint64 pmin = 0; // local min
	quint64 t1 = 0; // first sma==val
	quint64 t2 = 0; // second sma==val
	quint64 t3 = 0; // third sma==val
	quint64 newStamp = 0;
	quint64 newPeriod = 0;
	quint64 s1 = 0;
	quint64 s2 = 0;
	quint64 k = 0;
	if (dataCount > CORE_DATA_SIZE)
	{
	// 1st
	pmax = 0; // local max
	pmin = 0; // local min
	t1 = 0; // first sma==val
	t2 = 0; // second sma==val
	t3 = 0; // third sma==val
	newStamp = 0;
	newPeriod = 0;
	QList<quint64> p1;
	QList<quint64> p2;
	QList<quint64> p3;
	quint64 sum=0;
	for (quint64 i = _vals1->size() - 1; i > 0; i--)
	{
	if (_sma1->at(i) == _vals1->at(i)
	|| (_sma1->at(i) < _vals1->at(i) && _sma1->at(i - 1) > _vals1->at(i - 1))
	|| (_sma1->at(i) > _vals1->at(i) && _sma1->at(i - 1) < _vals1->at(i - 1)))
	{
	if (t1 == 0)
	{
	// if it is last part of sin
	if (_vals1->at(i - 1) < _vals1->at(i))
	t1 = i;
	}
	else if (t2 == 0)
	t2 = i;
	else if (t3 == 0)
	{
	t3 = i;
	//qDebug() << t3 << " " << t2 << " " << t1;
	// we found all 3 points
	pmin = posOfMin(_vals1, t2, t1);
	pmax = posOfMax(_vals1, t3, t2);
	if (pmax!=0 && pmin!=0 && _vals1->at(pmax) - _vals1->at(pmin) >= CORE_PEAK_TRESH)
	{
	//qDebug() << "Checked";
	p1.append(pmin);
	p2.append(pmax);
	p3.append(_t->at(pmin) - _t->at(pmax));
	sum += _t->at(pmin) - _t->at(pmax);
	//break;
	t1 = 0;
	t2 = 0;
	t3 = 0;
	}
	}
	}
	}
	if (p1.size() > 0)
	{
	quint64 res = sum/p3.count();
	period = res;
	// вычислим начало периода
	quint64 bestPos=0;
	quint64 lastSr = 0;
	quint64 ls = 0;
	stamp = 0;
	for (quint64 i = 0; i < period; i++)
	{
	ls = 0;
	for (quint64 j = 0; j < p1.size(); j++)
	{
	if (_t->at(p1.at(j)) % period == i)
	ls++;
	if (_t->at(p2.at(j)) % period == i)
	ls++;
	}
	if (ls > lastSr)
	{
	qDebug() << "ls = " << ls;
	bestPos = i;
	lastSr = ls;
	}
	}
	stamp = bestPos;
	qDebug() << stamp;
	}
	/*
	if (_vals1->at(pmax) - _vals1->at(pmin) >= CORE_PEAK_TRESH)
	{
	newPeriod = _t->at(pmin) - _t->at(pmax);
	newPeriod = (period + newPeriod) / 2;
	if (newPeriod != 0)
	{
	newStamp = _t->at(pmax) - (newPeriod / 2);
	if ((newStamp - stamp) % newPeriod != 0)
	qDebug() << "Stamp mismath: old = " << stamp << " new = " << newStamp << " old period = " << period << " new period = " << newPeriod;

	s1 += newPeriod;
	s2 += newStamp;
	k++;
	}
	else
	qDebug() << "New period = 0, pMin = " << pmin << ", pmax = " << pmax;
	}*/
	// 2nd 
	/*pmax = 0; // local max
	pmin = 0; // local min
	t1 = 0; // first sma==val
	t2 = 0; // second sma==val
	t3 = 0; // third sma==val
	newStamp = 0;
	newPeriod = 0;
	for (quint64 i = _vals2->size() - 1; i > 0; i--)
	{
	if (_sma2->at(i) == _vals2->at(i)
	|| (_sma2->at(i) < _vals2->at(i) && _sma2->at(i - 1) > _vals2->at(i - 1))
	|| (_sma2->at(i) > _vals2->at(i) && _sma2->at(i - 1) < _vals2->at(i - 1)))
	{
	if (t1 == 0)
	{
	// if it is last part of sin
	if (_vals2->at(i - 1) < _vals2->at(i))
	t1 = i;
	}
	else if (t2 == 0)
	t2 = i;
	else if (t3 == 0)
	{
	t3 = i;
	// we found all 3 points
	pmin = posOfMin(_vals2, t2, t3);
	pmax = posOfMax(_vals2, t1, t2);
	if (_vals2->at(pmax) - _vals2->at(pmin) >= CORE_PEAK_TRESH)
	break;
	}
	}
	}
	if (_vals2->at(pmax) - _vals2->at(pmin) >= CORE_PEAK_TRESH)
	{
	newPeriod = _t->at(pmin) - _t->at(pmax);
	newPeriod = (period + newPeriod) / 2;
	newStamp = _t->at(pmax) - (newPeriod / 2);
	if ((newStamp - stamp) % newPeriod != 0)
	qDebug() << "Stamp mismath: old = " << stamp << " new = " << newStamp << " old period = " << period << " new period = " << newPeriod;

	s1 += newPeriod;
	s2 += newStamp;
	k++;
	}
	// 3rd
	pmax = 0; // local max
	pmin = 0; // local min
	t1 = 0; // first sma==val
	t2 = 0; // second sma==val
	t3 = 0; // third sma==val
	newStamp = 0;
	newPeriod = 0;
	for (quint64 i = _vals3->size() - 1; i > 0; i--)
	{
	if (_sma3->at(i) == _vals3->at(i)
	|| (_sma3->at(i) < _vals3->at(i) && _sma3->at(i - 1) > _vals3->at(i - 1))
	|| (_sma3->at(i) > _vals3->at(i) && _sma3->at(i - 1) < _vals3->at(i - 1)))
	{
	if (t1 == 0)
	{
	// if it is last part of sin
	if (_vals3->at(i - 1) < _vals3->at(i))
	t1 = i;
	}
	else if (t2 == 0)
	t2 = i;
	else if (t3 == 0)
	{
	t3 = i;
	// we found all 3 points
	pmin = posOfMin(_vals3, t2, t3);
	pmax = posOfMax(_vals3, t1, t2);
	if (_vals3->at(pmax) - _vals3->at(pmin) >= CORE_PEAK_TRESH)
	break;
	}
	}
	}
	if (_vals3->at(pmax) - _vals3->at(pmin) >= CORE_PEAK_TRESH)
	{
	newPeriod = _t->at(pmin) - _t->at(pmax);
	newPeriod = (period + newPeriod) / 2;
	if (newPeriod != 0)
	{
	newStamp = _t->at(pmax) - (newPeriod / 2);
	if ((newStamp - stamp) % newPeriod != 0)
	qDebug() << "Stamp mismath: old = " << stamp << " new = " << newStamp << " old period = " << period << " new period = " << newPeriod;

	s1 += newPeriod;
	s2 += newStamp;

	k++;
	}
	}

	// 4th
	pmax = 0; // local max
	pmin = 0; // local min
	t1 = 0; // first sma==val
	t2 = 0; // second sma==val
	t3 = 0; // third sma==val
	newStamp = 0;
	newPeriod = 0;
	for (quint64 i = _vals4->size() - 1; i > 0; i--)
	{
	if (_sma4->at(i) == _vals4->at(i)
	|| (_sma4->at(i) < _vals4->at(i) && _sma4->at(i - 1) > _vals4->at(i - 1))
	|| (_sma4->at(i) > _vals4->at(i) && _sma4->at(i - 1) < _vals4->at(i - 1)))
	{
	if (t1 == 0)
	{
	// if it is last part of sin
	if (_vals4->at(i - 1) < _vals4->at(i))
	t1 = i;
	}
	else if (t2 == 0)
	t2 = i;
	else if (t3 == 0)
	{
	t3 = i;
	// we found all 3 points
	pmin = posOfMin(_vals4, t2, t3);
	pmax = posOfMax(_vals4, t1, t2);
	if (_vals4->at(pmax) - _vals4->at(pmin) >= CORE_PEAK_TRESH)
	break;
	}
	}
	}
	if (_vals4->at(pmax) - _vals4->at(pmin) >= CORE_PEAK_TRESH)
	{
	newPeriod = _t->at(pmin) - _t->at(pmax);
	newPeriod = (period + newPeriod) / 2;
	if (newPeriod != 0)
	{
	newStamp = _t->at(pmax) - (newPeriod / 2);
	if ((newStamp - stamp) % newPeriod != 0)
	qDebug() << "Stamp mismath: old = " << stamp << " new = " << newStamp << " old period = " << period << " new period = " << newPeriod;

	s1 += newPeriod;
	s2 += newStamp;
	k++;
	}
	}
	if (k != 0)
	{
	period = s1 / k;
	stamp = s2 / k;
	}
	}
	}*/
}

int Target::posOfMax(QVector<qreal>* v, quint64 b, quint64 e)
{
	quint64 res = b;
	if (v != Q_NULLPTR)
	{
		if (b < e && e < v->size())
			for (quint64 i = b + 1; i < e; i++)
				if (v->at(res) < v->at(i) && v->at(i) > 0)
					res = i;
	}
	return res;
}

int Target::posOfMin(QVector<qreal>* v, quint64 b, quint64 e)
{
	quint64 res = b;
	if (v != Q_NULLPTR)
	{
		if (b < e && e < v->size())
			for (quint64 i = b + 1; i < e; i++)
				if (v->at(res) > v->at(i))
					res = i;
	}
	return res;
}

void Target::I(QVector<int>* in, QVector<qreal>* out)
{
	/*time = time - stamp;
	time = time % period;
	qreal res;
	qreal t = time*_PI / (period*1.0);
	res = cos(2 * _PI*CORE_SIGN_FR*time);
	//res *= COM_ADC_POWER;
	qDebug() << " I = " <<  res << " t = "<<t;*/
	qreal res;
	qreal t;
	if (in != Q_NULLPTR && out != Q_NULLPTR)
	{
		for (quint64 i = 0; i < CORE_IQGETER_MULT; i++)
		{
			t = QTime::currentTime().msecsSinceStartOfDay() % CORE_MSECS_IN_SEC;
			t = t * 2 * _PI;
			t = t / (CORE_MSECS_IN_SEC * 1.0);
			t = t * (CORE_IQGETER_FR*1.0);
			out->append(cos(t)*(in->last()*1.0f));

			//return res;
		}
		for (quint64 i = 0; i < CORE_IQGETER_MULT; i++)
		{
			out->pop_front();
		}
	}
}

void Target::Q(QVector<int>* in, QVector<qreal>* out)
{
	/*time = time - stamp;
	time = time % period;
	qreal t = time*_PI / (period*1.0);
	qreal res;
	res = sin(2 * _PI*CORE_SIGN_FR*time);
	qDebug() << " Q = " << res << " t = " << t;
	//res *= COM_ADC_POWER;
	return res;*/
	qreal res;
	qreal t;
	if (in != Q_NULLPTR && out != Q_NULLPTR)
	{
		for (quint64 i = 0; i < CORE_IQGETER_MULT; i++)
		{
			t = QTime::currentTime().msecsSinceStartOfDay() % CORE_MSECS_IN_SEC;
			t = t * 2 * _PI;
			t = t / (CORE_MSECS_IN_SEC * 1.0);
			t = t * (CORE_IQGETER_FR*1.0);
			out->append(sin(t)*(in->last()*1.0f));

			//return res;
		}
		for (quint64 i = 0; i < CORE_IQGETER_MULT; i++)
		{
			out->pop_front();
		}
	}
}

qreal Target::lastAmp(QVector<qreal>* v, QVector<qreal>* sma)
{
	qreal res = 0;
	if (v != Q_NULLPTR && sma != Q_NULLPTR)
	{
		quint64 t1 = 0;
		quint64 t2 = 0;
		quint64 t3 = 0;
		quint64 pmax = 0; // local max
		quint64 pmin = 0; // local min
		quint64 count = 0;
		for (quint64 i = _vals1->size() - 1; i > 0; i--)
		{
			if (sma->at(i) == v->at(i)
				|| (sma->at(i) < v->at(i) && sma->at(i - 1) > v->at(i - 1))
				|| (sma->at(i) > v->at(i) && sma->at(i - 1) < v->at(i - 1)))
			{
				if (t1 == 0)
				{
					// if it is last part of sin
					if (v->at(i - 1) < v->at(i))
						t1 = i;
				}
				else if (t2 == 0)
					t2 = i;
				else if (t3 == 0)
				{
					t3 = i;
					pmin = posOfMin(v, t2, t1);
					pmax = posOfMax(v, t3, t2);
					if (pmax != 0 && pmin != 0)
					{
						res += v->at(pmax) - v->at(pmin);
						count++;
					}
					t1 = 0;
					t2 = 0;
					t3 = 0;
				}
			}
			if (count == CORE_AMP_COUNT)
				break;
		}
	}
	return res / (CORE_AMP_COUNT*1.0);
}

// another author

void Target::CIC(QVector<qreal>* in, QVector<qreal>* out, int R)
{
	if (in != Q_NULLPTR && out != Q_NULLPTR)
	{
		qreal y1_old = 0;
		qreal y2_old = 0;
		qreal y3_old = 0;
		qreal y4_old = 0;
		qreal y5_old = 0;

		qreal z1_old = 0;
		qreal z2_old = 0;
		qreal z3_old = 0;
		qreal z4_old = 0;
		qreal z5_old = 0;

		qreal z1 = 0;
		qreal z2 = 0;
		qreal z3 = 0;
		qreal z4 = 0;
		qreal z5 = 0;
		qreal z6 = 0;

		qreal y1 = 0;
		qreal y2 = 0;
		qreal y3 = 0;
		qreal y4 = 0;
		qreal y5 = 0;

		int k = 0;

		for (int i = 0; i < in->size(); i++)
		{
			y1 = y1_old + in->at(i);
			y2 = y2_old + y1;
			y3 = y3_old + y2;
			y4 = y4_old + y3;
			y5 = y5_old + y4;

			y1_old = y1;
			y2_old = y2;
			y3_old = y3;
			y4_old = y4;
			y5_old = y5;

			if (i % R == 0)
				z1 = y4;

			z2 = z1 - z1_old;
			z3 = z2 - z2_old;
			z4 = z3 - z3_old;
			z5 = z4 - z4_old;
			z6 = z5 - z5_old;

			z1_old = z1;
			z2_old = z2;
			z3_old = z3;
			z4_old = z4;
			z5_old = z5;

			k = k + 1;
			out->append(z5);
		}
	}
}

// another author
void Target::FIR(QVector<qreal>* in, QVector<qreal>* out)
{
	const int N = CORE_FIR_DEPTH; //Длина фильтра
	long double Fd = CORE_IQGETER_FR; //Частота дискретизации входных данных
	long double Fs = CORE_FIR_BAND_FREQ; //Частота полосы пропускания
	long double Fx = CORE_FIR_ATTUN_FREQ; //Частота полосы затухания

	long double H[N] = { 0 }; //Импульсная характеристика фильтра
	long double H_id[N] = { 0 }; //Идеальная импульсная характеристика
	long double W[N] = { 0 }; //Весовая функция

							  //Расчет импульсной характеристики фильтра
	qreal Fc = (Fs + Fx) / (2 * Fd);

	for (int i = 0; i < N; i++)
	{
		if (i == 0) H_id[i] = 2 * _PI*Fc;
		else H_id[i] = sinl(2 * _PI*Fc*i) / (_PI*i);
		// весовая функция Блекмена
		W[i] = 0.42 - 0.5 * cosl((2 * _PI*i) / (N - 1)) + 0.08 * cosl((4 * _PI*i) / (N - 1));
		H[i] = H_id[i] * W[i];
	}

	//Нормировка импульсной характеристики
	qreal SUM = 0;
	for (int i = 0; i < N; i++) SUM += H[i];
	for (int i = 0; i < N; i++) H[i] /= SUM; //сумма коэффициентов равна 1 

											 //Фильтрация входных данных
	for (int i = 0; i < in->size(); i++)
	{
		(*out)[i] = 0.;
		for (int j = 0; j < N - 1; j++)// та самая формула фильтра
			(*out)[i] += H[j] * (*in)[i - j];
	}
}

void Target::findthresh(vector<double> vector1, int N, double& t) {
	sort(vector1.begin(), vector1.end(), [](double x, double y) {return x > y; });
	t = vector1.at(N - 1);
}

void Target::waveFileter(QVector<int>* in, QVector<double>* out)
{
	if (in != Q_NULLPTR && out != Q_NULLPTR)
	{
		out->clear();
		// using wavelet
		string nm = CORE_WAVELET_NAME; // nm will store the name of Wavelet Family
		vector<double> sig;
		sig.clear();
		for (quint64 i = 0; i<in->size(); i++)
			sig.push_back(in->at(i)*1.0); //input values

		vector<double> original;
		original = sig;

		int J = CORE_WAVELET_STAGES;

		vector<double> dwt_output, flag;

		vector<int> length;
		length.clear();
		length.reserve(dwt_output.size());
		//Perform J-Level DWT
		dwt_sym(sig, J, nm, dwt_output, flag, length);

		qreal sigma = CORE_WAVELET_FILTER_MARGIN;
		qreal val = 0;
		// filter coef
		for (quint64 i = 0; i < dwt_output.size(); i++)
		{
			val = dwt_output.at(i);
			/*if (val < 0)
			{
			if (val + sigma < 0)
			dwt_output[i] += sigma;
			else
			dwt_output[i] =0;
			}
			else
			{
			if (val - sigma > 0)
			dwt_output[i] -= sigma;
			else
			dwt_output[i] = 0;
			}*/
			if (val<sigma)
				dwt_output[i] = 0;
		}

		//Perform J-Level IDWT
		vector<double> output;
		output.clear();
		output.reserve(dwt_output.size());
		idwt_sym(dwt_output, flag, nm, output, length);

		out->clear();
		for (quint64 i = 0; i < output.size(); i++)
			out->push_back(output.at(i));
	}
};

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
					emit newValues(id, v0, v1, v2, v3, time);
				else
					emit logAny("Something went wrong while parsing input data from COM port:" + _port->portName());
			}
			else
				emit logAny("Wrong data count for COM port: " + _port->portName());
		}
	}
}

void Target::error(QSerialPort::SerialPortError error)
{
	qDebug() << error;
}