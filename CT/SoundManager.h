#pragma once

#include <QObject.h>
#include <QThread>
#include <QSound>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QStringList>
#include <QTimer>
#include "Defines.h"

class SoundManager : public QObject
{
	Q_OBJECT
public:
	SoundManager();
	~SoundManager();

private:
	//.QSound* s;
	QStringList* sounds;
	//QMediaPlayer* player;
	//QMediaPlaylist* playlist;
	QTimer* soundTimer;
protected:
	//bool event(QEvent *event);
public slots:
	void run();
	void playRifle2(quint64 id);
	void playResults2(quint64 id,  QString res, QString hour);
	void timerHand();
signals:
	void workFinished();
};