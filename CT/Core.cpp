#include "Core.h"

Core::Core()
{}


void Core::init(CT* ui)
{
	gui = ui;
	if (ui == Q_NULLPTR)
		qDebug() << "Received ui class is null!";
}

Core::~Core()
{
	qDeleteAll(_targets);
	_targets.clear();

	qDeleteAll(_tInfos);
	_tInfos.clear();
	//QList<QThread*> _tThreads;

	if (_cHelper != Q_NULLPTR)
		_cHelper->deleteLater();

	if (_sm != Q_NULLPTR)
		_sm->deleteLater();

	if (_ph != Q_NULLPTR)
		_ph->deleteLater();
	
	emit workFinished();
}

void Core::run()
{
	settings = new QSettings(DEF_COFIG, QSettings::Format::IniFormat);
	_chThread = new QThread();
	_cHelper = new ControllerHelper();
	_cHelper->moveToThread(_chThread);
	connect(_chThread, &QThread::started, _cHelper, &ControllerHelper::run);
	connect(_cHelper, SIGNAL(workFinished()), _chThread, SLOT(quit()));
	connect(_chThread, SIGNAL(finished()), _cHelper, SLOT(deleteLater()));
	connect(_chThread, SIGNAL(finished()), _chThread, SLOT(deleteLater()));
	_chThread->start();

	_smThread = new QThread();
	_sm = new SoundManager();
	_sm->moveToThread(_smThread);
	connect(_smThread, &QThread::started, _sm, &SoundManager::run);
	connect(_sm, SIGNAL(workFinished()), _smThread, SLOT(quit()));
	connect(_smThread, SIGNAL(finished()), _sm, SLOT(deleteLater()));
	connect(_smThread, SIGNAL(finished()), _smThread, SLOT(deleteLater()));
	_smThread->start();

	_phThread = new QThread();
	_ph = new PrintHelper();
	_ph->moveToThread(_phThread);
	connect(_phThread, &QThread::started, _ph, &PrintHelper::run);
	connect(_ph, SIGNAL(workFinished()), _phThread, SLOT(quit()));
	connect(_phThread, SIGNAL(finished()), _ph, SLOT(deleteLater()));
	connect(_phThread, SIGNAL(finished()), _phThread, SLOT(deleteLater()));
	_phThread->start();

	// смысловые галлюцинации
	connect(this, &Core::targetDisconnected, _cHelper, &ControllerHelper::targetDisconnected);
	connect(_cHelper, &ControllerHelper::targetConnected, this, &Core::targetConnected);
	connect(_cHelper, &ControllerHelper::newShotBegin, this, &Core::newShotBegin);
	connect(_cHelper, &ControllerHelper::newShotEnd, this, &Core::newShotEnd);
	connect(gui, &CT::saveCorrection, this, &Core::saveCorrection);
	connect(this, &Core::loadCorrection, gui, &CT::loadCorrection);

	loadSettings();

	connect(gui, &CT::playRifle2, _sm, &SoundManager::playRifle2);
	connect(gui, &CT::playResults2, _sm, &SoundManager::playResults2);

	// Создаем задания
	for (quint64 i = 0; i < KE_IN_PORTS; i++)
	{
		QThread* t = new QThread();
		Practice* p = new Practice();
		p->moveToThread(t);
		connect(t, &QThread::started, p, &Practice::run);
		connect(p, SIGNAL(workFinished()), t, SLOT(quit()));
		connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
		connect(t, SIGNAL(finished()), p, SLOT(deleteLater()));

		// Смысловое подключение сигналов слотов для обработчика упражнений
		connect(this, &Core::newPosition, p, &Practice::newPosition);
		connect(this, &Core::newShotBegin, p, &Practice::newShotBegin);
		connect(this, &Core::newShotEnd, p, &Practice::newShotEnd);
		connect(this, &Core::targetOffline, p, &Practice::targetOffline);
		connect(this, &Core::targetOnline, p, &Practice::targetOnline);
		connect(gui, &CT::setMult, p, &Practice::setMult);
		connect(gui, &CT::newPracticeSelected, p, &Practice::newPracticeSelected);
		connect(p, &Practice::paintDot, gui, &CT::paintDot);
		connect(p, &Practice::removeLine, gui, &CT::removeLine);
		connect(p, &Practice::repaintDot, gui, &CT::repaintDot);
		connect(p, &Practice::moveDot, gui, &CT::moveDot);
		connect(p, &Practice::nextShot, gui, &CT::nextShot);
		connect(p, &Practice::practiceEnded, gui, &CT::practiceEnded);
		connect(p, &Practice::newPractice, gui, &CT::newPractice);
		connect(p, &Practice::targetConnected, gui, &CT::targetConnected);
		connect(p, &Practice::targetDisconnected, gui, &CT::targetDisconnected);
		connect(p, &Practice::targetConnected, _cHelper, &ControllerHelper::targetCon);
		connect(p, &Practice::targetDisconnected, _cHelper, &ControllerHelper::targetNonCon);
		connect(p, &Practice::logTo, gui, &CT::logTo);
		connect(p, &Practice::playRifle, gui, &CT::playRifle);
		connect(p, &Practice::playResults, gui, &CT::playResults);
	
		connect(p, &Practice::changeBoomState, _cHelper, &ControllerHelper::changeBoomState);

		p->init(i);
		t->start();
	}
}

