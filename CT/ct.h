#ifndef CT_H
#define CT_H

#include <QtWidgets/QMainWindow>
#include <QGridLayout>
#include <QObject>
#include <QTimer>
#include <QtUiTools>
#include <QDialog>
#include <QSound>
#include <QDir>
#include <QPixmap>
#include <QPainter>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPrinter>
#include <QDirIterator>
#include <QPrintDialog>
#include "ui_ct.h"
#include "Defines.h"
#include "qcustomplot.h"

class CT : public QMainWindow
{
	Q_OBJECT

public:
	CT(QWidget *parent = 0);
	~CT();
	//QList<TargetWidget*> _targets;
private:

	QList<QList<qreal>> CORR =	{
		{ 1.0, 1.0, 1.0, 1.0, 3.0 },
		{ 1.0, 1.0, 1.0, 1.0, 3.0 },
		{ 1.0, 1.0, 1.0, 1.0, 3.0 },
		{ 1.0, 1.0, 1.0, 1.0, 3.0 },
		{ 1.0, 1.0, 1.0, 1.0, 3.0 },
		{ 1.0, 1.0, 1.0, 1.0, 3.0 }
	};

	Ui::CTClass ui;
	QGridLayout* mainLayout;

	void showDialog();
	void switchScene();
	void initTargetView(QGraphicsView* view, QGraphicsScene* scene);
	void initScenes(QVector<QGraphicsScene*> scenes);
	void fillValues(quint64 index, guiShotData* pRes); // ������� ������ � ���������� �������� � ���������
	void redrawPlots(quint64 index);
	//void drawTarget(quint64 index);
	void clearGraphData();

	// ��������������� ����������
	QVector<quint64> shotsCountInPractice; // ��� ������ ������ ������ ���������� ���������, ��������� ��� ������� ���������� ��� ������ 
	QVector<bool> connectedTargets; // ������ ������ ����������� �������
	QVector<bool> practiceStarted; // ������ ������ ������ ����������
	QVector<bool> sceneUpdating; // ������ ������ ������ ����������
	//QVector<QWidget*> targets; // ��� ������� ������� + �������
	QStackedWidget* targets; // ��� ������� �������. mainForm � ct.ui, ����� ��� ������������ ����
	QVector<QVector<QGraphicsScene*>> scenes; // ����� ��� ��������� �������. ������ ������ �������� ����� ���� � ������� ������� ��������
	QVector<quint64> sceneSizes; // ����� ��������� ������������ �����
	QVector<quint64> sceneIndexes; // ����� �������� ����� ������� ��� ������� ������� ������
	QVector<QVector<QCustomPlot*>> _graphs; // �������
	QVector<QVector<QVector<qreal>>> xx; // �������� �� ��� � ��� ��������
	QVector<QVector<QVector<qreal>>> yy; // �������� �� ��� y ��� ��������
	QVector<qreal> zoom;
	QVector<qreal> mults;
	QUiLoader _loader; // ��������� ui �����
	QVector<QVector<QGraphicsLineItem*>> lines;
	QVector<QVector<quint64>*> lids;
	QStringList sounds;

	// ���������� ���� ������ ���������� ��������� � ����������
	QMainWindow* _dlg; // ��������� ������� ������ ���������� ��������� ��� ����������
	QSpinBox* _dlgSpinBox; // �������� �������
	quint64 _dlgVal = 5; // �������� �������
	QPushButton* _dlgPushButton; // ������ �������
	
	// ������ ������������ �������
	quint64 currentIndex = 0;

	QPushButton* b0;
	QPushButton* b1;
	QPushButton* b2;
	QPushButton* b3;
	QPushButton* b4;
	QPushButton* b5;

	// ������ ���� ��������� ������ ������
	
	//QStackWidget* mainW; // 

	QVector<QLabel*> caption; // mainLabel
	QVector<QLineEdit*> fioField; // fioEdit
	QVector<QLineEdit*> zvanie; // zvanEdit
	QVector<QLineEdit*> addInfo; // otherEdit
	QVector<QGraphicsView*> tarViews; //targetView
	QVector<QPushButton*> prevShotButton; //prevButton
	QVector<QPushButton*> zoomLessButton; //lessButton
	QVector<QPushButton*> zoomMoreButton; //moreButton
	QVector<QPushButton*> nextShotButton; //nextButton
	QVector<QLineEdit*> averageResult; //averageMark
	QVector<QLineEdit*> averageMark; //resultsField
	QVector<QLineEdit*> recommendations; // recommends
	QVector<QPlainTextEdit*> currentShotInfo; //oneShotInfo
	QVector<QPlainTextEdit*> allShotsInfo; //shotSeriesInfo
	QVector<QPushButton*> reportManagerButton; //resultsButton
	QVector<QPushButton*> newPracticeButton; //newPractice
	QVector<QWidget*> resultsGraphFrame; //resGraph
	QVector<QWidget*> shotTimeGraphFrame; //shotTimeGraph
	QVector<QWidget*> shotKrenGraphFrame; //krenGraph
	QVector<QWidget*> shotTraectoryFrame; //traectoryGraph

