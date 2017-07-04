#include "SoundManager.h"

SoundManager::SoundManager()
{}

SoundManager::~SoundManager()
{
	//s->deleteLater();
	//player->deleteLater();
	//playlist->deleteLater();
	delete sounds;
	emit workFinished();
}

void SoundManager::run()
{
	soundTimer = new QTimer();
	connect(soundTimer, &QTimer::timeout, this, &SoundManager::timerHand);
	//s = new QSound("");
	//player = new QMediaPlayer(this);
	//playlist = new QMediaPlaylist(player);
	//player->setPlaylist(playlist);
	//player->setVolume(100);
	//player->setPlaybackRate(QMediaPlaylist::Sequential);
	sounds = new QStringList();
	soundTimer->start(SOUND_TIMER);
}

void SoundManager::playRifle2(quint64 id)
{
	//qDebug() << "Rifle sound: " << id;
	QSound::play(SHOT_SOUNDS.at(id));
	//sounds.append(SHOT_SOUNDS.at(id));
	//sounds.append(":/Sound/silence.wav");
}

void SoundManager::playResults2(quint64 id, QString res, QString hour)
{
	qDebug() << "Result sound: " << res;
	qDebug() << "Hour sound: " << hour;

	sounds->append(res);
	sounds->append(hour);
	//playlist->addMedia(QUrl::fromLocalFile(res));
	//playlist->addMedia(QUrl::fromLocalFile(hour));
	//player->play();
}

/*
bool SoundManager::event(QEvent *event)
{

	if ( sounds.size() > 0 && s->isFinished())
	{
			s->play(sounds.takeFirst());
	}
	
	return QObject::event(event);
}*/

void SoundManager::timerHand()
{
	if (sounds->size() > 0)
	{
		QSound::play(sounds->takeFirst());
	}
}