void Core::targetConnected(QSerialPortInfo* inf)
{
	if (inf != Q_NULLPTR)
	{
		// Создаем считыватель данных с com порта
		_tInfos.append(inf);
		Target* t = new Target();
		t->init(inf);
		_targets.append(t);
		QThread* th = new QThread();
		//th->setPriority(QThread::Priority);
		_tThreads.append(th);
		t->moveToThread(th);
		connect(th, &QThread::started, t, &Target::run);
		connect(t, SIGNAL(workFinished()), th, SLOT(quit()));
		connect(th, SIGNAL(finished()), t, SLOT(deleteLater()));
		connect(th, SIGNAL(finished()), th, SLOT(deleteLater()));
		th->start();

		connect(t, &Target::newPosition, this, &Core::newPosition);
		connect(t, &Target::disconnectedInf, this, &Core::targetDisconnected);
		connect(t, &Target::connected, this, &Core::targetOnline);
		connect(t, &Target::disconnected, this, &Core::targetOffline);

		connect(t, &Target::plotGraph0, gui, &CT::plotGraph0);
		connect(t, &Target::plotGraph1, gui, &CT::plotGraph1);
		connect(t, &Target::plotGraph2, gui, &CT::plotGraph2);
		connect(t, &Target::plotGraph3, gui, &CT::plotGraph3);
		connect(gui, &CT::setCorrection, t, &Target::setCorrection);
		connect(gui, &CT::setSmaPer, t, &Target::setSmaPer);
		connect(gui, &CT::setUserSmaPer, t, &Target::setUserSmaPer);
		connect(gui, &CT::setMulti, t, &Target::setMulti);
	}
	else
		qDebug() << "Core recieved null information about disconnected com port!";
}

void Core::targetDisconnectedFromCom(QSerialPortInfo* inf)
{
	if (inf != Q_NULLPTR)
	{
		if (_tInfos.size() > 0)
		{
			for (quint64 i = 0; i < _tInfos.size(); i++)
			{
				if (_tInfos.at(i)->portName().compare(inf->portName())==0)
				{
					_tInfos.removeAt(i);
					_targets.at(i)->deleteLater();
					_tThreads.removeAt(i);
					emit targetOffline(i);
					break;
				}
			}
		}
		else
			qDebug() << "Core could not process disconnecting signal.";
		emit targetDisconnected(inf);
	}
	else
		qDebug() << "Core recieved null information about disconnected com port!";
}

void Core::saveCorrection(QList<QList<qreal>> v)
{
	//QFile file(CORRECTION_FILE);
	for (quint64 i = 0; i < 6; i++)
		for (quint64 j = 0; j < 5; j++)
		{
			settings->setValue("Weapon" + QString::number(i + 1) + "/" + ('a' + j), v[i][j]);
		}
	settings->sync();
}

void Core::loadSettings()
{
	QList<QList<qreal>> v = {
		{ 1.0, 1.0, 1.0, 1.0, 3.0 },
		{ 1.0, 1.0, 1.0, 1.0, 3.0 },
		{ 1.0, 1.0, 1.0, 1.0, 3.0 },
		{ 1.0, 1.0, 1.0, 1.0, 3.0 },
		{ 1.0, 1.0, 1.0, 1.0, 3.0 },
		{ 1.0, 1.0, 1.0, 1.0, 3.0 }
	};
	settings->sync();
	for (quint64 i = 0; i<6; i++)
		for (quint64 j = 0; j < 5; j++)
		{
			v[i][j] = settings->value("Weapon" + QString::number(i + 1) + "/" + ('a' + j), v[i][j]).toDouble();
			//qDebug() << "Weapon" + QString::number(i + 1) + "/" + ('a' + j) << " : " << v[i][j];
			//settings->setValue("Weapon"+QString::number(i+1)+"/"+('a'+j),v[i][j]);
		}
	settings->sync();
	/*bool res = true;
	QFile file(CORRECTION_FILE);
	bool ok = file.open(QIODevice::ReadOnly | QIODevice::Text);
	if (ok)
																																																																																																																																																																																																																																																																{
		QString s = file.readAll();
		s.replace('\n', "");
		QStringList sl = s.split(' ');
		for (quint64 i = 0; i<6; i++)
			for (quint64 j = 0; j < 4; j++)
			{
				v[i][j] = sl.at(i*4+j).toDouble(&ok);
				if (!ok)
					return false;
			}
		file.close();
	}
	else
	{
		res = false;
		qDebug() << "Could not open file for writing corrections!";
	}*/

	emit loadCorrection(v);

	//return res;
}