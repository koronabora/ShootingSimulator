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
	void fillValues(quint64 index, guiShotData* pRes); // Заносим данные о предыдущем выстреле в интерфейс
	void redrawPlots(quint64 index);
	//void drawTarget(quint64 index);
	void clearGraphData();

	// Вспомогательные переменные
	QVector<quint64> shotsCountInPractice; // для каждой мишени храним количество выстрелов, выбранное для данного упражнения для каждой 
	QVector<bool> connectedTargets; // Массив флагов подключения мишеней
	QVector<bool> practiceStarted; // Массив флагов начала упражнения
	QVector<bool> sceneUpdating; // Массив флагов начала упражнения
	//QVector<QWidget*> targets; // все виджеты мишеней + обвязкя
	QStackedWidget* targets; // все виджеты мишеней. mainForm в ct.ui, место для переключения окон
	QVector<QVector<QGraphicsScene*>> scenes; // сцены для отрисовки мишеней. Каждая мишень содержит набор сцен с данными каждого выстрела
	QVector<quint64> sceneSizes; // номер последней используемой сцены
	QVector<quint64> sceneIndexes; // номер активной сцены прицела для каждого виджета мишени
	QVector<QVector<QCustomPlot*>> _graphs; // графики
	QVector<QVector<QVector<qreal>>> xx; // значения по оси х для графиков
	QVector<QVector<QVector<qreal>>> yy; // значения по оси y для графиков
	QVector<qreal> zoom;
	QVector<qreal> mults;
	QUiLoader _loader; // загрузчик ui фалов
	QVector<QVector<QGraphicsLineItem*>> lines;
	QVector<QVector<quint64>*> lids;
	QStringList sounds;

	// Диалоговое окно выбора количества выстрелов в упражнении
	QMainWindow* _dlg; // Экземпляр диалога выбора количества выстрелов для упражнения
	QSpinBox* _dlgSpinBox; // Спинбокс диалога
	quint64 _dlgVal = 5; // значение диалога
	QPushButton* _dlgPushButton; // кнопка диалога
	
	// Кнопки переключения мишеней
	quint64 currentIndex = 0;

	QPushButton* b0;
	QPushButton* b1;
	QPushButton* b2;
	QPushButton* b3;
	QPushButton* b4;
	QPushButton* b5;

	// Группы всех контролов внутри мишени
	
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

	// Менеджер отчетов
	QMainWindow* reportWizard;
	QPushButton* toRigthButton;
	QPushButton* toLeftButton;
	QPushButton* formReport;
	QPushButton* pb2;
	QListWidget* srcTree;
	QListWidget* destTree;
	void completeSrcTree(); // Заполняем исходное дерево отчетов. Вызывается 1 раз при запуске программы. В процессе работы все отчеты добавляются туда после создания и нет нужды снова читать файлы
	bool ls = true;
	void scanFiles(QString fName);
	quint64 sl = 0;
	quint64 sr = 0;

	// графики
	//QCustomPlot* pl;
	QTimer* plotTimer;
	QMainWindow *w;

	// Менеджер корректировки
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

	//Меню
	QDialog* menuWid;
	QPushButton* mbRep;
	QPushButton* mbCorr;
	QPushButton* mbExit;
	QPushButton* mbShutdown;


	// Менеджер архива
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
	void paintDot(quint64 index, quint64 id, qreal x, qreal y, qreal x2, qreal y2, quint64 dotId); // из класса упражнение
	void nextShot(quint64 index, guiShotData* pRes); // из класса упражнение
	void practiceEnded(quint64 index, guiShotData* pRes); // из класса упражнение
	void newPractice(quint64 index, quint64 shotCount); //  из класса упражнение

	//qreal transX(qreal x, quint64 index);
	//qreal transY(qreal y, quint64 index);

	void targetConnected(quint64 index); //  из класса упражнение
	void targetDisconnected(quint64 index); //  из класса упражнение

	void plotGraph0(quint64 id, qreal x);
	void plotGraph1(quint64 id, qreal x);
	void plotGraph2(quint64 id, qreal x);
	void plotGraph3(quint64 id, qreal x);
	void replotGraphs();

	// Обработчики gui

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

	// из окна выбора 
	void dlgOkClicked();
	void dlgSpinValCHanged(int val);

	// Из менеджера отчетов
	void showInExplorerPressed();
	void toRigthButtonPressed();
	void toLeftButtonPressed();
	void formReportPressed();
	void leftSelected(int row);
	void rigthSelected(int row);

	// Из окна корректировки
	void okBtn();
	void canBtn();
	void updVals(double val);
	void updSpin(int qreal);
	void updMainSpin(int val);
	void updMulti(double val);

	// Из окна меню
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
