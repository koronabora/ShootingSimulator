#include "ControllerHelper.h"

ControllerHelper::ControllerHelper()
{}

ControllerHelper::~ControllerHelper()
{
	if (_timer != Q_NULLPTR)
		_timer->deleteLater();
	if (_inTimer != Q_NULLPTR)
		_inTimer->deleteLater();
	if (_port != Q_NULLPTR)
	{
		if (_port->isOpen())
			_port->close();
		_port->deleteLater();
	}
	emit workFinished();
}

void ControllerHelper::init()
{
	quint64 t = QTime::currentTime().msecsSinceStartOfDay();
	_timer = new QTimer();
	_inTimer = new QTimer();

	for (quint64 i = 0; i < KE_BOOM_PORTS; i++)
	{
	}
	_lastInput = "000000xxxxxxxxxxxx";
	for (quint64 i = 0; i < KE_IN_PORTS; i++)
	{
		_tarStates.append(false);
		_lastShots.append(t);
		_boomStates.append(false);
		_relStates.append(false);
		_buttonStates.append(false);
		_lastBooms.append(t);
	}
}

void ControllerHelper::checkNewTargets()
{
	QSerialPortInfo inf;
	QList<QSerialPortInfo> ports = inf.availablePorts();

	for (quint64 i = 0; i < ports.size(); i++)
	{
		//qDebug() << ports.at(i).description();
		QSerialPortInfo* info = new QSerialPortInfo(ports.at(i));
		if (ports.at(i).description().compare(COM_TARGET_NAME) == 0 && !_inf.contains(info->portName()))
		{
			qDebug() << "Found new target: " << info->portName();
			_inf.append(ports.at(i).portName());
			//QSerialPortInfo* info = new QSerialPortInfo(ports.at(i));
			emit targetConnected(info);
		}
		else if ((!contrlConnected) && (ports.at(i).description().compare(COM_CONTROLLER_NAME) == 0 || ports.at(i).description().compare(COM_CONTROLLER_NAME2) == 0))
		{
			if (!_inf.contains(ports.at(i).portName()))
			{
				_inf.append(ports.at(i).portName());
				contrlConnected = true;
				qDebug() << "Controller found: " << COM_CONTROLLER_NAME;
				controllerConnected(info);
			}
		}
		else
			delete info;
	}
}

void ControllerHelper::checkButtonValues()
{
	if (!keInUse)
	{
		quint64 time = QTime::currentTime().msecsSinceStartOfDay();
		bool ok = false;
		for (quint64 i = 0; i < KE_IN_PORTS; i++)
		{
			if (_buttonStates.at(i) && BURST_AVALIABLE.at(i) && (_lastShots.at(i)+ SHOTS_DELAY.at(i) <= time)) // выстрел отдачей
			{
				ok = true;
				processShot(i, true);
			}
			else
			if (_relStates.at(i) && (_lastBooms.at(i) < time - BOOM_DELAY.at(i))) // закроем реле
			{
				_lastBooms[i] = time;
				//_buttonStates[i] = false;
				ok = true;
				_relStates[i] = false;
				if (PORT_TYPES.at(i) == WEAPON_TYPES::AK || PORT_TYPES.at(i) == WEAPON_TYPES::PM)
					newShotEnd(i, time);
				qDebug() << "Boom #" << i << " ended time = " << time;
				lastCId = KE::REL;
				sendToCam(tr(KE_REL_COMMAND) + QString::number(i + 1) + ",0");
			}

		}
		// проверка порта
		if (!ok && lastCId == KE::READ && contrlConnected && _port != Q_NULLPTR && _port->isOpen())
		{
			sendToCam(tr(KE_READ_COMMAND));
		}
	}
}

void ControllerHelper::run()
{
	init();
	if (_timer != Q_NULLPTR)
	{
		connect(_timer, &QTimer::timeout, this, &ControllerHelper::checkNewTargets);
		connect(_inTimer, &QTimer::timeout, this, &ControllerHelper::checkButtonValues);
		_timer->start(PORTS_UPD_TIMER);
		_inTimer->start(KE_UPD_TIMER);
	}
	//connect(_eTimer, &QTimer::timeout, this, &ControllerHelper::emulate);
#ifdef _EMULATE
	_eTimer->start(EMULATE_TIMER);
#endif
}

