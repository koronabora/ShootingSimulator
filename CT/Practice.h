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
	QVector<shot*> shots; // ���������� �� ����������� ���������
	//QVector<shotPoint*> buffer; // ����� ��������� ��������� �� ����������� ���������� �������
	void calcResults();
	bool connected;
	bool firstTime;
	bool inShot;
	
	qreal lastX;
	qreal lastY;
	quint64 lastTime; // ����� ���������� ��������. ��� ������� �������� - ��� ����� ������ ����������
	quint64 lastUpdTime; // ����� ��������� ��������� ������� � ������
	quint64 practiceShotCount;
	quint64 currentShotCount;
	QVector<qreal> mults;
	QVector<qreal> bufX;
	QVector<qreal> bufY;
	QVector<quint64> bufId;

	//QTimer* inTimer; // ������ ��������� �������� � ���� ������ � ���������� �� ����� ��������
	QTimer* afterTimer; // ������ ��������� �������� ��������� � ���� � ���������� ����� ��������
	//QTimer* shotRepTimer; // ������ �������.
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

	// � GUI
	void paintDot(quint64 index, quint64 id, qreal x, qreal y, qreal x2, qreal y2, quint64 dotId); // ������ ����� �������
	void nextShot(quint64 index, guiShotData* pRes); // ����� ����� �������� ������������ ��������� �������
	void practiceEnded(quint64 index, guiShotData* pRes); // ��������� ���������� ����������
	void newPractice(quint64 index, quint64 shotCount); //  �������� ��������� ����������
	void targetConnected(quint64 index); //  ������ ����������
	void targetDisconnected(quint64 index); //  ������ �� �����������
	void removeLine(quint64 index, quint64 dotId); //
	void logTo(quint64 index, QString mes); //
	void repaintDot(quint64 index, quint64 id, quint64 dotId);
	void moveDot(quint64 index, quint64 id, qreal newx, qreal newy, qreal newx2, qreal newy2, quint64 dotId);
	void playRifle(quint64 id);
	void playResults(quint64 id, QString res, QString hour);

	// � �������� �� �����������, ����������� ������
	void changeBoomState(quint64 id, bool val); // �� ����

public slots:
	void newPosition(quint64 targetId, qreal x, qreal y, quint64 time); // �� ���������� ����������� ������
	void newShotBegin(quint64 targetId, quint64 time); // �� ���������� ����������� ������
	//void newShot(quint64 time);
	void newShotEnd(quint64 targetId, quint64 time); // �� ���������� ����������� ������
	void targetOffline(quint64 targetId); // �� ���������� ����������� ������
	void targetOnline(quint64 targetId); // �� ���������� ����������� ������
	void run(); // ������ �� ������
	void setMult(QVector<qreal> mult);
	void newPracticeSelected(quint64 index, quint64 shotCount); // � GUI. ����� �������� ���������� ��������� � ����� ����������

	// �������
	//void timer1();
	void timer2();
	//void timer3();
};