	// �������� �������
	QMainWindow* reportWizard;
	QPushButton* toRigthButton;
	QPushButton* toLeftButton;
	QPushButton* formReport;
	QPushButton* pb2;
	QListWidget* srcTree;
	QListWidget* destTree;
	void completeSrcTree(); // ��������� �������� ������ �������. ���������� 1 ��� ��� ������� ���������. � �������� ������ ��� ������ ����������� ���� ����� �������� � ��� ����� ����� ������ �����
	bool ls = true;
	void scanFiles(QString fName);
	quint64 sl = 0;
	quint64 sr = 0;

	// �������
	//QCustomPlot* pl;
	QTimer* plotTimer;
	QMainWindow *w;

	// �������� �������������
	QMainWindow* cor;
	QVector<QVector<QDoubleSpinBox*>> akA;
	QSpinBox* smaSpin;
	QSpinBox* sma2Spin;
	QPushButton* okCorBtn;
	QPushButton* canCorBtn;
	QDoubleSpinBox* d0;
	QDoubleSpinBox* d1;
	QDoubleSpinBox* d2;
	QDoubleSpinBox* d3;
	QDoubleSpinBox* d4;
	QDoubleSpinBox* d5;
	bool loaded = false;
	bool inWork = false;
	bool firstTime = false;
	bool firstRun = true;

	//����
	QDialog* menuWid;
	QPushButton* mbRep;
	QPushButton* mbCorr;
	QPushButton* mbExit;
	QPushButton* mbShutdown;


	// �������� ������
	void saveAllImages(quint64 index, guiShotData* dat);
	QString checkDirs(quint64 index, QDateTime dt);
protected:
	bool event(QEvent *event);

signals:
	void newPracticeSelected(quint64 index, quint64 shotCount);
	void saveCorrection(QList<QList<qreal>> v);
	void setCorrection(QList<QList<qreal>> v);
	void setSmaPer(quint64 sma);
	void setUserSmaPer(quint64 sma);
	void setMulti(qreal val, quint64 _id);
	void playRifle2(quint64 id);
	void playResults2(quint64 id, QString res, QString hour);
	void setMult(QVector<qreal> mult);
public slots:
	void loadCorrection(QList<QList<qreal>> v);
	void removeLine(quint64 index, quint64 dotId);
	void logTo(quint64 index, QString mes);
	void repaintDot(quint64 index, quint64 id, quint64 dotId);
	void moveDot(quint64 index, quint64 id, qreal newx, qreal newy, qreal newx2, qreal newy2, quint64 dotId);
	void paintDot(quint64 index, quint64 id, qreal x, qreal y, qreal x2, qreal y2, quint64 dotId); // �� ������ ����������
	void nextShot(quint64 index, guiShotData* pRes); // �� ������ ����������
	void practiceEnded(quint64 index, guiShotData* pRes); // �� ������ ����������
	void newPractice(quint64 index, quint64 shotCount); //  �� ������ ����������

	//qreal transX(qreal x, quint64 index);
	//qreal transY(qreal y, quint64 index);

	void targetConnected(quint64 index); //  �� ������ ����������
	void targetDisconnected(quint64 index); //  �� ������ ����������

	void plotGraph0(quint64 id, qreal x);
	void plotGraph1(quint64 id, qreal x);
	void plotGraph2(quint64 id, qreal x);
	void plotGraph3(quint64 id, qreal x);
	void replotGraphs();

	// ����������� gui

	void button0Pressed(); 
	void button1Pressed();
	void button2Pressed();
	void button3Pressed();
	void button4Pressed();
	void button5Pressed();

	void prevShotButtonPressed(); //prevButton
	void zoomLessButtonPressed(); //lessButton
	void zoomMoreButtonPressed(); //moreButton
	void nextShotButtonPressed(); //nextButton
	void reportManagerButtonPressed(); //resultsButton
	void newPracticeButtonPressed(); //newPractice

	void menuShowPressed();

	// �� ���� ������ 
	void dlgOkClicked();
	void dlgSpinValCHanged(int val);

	// �� ��������� �������
	void showInExplorerPressed();
	void toRigthButtonPressed();
	void toLeftButtonPressed();
	void formReportPressed();
	void leftSelected(int row);
	void rigthSelected(int row);

	// �� ���� �������������
	void okBtn();
	void canBtn();
	void updVals(double val);
	void updSpin(int qreal);
	void updMainSpin(int val);
	void updMulti(double val);

	// �� ���� ����
	//void runExplorer();
	void exitAndShutdown();
	void mbReport();
	void showCorr();
	void exitButton();
	//QPushButton* mbExit;

	void playRifle(quint64 id);
	void playResults(quint64 id, QString res, QString hour);
};

#endif // CT_H