void ControllerHelper::makeBoom(quint64 id)
{
	if (id < _boomStates.size())
	{
		if (_boomStates.at(id))
		{
			// Подаем на вход сигнал о переключении клапана
			if (_boomStates.at(id))
			{
				quint64 time = QTime::currentTime().msecsSinceStartOfDay();
				_relStates[id] = true;
				_lastBooms[id] = time;
				if (PORT_TYPES.at(id) == WEAPON_TYPES::AK || PORT_TYPES.at(id) == WEAPON_TYPES::PM)
					emit newShotBegin(id, time);
				qDebug() << "Boom #" << id << " fired, time = " << time;
				lastCId = KE::REL;
				sendToCam(tr(KE_REL_COMMAND) + QString::number(id + 1) + ",1");
				//waitingForRele1.append(id);
				//boomTimer->setSingleShot(true);
				//boomTimer->start();
				//QThread::msleep(KE_BOOM_DELAY);
			}
		}
		else
		{
			//_buttonStates[id] = false;
			qDebug() << "Boom fired, but is not allowed: " << id;
		}
	}
}

void ControllerHelper::keInit()
{
	lastCId = KE::INIT;
	QString str;
	//for (quint64 i = 0; i < KE_IN_PORTS; i++)
	if (curWPort < KE_IN_PORTS)
	{
		qDebug() << "Init port #" << curWPort;
		str = tr(KE_INIT_SET_LINE_PREF) + QString::number(curWPort + 1) + tr(KE_INIT_SET_LINE_SUFF);
		sendToCam(str);
	}
	else
		qDebug() << "Port is out of range: " << curWPort;
}

void ControllerHelper::sendToCam(QString command)
{
	if (_port != Q_NULLPTR && _port->isOpen())
	{
		//qDebug() << "Send: " << command;
		//qDebug() << _port->isWritable();
		//command.append(KE_SUFFIX);
		command.append('\r');
		command.append('\n');
		_port->write(command.toUtf8());
		_port->flush();
		keInUse = true;
		//_port->waitForBytesWritten(0);
	}
	else
		qDebug() << "Eror while sending data = " << command << " Port is closed or NULL!";
}

void ControllerHelper::processShot(quint64 i, bool type)
{
	//readyForShot[i] = false;
	//shot_now[i] = true;
	//waitingForTimer.append(i);
	quint64 time = QTime::currentTime().msecsSinceStartOfDay();
	if (type)
	{
		_buttonStates[i] = true;
		if ((time - _lastShots.at(i)) >= SHOTS_DELAY.at(i))
		{
			_lastShots[i] = time;
			//if (PORT_TYPES.at(i) != WEAPON_TYPES::AK && PORT_TYPES.at(i) != WEAPON_TYPES::PM)
			//	emit newShotBegin(i, time);
			if (BOOM_AVALIABLE.at(i))
				makeBoom(i);
		}
	}
	else
	{
		_buttonStates[i] = false;
		if (PORT_TYPES.at(i) != WEAPON_TYPES::AK && PORT_TYPES.at(i) != WEAPON_TYPES::PM)
			emit newShotEnd(i, time);
	}
	//if (BURST_AVALIABLE.at(i))
	//{
	//	burstTimer->setSingleShot(true);
	//	waitingForBurstTimer.append(i);
	///	burstTimer->start(BURST_DELAY.at(i));
	//}
	//nextShotTimer->setSingleShot(true);
	//nextShotTimer->start(SHOTS_DELAY.at(i));
	//emit newShotBegin(i, QTime::currentTime().msecsSinceStartOfDay());
}

void ControllerHelper::readyRead()
{
	keInUse = false;
	bool ttt = false;
	quint64 time = QTime::currentTime().msecsSinceStartOfDay();
	//if (ok && _port != Q_NULLPTR)
	if (_port != Q_NULLPTR)
	{
		while (_port->canReadLine())
		{
			QString dat = _port->readLine();
			//qDebug() << "Read: " << dat;
			dat.remove(dat.size() - 2, 2);
			QStringList ls = dat.split(KE_DIVIDER);
			ls.removeAll("");
			//qDebug() << ls;
			/*
				Сначала проверяем модуль, ответ - #OK,
				потом включаем первые 6 линий на вход, ответ
				потом уже работа с модулем
				- чтение входов, ответ
				- управление реле, ответ
			*/
			bool ok = true;
			if (ls.size() > 0)
				// Parsing answers
				switch (lastCId)
				{
				case KE::TEST:
					if (ls.at(0).compare(KE_TEST_ANSWER) == 0)
					{
						curWPort = 0;
						keInit();
					}
					else
						qDebug() << "KE not recognized!";
					break;
				case KE::INIT:
					curWPort++;
					if (curWPort < KE_IN_PORTS)
					{
						keInit();
					}
					else
					{
						qDebug() << "End of init KE!";
						lastCId = KE::READ;
					}
					break;
				case KE::READ:
					checkInput(ls);
					break;
				case KE::REL:
					//if (waitingForRele2.size() > 0)
					//{
					//	waitingForRele2.removeFirst();
					//	lastCId = KE::READ;
					//}

					for (quint64 i = 0; i < _relStates.size(); i++)
						if (_relStates.at(i))
							ok = false;
					if (ok)
						lastCId = KE::READ;
					qDebug() << "Rele result: " << ls;
					break;
				default:
					qDebug() << "Unkown incoming data: " << dat << ", command Id = " << lastCId;
				}
		}
	}
}

