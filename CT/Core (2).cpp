#include "Core.h"

Core::Core()
{}

Core::~Core()
{
}

void Core::run()
{
	QThread* th = new QThread();
	_cHeler = new ControllerHelper();
	_cHeler->moveToThread(th);
	connect(th, &QThread::started, _cHeler, &ControllerHelper::run);
	connect(_cHeler, SIGNAL(workFinished()), th, SLOT(quit()));
	connect(th, SIGNAL(finished()), _cHeler, SLOT(deleteLater()));
	connect(th, SIGNAL(finished()), th, SLOT(deleteLater()));
	//connect(_cHeler, &ControllerHelper::newValues, this, &Target::newValues);
	th->start();
}

void Core::targetConnected(QSerialPortInfo inf)
{}

void Core::targetDisconnected(QSerialPortInfo inf)
{}