#include "ControllerHelper.h"

ControllerHelper::ControllerHelper()
{}

ControllerHelper::~ControllerHelper()
{
	emit workFinished();
}

void ControllerHelper::init()
{
	_timer = new QTimer();
	_inTimer = new QTimer();
	//_eVal = 0;
	//qsrand(QTime::currentTime().msecsSinceStartOfDay());
}

void ControllerHelper::checkNewTargets()
{
	QSerialPortInfo inf;
	QList<QSerialPortInfo> ports = inf.availablePorts();
	if (_inf.size() < ports.size())
	{
		_inf = ports;
		for (quint64 i = 0; i < ports.size(); i++)
		{
			QSerialPortInfo* info = new QSerialPortInfo(ports.at(i));
			if (ports.at(i).description().compare(COM_TARGET_NAME) == 0)
			{
				//QSerialPortInfo* info = new QSerialPortInfo(ports.at(i));
				emit targetConnected(info);
			}
			else if ((!contrlConnected) && ports.at(i).description().compare(COM_CONTROLLER_NAME) == 0)
			{
				contrlConnected = true;
				qDebug() << "Controller found: " << COM_CONTROLLER_NAME;
				controllerConnected(info);
			}
		}
	}
	// Проверяем состояние входа на контроллере, также, если надо, организуем сразу отдачу

}

void ControllerHelper::checkButtonValues()
{
	if (contrlConnected && _port != Q_NULLPTR && _port->isOpen())
	{
		lastCId = 
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
				sendToCam(tr(KE_REL_COMMAND) + QString::number(id));
				lastCId = KE::REL;
			}
		}
	}
}

void ControllerHelper::keInit()
{
	lastCId = KE::INIT;
	QString str;
	for (quint64 i = 0; i < KE_IN_PORTS; i++)
	{
		str = tr(KE_INIT_SET_LINE_PREF) + QString::number(i) + tr(KE_INIT_SET_LINE_SUFF);
		sendToCam(tr(KE_TEST_COMMAND));
	}
}

void ControllerHelper::sendToCam(QString command)
{
	if (_port != Q_NULLPTR && _port->isOpen())
	{
		command.append(KE_SUFFIX);
		_port->write(command.toUtf8());
		_port->flush();
		keInUse = true;
	}
	else
		qDebug() << "Eror while sending data = " << command << " Port is closed or NULL!";
}

/*void ControllerHelper::changeBoomState(bool state)
{
	_boomState = state;
}

void ControllerHelper::inputValuesFromTarget(quint64 id, quint64 a, quint64 b, quint64 c, quint64 d, quint64 time)
{
	emit newValues(id, a, b, c, d, time);
}

void ControllerHelper::emulate()
{
	quint64 t = QTime::currentTime().msecsSinceStartOfDay();
	_eVal = sin((t % EMULATE_COUNT) / (EMULATE_COUNT*1.0)*2*_PI);
	qsrand(time(NULL));
	quint64 v1 = _eVal * 512 + sin((qrand()%100)/100*2*_PI)*20;
	qsrand(time(NULL));
	quint64 v2 = _eVal * 512 + sin((qrand() % 100) / 100 * 2 * _PI) * 20;
	qsrand(time(NULL));
	quint64 v3 = _eVal * 512 + sin((qrand() % 100) / 100 * 2 * _PI) * 20;
	qsrand(time(NULL));
	quint64 v4 = _eVal * 512 + sin((qrand() % 100) / 100 * 2 * _PI) * 20;
	emit newValues(0, v1, v2, v3, v4, t);
}*/

void ControllerHelper::readyRead()
{
	keInUse = false;
	quint64 time = QTime::currentTime().msecsSinceStartOfDay();
	//if (ok && _port != Q_NULLPTR)
	if (_port != Q_NULLPTR)
	{
		while (_port->canReadLine())
		{
			QString dat = _port->readLine();
			//qDebug() << dat;
			QStringList ls = dat.split(KE_DIVIDER);
			ls.removeAll("");
			/*
				Сначала проверяем модуль, ответ - #OK,
				потом включаем первые 6 линий на вход, ответ
				потом уже работа с модулем
				- чтение входов, ответ
				- управление реле, ответ
			*/
			if (ls.size() > 0)
				// Parsing answers
				switch (lastCId)
				{
				case 0:
					if (ls.at(0).compare(KE_TEST_ANSWER)==0)
					break;
				case 1:
					break;
				case 2:
					break;
				case 3:
					break;
				case 4:
					break;
				case 5:
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
		connect(_port, &QSerialPort::readyRead, this, &ControllerHelper::readyRead);
		connect(_port, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error), this, &ControllerHelper::error);
		connect(_port, &QSerialPort::aboutToClose, &ControllerHelper::contrlDisconnected);

		_port->setBaudRate(COM_CONTROLLER_BAUDRATE);
		//_port->setFlowControl(QSerialPort::FlowControl::NoFlowControl);
		bool ok = _port->open(QSerialPort::ReadWrite);
		qDebug() << "Opening controller port = " << ok << " (" << info->portName << ")";
		if (ok)
		{
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
	contrlConnected = false;
	_port->deleteLater();
	keInUse = true;
}

void ControllerHelper::targetDisconnected()
{
	
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

