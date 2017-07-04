#pragma once

#include <QObject>
#include <QVector>
#include <QDebug>
#include <QVector>
#include <QDateTime>
#include <QTimer>
#include "Defines.h"
//#include <QMediaPlayer>
//#include <QMediaPlaylist>
//#include <QUrl>
//#include <QSound>

class Practice : public QObject
{
	Q_OBJECT
public:
	Practice();
	~Practice();

	void init(quint64 id);
private:
	quint64 id;
	QVector<shot*> shots; // информация по завершенным выстрелам
	//QVector<shotPoint*> buffer; // буфер последних координат за опереденное количество времени
	void calcResults();
	bool connected;
	bool firstTime;
	bool inShot;
	
	qreal lastX;
	qreal lastY;
	quint64 lastTime; // Время последнего выстрела. Для первого выстрела - это время начала упражнения
	quint64 lastUpdTime; // Время последней обработки сигнала с мишени
	quint64 practiceShotCount;
	quint64 currentShotCount;
	QVector<qreal> mults;
	QVector<qreal> bufX;
	QVector<qreal> bufY;
	QVector<quint64> bufId;

	//QTimer* inTimer; // таймер окончания внесения в базу данных о траектории во время выстрела
	QTimer* afterTimer; // таймер окончания внесения изменений в базу о траектории после выстрела
	//QTimer* shotRepTimer; // таймер очереди.
	quint8 state = STATES::NC;
	shot* currentShot;
	void rotateShots();
	quint64 itemId;
	QVector<qreal> shotX;
	QVector<qreal> shotY;
	qreal Practice::transX(qreal x, quint64 index);
	qreal Practice::transY(qreal y, quint64 index);
	qreal posToGrad(qreal x, qreal y);

	//QMediaPlayer* p;
	//QMediaPlaylist* pl;

	//QStringList filesToPlay;
	void fillRes(guiShotData* res);
signals:
	//void plotPoint(quint64 id, qreal x, qreal y, quint64 type);
	void writeDotToPoint(quint64 id, qreal x, qreal y, qreal x2, qreal y2);
	void workFinished();

	// В GUI
	void paintDot(quint64 index, quint64 id, qreal x, qreal y, qreal x2, qreal y2, quint64 dotId); // рисуем точку прицела
	void nextShot(quint64 index, guiShotData* pRes); // После этого начинаем обрабатывать следующий выстрел
	void practiceEnded(quint64 index, guiShotData* pRes); // Завершаем выполнение упражнения
	void newPractice(quint64 index, quint64 shotCount); //  Начинаем выполнять упражнение
	void targetConnected(quint64 index); //  мишень подключена
	void targetDisconnected(quint64 index); //  мишень не подоключена
	void removeLine(quint64 index, quint64 dotId); //
	void logTo(quint64 index, QString mes); //
	void repaintDot(quint64 index, quint64 id, quint64 dotId);
	void moveDot(quint64 index, quint64 id, qreal newx, qreal newy, qreal newx2, qreal newy2, quint64 dotId);
	void playRifle(quint64 id);
	void playResults(quint64 id, QString res, QString hour);

	// в помошник по контроллеру, регулировка отадчи
	void changeBoomState(quint64 id, bool val); // от ядра

public slots:
	void newPosition(quint64 targetId, qreal x, qreal y, quint64 time); // Из экземпляра обработчика мишени
	void newShotBegin(quint64 targetId, quint64 time); // Из экземпляра обработчика мишени
	//void newShot(quint64 time);
	void newShotEnd(quint64 targetId, quint64 time); // Из экземпляра обработчика мишени
	void targetOffline(quint64 targetId); // Из экземпляра обработчика мишени
	void targetOnline(quint64 targetId); // Из экземпляра обработчика мишени
	void run(); // Запуск из потока
	void setMult(QVector<qreal> mult);
	void newPracticeSelected(quint64 index, quint64 shotCount); // В GUI. Когда выбирают количество выстрелов в новом упражнении

	// таймеры
	//void timer1();
	void timer2();
	//void timer3();
};