void ControllerHelper::error(QSerialPort::SerialPortError error)
{
	qDebug() << error;
}

void ControllerHelper::controllerConnected(QSerialPortInfo* info)
{
	if (info != Q_NULLPTR)
	{
		_port = new QSerialPort(*info);
		//_port->setPort(*info);
		_port->setBaudRate(COM_CONTROLLER_BAUDRATE);
		_port->setFlowControl(QSerialPort::FlowControl::NoFlowControl);
		_port->setDataBits(QSerialPort::DataBits::Data8);
		bool ok = _port->open(QSerialPort::ReadWrite);
		qDebug() << "Opening controller port = " << ok << " (" << info->portName() << ")";
		if (ok)
		{
			connect(_port, &QSerialPort::readyRead, this, &ControllerHelper::readyRead);
			connect(_port, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error), this, &ControllerHelper::error);
			connect(_port, &QSerialPort::aboutToClose, this, &ControllerHelper::contrlDisconnected);
			connect(_port, &QSerialPort::bytesWritten, this, &ControllerHelper::bytesWritten);
			lastCId = KE::TEST;
			sendToCam(tr(KE_TEST_COMMAND));
			curWPort = 0;
		}
	}
	else
		qDebug() << "Serial port info was null while trying to init terminal controller!";
}

void ControllerHelper::contrlDisconnected()
{
	qDebug() << "KE disconnected: " << _port->portName();
	contrlConnected = false;
	_port->deleteLater();
	keInUse = true;
	lastCId = 0;
	lastCId = KE::TEST;
}

void ControllerHelper::targetDisconnected(QSerialPortInfo* inf)
{
	if (inf != Q_NULLPTR && _inf.size() > 0 && _inf.contains(inf->portName()))
	{
		_inf.removeAll(inf->portName());
	}
	else
		qDebug() << "Could not process target disconnected signal: wrong port info!s";
}

void ControllerHelper::changeBoomState(quint64 id, bool val)
{
	if (id < _boomStates.size())
	{
		_boomStates[id] = val;
	}
	else
		qDebug() << "Error while changin boom state: id > states.size(), id = " << id;
}

void ControllerHelper::bytesWritten(qint64 bytes)
{
	//qDebug() << "Some bytes written " << bytes;
	//_port->waitForReadyRead(0);
}

void ControllerHelper::targetCon(quint64 index)
{
	if (index < _tarStates.size())
	{
		_tarStates.replace(index, true);
	}
}

void ControllerHelper::targetNonCon(quint64 index)
{
	if (index < _tarStates.size())
	{
		_tarStates.replace(index, false);
	}
}

void ControllerHelper::checkInput(QStringList ls)
{
	if (ls.size() > 1)
	{
		//ls.removeAt(0); // removing "#RD"
		if (ls.at(1).size() == _lastInput.size() && ls.at(1).compare(_lastInput) != 0 && _tarStates.size() >= KE_IN_PORTS)
		{
			QString s = ls.at(1);
			for (quint64 i = 0; i < KE_IN_PORTS; i++) // порты в KE нумеруются с 1
			{
				bool ok = false;
				if (_tarStates.at(i) && s.at(i) != _lastInput.at(i))
				{
					ok = true;
					//qDebug() << "Found changes at port: " << i << ", value  = " << s.at(i);
					if (s.at(i) == KE_READ_HIGH)
					{
						processShot(i, true);
					}
					else if (s.at(i) == KE_READ_LOW)
					{
						_buttonStates[i] = false;
						processShot(i, false);
						//readyForShot[i] = false;
						//burstTimer->stop();
						//shot_now[i] = false;
					}
				}
				if (ok)
				{
					//qDebug() << "Current = " << ls.at(1) << ", last = " << _lastInput;
				}
			}
			_lastInput = s;
		}
	}
	else
		qDebug() << "Reading input answer length mismath!";
}
