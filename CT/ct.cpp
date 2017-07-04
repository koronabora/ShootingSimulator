#include "ct.h"

CT::CT(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setUpdatesEnabled(true);
	setBackgroundRole(QPalette::Base);
	setAutoFillBackground(true);

	targets = ui.mainForm;
	
	QVector<qreal> vx;
	QVector<qreal> vy;
	for (quint64 i = 0; i < KE_IN_PORTS; i++)
	{
		xx.append(QVector<QVector<qreal>>());
		yy.append(QVector<QVector<qreal>>());
		//sounds.append(QStringList());
		for (quint64 j = 0; j < CORE_GRAPH_COUNT_IN_TARGET; j++)
		{
			//vx = new QVector<qreal>();
			//vy = new QVector<qreal>();
#ifdef TEST_MODE
			//for (quint64 m = 0; m < CORE_DATA_SIZE; m++)
			//{
			//	vx->append(m);
			//	vy->append(0);
			//}
#else
			//vx->append(0);
			//vy->append(0);
#endif
			xx[i].append(QVector<qreal>(0));
			yy[i].append(QVector<qreal>(0));
		}
	}

#ifndef TEST_MODE
	clearGraphData();
#endif
	plotTimer = new QTimer();
	connect(plotTimer, &QTimer::timeout, this, &CT::replotGraphs);
#ifdef TEST_MODE
	plotTimer->start(REDRAW_PERIOD);
#endif

	// Загрузка диалогового окна
	QFile file(UI_COUNT_CHOOSER_FILE);
	bool ok = file.open(QIODevice::ReadOnly);
	qDebug() << "Opening shot count chooser form: " << ok;
	_dlg = (QMainWindow*)_loader.load(&file, this);
	file.close();
	if (_dlg != Q_NULLPTR)
	{
		_dlg->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
		//qDebug() << "Error while loading dialog! UI file is corrupted!";
		_dlgSpinBox = _dlg->findChild<QSpinBox*>("spinBox");
		if (_dlgSpinBox != Q_NULLPTR)
			connect(_dlgSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &CT::dlgSpinValCHanged);
		//connect(_dlgSpinBox, SIGNAL(valueChanged(INT)), this, SLOT(dlgSpinValCHanged(INT)));
		_dlgPushButton = _dlg->findChild<QPushButton*>("okButton");
		if (_dlgPushButton != Q_NULLPTR)
			connect(_dlgPushButton, &QPushButton::pressed, this, &CT::dlgOkClicked);
	}
	//_dlgVal = 0;

	// Инициализация 
	file.setFileName(UI_TARGET_FILE);
	ok = file.open(QIODevice::ReadOnly);
	qDebug() << "Opening target form: " << ok;
	QWidget* ff;
	for (quint64 i = 0; i < KE_IN_PORTS; i++)
	{
		//scenes.append(QVector<QGraphicsScene*>());
		//scenes.last().append(new QGraphicsScene());
		zoom.append(1.0);
		mults.append(3.0);
		sceneIndexes.append(0);
		ff = _loader.load(&file);
		file.reset();
		if (ff != Q_NULLPTR)
		{
			//ff->setVisible(false);
			connectedTargets.append(false);
			practiceStarted.append(false);
			sceneUpdating.append(false);
			targets->addWidget(ff);
		}
		else
			qDebug() << "Error while loading main target! UI file is corrupted!";
	}
	file.close();

	// Инициализация основных кнопок
	b0 = ui.b1;
	b1 = ui.b2;
	b2 = ui.b3;
	b3 = ui.b4;
	b4 = ui.b5;
	b5 = ui.b6;

	connect(b0, &QPushButton::clicked, this, &CT::button0Pressed);
	connect(b1, &QPushButton::clicked, this, &CT::button1Pressed);
	connect(b2, &QPushButton::clicked, this, &CT::button2Pressed);
	connect(b3, &QPushButton::clicked, this, &CT::button3Pressed);
	connect(b4, &QPushButton::clicked, this, &CT::button4Pressed);
	connect(b5, &QPushButton::clicked, this, &CT::button5Pressed);

	currentIndex = KE_IN_PORTS + 1;

	// Подгружаем и заполняем все данные
	//scenes.reserve(KE_IN_PORTS);
	QLabel* l;
	QGraphicsView* v;
	QPushButton* b;
	QLineEdit* le;
	QPlainTextEdit* pte;
	QCustomPlot* foo;
	QVector<QCustomPlot*> bar;
	QWidget* fr;

	for (quint64 i = 0; i < KE_IN_PORTS; i++)
	{
		if (targets != Q_NULLPTR && targets->widget(i) != 0)
		{
			bar.clear();
			//initScenes(scenes.at(i));
			//caption.append(targets->widget(i)->mainLabel);
			l = targets->widget(i)->findChild<QLabel*>("mainLabel");
			if (l != Q_NULLPTR)
				caption.append(l);
			else
				qDebug() << "Error while loading some ui data!";

			//fioField.append(targets->widget(i)->fioEdit);
			le = targets->widget(i)->findChild<QLineEdit*>("fioEdit");
			le->setText(QString::number(i));
			if (le != Q_NULLPTR)
				fioField.append(le);
			else
				qDebug() << "Error while loading some ui data!";

			//zvanie.append(targets->widget(i)->zvanEdit);
			le = targets->widget(i)->findChild<QLineEdit*>("zvanEdit");
			if (le != Q_NULLPTR)
				zvanie.append(le);
			else
				qDebug() << "Error while loading some ui data!";

			//addInfo.append(targets->widget(i)->otherEdit);
			le = targets->widget(i)->findChild<QLineEdit*>("otherEdit");
			if (le != Q_NULLPTR)
				addInfo.append(le);
			else
				qDebug() << "Error while loading some ui data!";

			//tarViews.append(targets->widget(i)->targetView); // target view
			v = targets->widget(i)->findChild<QGraphicsView*>("targetView");
			if (v != Q_NULLPTR)
			{
				v->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
				v->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
				tarViews.append(v);
				QGraphicsScene* s;
				scenes.append(QVector<QGraphicsScene*>());
				for (quint64 i = 0; i < CORE_MAX_SHOTS_COUNT; i++)
				{
					s = new QGraphicsScene();
					s->setSceneRect(0, 0, SCENE_RECT_X, SCENE_RECT_Y);
					v->setSceneRect(0, 0, SCENE_RECT_X, SCENE_RECT_Y);
					//v->scale(0.9, 0.9);
					v->setUpdatesEnabled(true);
					if (i == 0)
					{
						//v->setScene(s);
						//initTargetView(v, s);
					}
					scenes.last().append(s);
				}
				sceneSizes.append(0);
				lines.append(QVector<QGraphicsLineItem*>());
				lids.append(new QVector<quint64>());
			}
			else
				qDebug() << "Error while loading some ui data!";

			//prevShotButton.append(targets->widget(i)->prevButton); // button
			b = targets->widget(i)->findChild<QPushButton*>("prevButton");
			if (b != Q_NULLPTR)
			{
				prevShotButton.append(b);
				connect(b, &QPushButton::clicked, this, &CT::prevShotButtonPressed);
			}
			else
				qDebug() << "Error while loading some ui data!";

			//zoomLessButton.append(targets->widget(i)->lessButton); // button
			b = targets->widget(i)->findChild<QPushButton*>("lessButton");
			connect(b, &QPushButton::clicked, this, &CT::zoomLessButtonPressed);
			if (b != Q_NULLPTR)
			{
				zoomLessButton.append(b);
				connect(b, &QPushButton::clicked, this, &CT::zoomLessButtonPressed);
			}
			else
				qDebug() << "Error while loading some ui data!";

			//zoomMoreButton.append(targets->widget(i)->moreButton); // button
			b = targets->widget(i)->findChild<QPushButton*>("moreButton");
			if (b != Q_NULLPTR)
			{
				zoomMoreButton.append(b);
				connect(b, &QPushButton::clicked, this, &CT::zoomMoreButtonPressed);
			}
			else
				qDebug() << "Error while loading some ui data!";

			//nextShotButton.append(targets->widget(i)->nextButton); // button
			b = targets->widget(i)->findChild<QPushButton*>("nextButton");
			if (b != Q_NULLPTR)
			{
				nextShotButton.append(b);
				connect(b, &QPushButton::clicked, this, &CT::nextShotButtonPressed);
			}
			else
				qDebug() << "Error while loading some ui data!";

			//averageResult.append(targets->widget(i)->averageMark); // read only
			le = targets->widget(i)->findChild<QLineEdit*>("averageMark");
			if (le != Q_NULLPTR)
			{
				averageResult.append(le);
				le->setReadOnly(true);
			}
			else
				qDebug() << "Error while loading some ui data!";

			//averageMark.append(targets->widget(i)->resultsField); // read only
			le = targets->widget(i)->findChild<QLineEdit*>("resultsField");
			if (le != Q_NULLPTR)
			{
				averageMark.append(le);
				le->setReadOnly(true);
			}
			else
				qDebug() << "Error while loading some ui data!";

			//recommendations.append(targets->widget(i)->recommends); // read only
			le = targets->widget(i)->findChild<QLineEdit*>("recommends");
			if (le != Q_NULLPTR)
			{
				recommendations.append(le);
				le->setReadOnly(true);
			}
			else
				qDebug() << "Error while loading some ui data!";

			//currentShotInfo.append(targets->widget(i)->oneShotInfo); // read only
			pte = targets->widget(i)->findChild<QPlainTextEdit*>("oneShotInfo");
			if (pte != Q_NULLPTR)
			{
				currentShotInfo.append(pte);
				pte->setReadOnly(true);
			}
			else
				qDebug() << "Error while loading some ui data!";

			//allShotsInfo.append(targets->widget(i)->shotSeriesInfo); // read only
			pte = targets->widget(i)->findChild<QPlainTextEdit*>("shotSeriesInfo");
			if (pte != Q_NULLPTR)
			{
				allShotsInfo.append(pte);
				pte->setReadOnly(true);
			}
			else
				qDebug() << "Error while loading some ui data!";

			//reportManagerButton.append(targets->widget(i)->resultsButton); // button
			b = targets->widget(i)->findChild<QPushButton*>("resultsButton");
			if (b != Q_NULLPTR)
			{
				connect(b, &QPushButton::clicked, this, &CT::menuShowPressed);
				reportManagerButton.append(b);
			}
			else
				qDebug() << "Error while loading some ui data!";

			//newPracticeButton.append(targets->widget(i)->newPractice); // button
			b = targets->widget(i)->findChild<QPushButton*>("newPractice");
			if (b != Q_NULLPTR)
			{
				newPracticeButton.append(b);
				connect(b, &QPushButton::clicked, this, &CT::newPracticeButtonPressed);
			}
			else
				qDebug() << "Error while loading some ui data!";

			//resultsGraphFrame.append(targets->widget(i)->resGraph); // frame 
			fr = targets->widget(i)->findChild<QWidget*>("resGraph");
			if (fr != Q_NULLPTR)
			{
				resultsGraphFrame.append(fr);
				foo = new QCustomPlot(fr);
				foo->resize(GRAPH_X_SIZE, GRAPH_Y_SIZE);
				fr->setLayout(new QVBoxLayout());
				fr->setAutoFillBackground(true);
				foo->addGraph();
#ifdef TEST_MODE
				foo->xAxis->setRange(0, CORE_DATA_SIZE);//Для оси Ox
				foo->yAxis->setRange(0, COM_ADC_POWER);//Для оси Ox
				foo->graph(0)->setPen(QPen(Qt::blue));
				foo->graph(0)->setLineStyle(QCPGraph::LineStyle::lsStepCenter);
				foo->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::blue, Qt::white, GRAPGS_SIZE));
#else
				foo->yAxis->setRange(0, 1);
				foo->yAxis->setSubTicks(false);
				foo->xAxis->setRange(0, 1);
				foo->xAxis->setSubTicks(false);
				foo->graph(0)->setAntialiased(CORE_GRAPHS_ALIASED);
				foo->graph(0)->setPen(QPen(Qt::blue));
				foo->graph(0)->setLineStyle(QCPGraph::LineStyle::lsStepCenter);
				foo->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::magenta, Qt::magenta, GRAPGS_SIZE));
#endif
				bar.append(foo);
			}
			else
				qDebug() << "Error while loading some ui data!";

			//shotTimeGraphFrame.append(targets->widget(i)->shotTimeGraph); // frame
			fr = targets->widget(i)->findChild<QWidget*>("shotTimeGraph");
			if (fr != Q_NULLPTR)
			{
				shotTimeGraphFrame.append(fr);
				foo = new QCustomPlot(fr);
				fr->setLayout(new QVBoxLayout());
				foo->resize(GRAPH_X_SIZE, GRAPH_Y_SIZE);
				fr->setAutoFillBackground(true);
				foo->addGraph();
#ifdef TEST_MODE
				foo->xAxis->setRange(0, CORE_DATA_SIZE);//Для оси Ox
				foo->yAxis->setRange(0, COM_ADC_POWER);//Для оси Ox
				foo->graph(0)->setPen(QPen(Qt::red));
				foo->graph(0)->setLineStyle(QCPGraph::LineStyle::lsStepCenter);
				foo->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::red, Qt::white, GRAPGS_SIZE));
#else
				foo->yAxis->setRange(0, 1);
				foo->yAxis->setSubTicks(false);
				foo->xAxis->setRange(0, 1);
				foo->xAxis->setSubTicks(false);
				foo->graph(0)->setAntialiased(CORE_GRAPHS_ALIASED);
				foo->graph(0)->setPen(QPen(Qt::blue));
				foo->graph(0)->setLineStyle(QCPGraph::LineStyle::lsStepCenter);
				foo->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::magenta, Qt::magenta, GRAPGS_SIZE));
#endif
				bar.append(foo);
			}
			else
				qDebug() << "Error while loading some ui data!";

			//shotKrenGraphFrame.append(targets->widget(i)->krenGraph); // frame
			fr = targets->widget(i)->findChild<QWidget*>("krenGraph");
			if (fr != Q_NULLPTR)
			{
				shotKrenGraphFrame.append(fr);
				foo = new QCustomPlot(fr);
				fr->setLayout(new QVBoxLayout());
				foo->resize(GRAPH_X_SIZE, GRAPH_Y_SIZE);
				fr->setAutoFillBackground(true);
				foo->addGraph();
#ifdef TEST_MODE
				foo->xAxis->setRange(0, CORE_DATA_SIZE);//Для оси Ox
				foo->yAxis->setRange(0, COM_ADC_POWER);//Для оси Ox
				foo->graph(0)->setPen(QPen(Qt::darkGreen));
				foo->graph(0)->setLineStyle(QCPGraph::LineStyle::lsStepCenter);
				foo->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::darkGreen, Qt::white, GRAPGS_SIZE));
#else
				foo->yAxis->setRange(0, 1);
				foo->yAxis->setSubTicks(false);
				foo->xAxis->setRange(0, 1);
				foo->xAxis->setSubTicks(false);
				foo->graph(0)->setAntialiased(CORE_GRAPHS_ALIASED);
				foo->graph(0)->setPen(QPen(Qt::blue));
				foo->graph(0)->setLineStyle(QCPGraph::LineStyle::lsStepCenter);
				foo->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::magenta, Qt::magenta, GRAPGS_SIZE));
#endif
				bar.append(foo);
			}
			else
				qDebug() << "Error while loading some ui data!";

			//shotTraectoryFrame.append(targets->widget(i)->traectoryGraph); // frame
			fr = targets->widget(i)->findChild<QWidget*>("traectoryGraph");
			if (fr != Q_NULLPTR)
			{
				shotTraectoryFrame.append(fr);
				foo = new QCustomPlot(fr);
				fr->setLayout(new QVBoxLayout());
				foo->resize(GRAPH_X_SIZE, GRAPH_Y_SIZE);
				fr->setAutoFillBackground(true);
				foo->addGraph();
#ifdef TEST_MODE
				foo->xAxis->setRange(0, CORE_DATA_SIZE);//Для оси Ox
				foo->yAxis->setRange(0, COM_ADC_POWER);//Для оси Ox
				foo->graph(0)->setPen(QPen(Qt::magenta));
				foo->graph(0)->setLineStyle(QCPGraph::LineStyle::lsStepCenter);
				foo->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::magenta, Qt::white, GRAPGS_SIZE));
#else
				foo->yAxis->setRange(0, 1);
				foo->yAxis->setSubTicks(false);
				foo->xAxis->setRange(0, 1);
				foo->xAxis->setSubTicks(false);
				foo->graph(0)->setAntialiased(CORE_GRAPHS_ALIASED);
				foo->graph(0)->setPen(QPen(Qt::blue));
				foo->graph(0)->setLineStyle(QCPGraph::LineStyle::lsStepCenter);
				foo->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::magenta, Qt::magenta, GRAPGS_SIZE));
#endif
				bar.append(foo);
			}
			else
				qDebug() << "Error while loading some ui data!";
			_graphs.append(bar); // Добавялем сформированные графики в спец массив
		}
		else
			qDebug() << "Target widget with id = " << i << " is null!";
	}

	targets->addWidget(new QWidget(this));
	targets->setCurrentIndex(KE_IN_PORTS);

	// Загрузка менеджера отчетов
	file.setFileName(UI_REPORT_WIZARD_FILE);
	ok = file.open(QIODevice::ReadOnly | QIODevice::Text);
	qDebug() << "Opening report wizard form: " << ok;
	reportWizard = (QMainWindow*)_loader.load(&file, this);
	file.close();
	if (reportWizard != Q_NULLPTR)
	{
		toRigthButton = reportWizard->findChild<QPushButton*>("addToReport");
		if (toRigthButton != Q_NULLPTR)
		{
			connect(toRigthButton, &QPushButton::clicked, this, &CT::toRigthButtonPressed);
		}
		else qDebug() << "Could not load Report Wizard ui";
		toLeftButton = reportWizard->findChild<QPushButton*>("removeFromReport");
		if (toLeftButton != Q_NULLPTR)
		{
			connect(toLeftButton, &QPushButton::clicked, this, &CT::toLeftButtonPressed);
		}
		else qDebug() << "Could not load Report Wizard ui";
		formReport = reportWizard->findChild<QPushButton*>("printReport");
		if (formReport != Q_NULLPTR)
		{
			connect(formReport, &QPushButton::clicked, this, &CT::formReportPressed);
		}
		else qDebug() << "Could not load Report Wizard ui";
		srcTree = reportWizard->findChild<QListWidget*>("srcTree");
		if (srcTree == Q_NULLPTR)
			qDebug() << "Could not load Report Wizard ui";
		else
		{
			connect(srcTree, &QListWidget::currentRowChanged, this, &CT::leftSelected);
			//completeSrcTree();
		}
		destTree = reportWizard->findChild<QListWidget*>("destTree");
		if (destTree == Q_NULLPTR)
			qDebug() << "Could not load Report Wizard ui";
		else
			connect(destTree, &QListWidget::currentRowChanged, this, &CT::rigthSelected);
		
		pb2 = reportWizard->findChild<QPushButton*>("pushButton");
		if (pb2 != Q_NULLPTR)
			connect(pb2, &QPushButton::clicked, this, &CT::showInExplorerPressed);

	}
	else
		qDebug() << "Could not load Report Wizard window ui!";

	// loading correction windows
	file.setFileName(UI_CORRECTIONS);
	ok = file.open(QIODevice::ReadOnly | QIODevice::Text);
	qDebug() << "Opening corrections form: " << ok;
	cor = (QMainWindow*)_loader.load(&file, this);
	file.close();
	if (cor != Q_NULLPTR)
	{
		//cor->setWindowFlags(Qt::FramelessWindowHint);
		bool ok = true;
		QDoubleSpinBox* fff;

		for (quint64 l = 0; l < 6; l++)
		{
			akA.append(QVector<QDoubleSpinBox* >());
			for (quint64 k = 0; k < 4; k++)
			{
				//qDebug() << l * 4 + k << " " << spins.at(l * 4 + k);
				fff = cor->findChild<QDoubleSpinBox*>(spins.at(l * 4 + k));
				akA[l].append(fff);
				if (fff == Q_NULLPTR)
				{
					qDebug() << "Error while loading spinbox: " << spins.at(l * 4 + k);
					ok = false;
				}
			}
		}
		smaSpin = cor->findChild<QSpinBox*>("smaSpin");
		sma2Spin = cor->findChild<QSpinBox*>("smaSpinBox");
		d0 = cor->findChild<QDoubleSpinBox*>("ds1");
		d1 = cor->findChild<QDoubleSpinBox*>("ds2");
		d2 = cor->findChild<QDoubleSpinBox*>("ds3");
		d3 = cor->findChild<QDoubleSpinBox*>("ds4");
		d4 = cor->findChild<QDoubleSpinBox*>("ds5");
		d5 = cor->findChild<QDoubleSpinBox*>("ds6");

		connect(d0, SIGNAL(valueChanged(qreal)), this, SLOT(updMulti(qreal)));
		connect(d1, SIGNAL(valueChanged(qreal)), this, SLOT(updMulti(qreal)));
		connect(d2, SIGNAL(valueChanged(qreal)), this, SLOT(updMulti(qreal)));
		connect(d3, SIGNAL(valueChanged(qreal)), this, SLOT(updMulti(qreal)));
		connect(d4, SIGNAL(valueChanged(qreal)), this, SLOT(updMulti(qreal)));
		connect(d5, SIGNAL(valueChanged(qreal)), this, SLOT(updMulti(qreal)));
		//connnect(smaSPi);
		okCorBtn = cor->findChild<QPushButton*>("saveButton");
		canCorBtn = cor->findChild<QPushButton*>("cancelButton");
		if (okCorBtn == Q_NULLPTR || canCorBtn == Q_NULLPTR || !ok)
		{
			cor->deleteLater();
		}

		if (cor == Q_NULLPTR)
		{
			qDebug() << "Eror while loading some ui data!";
		}
		else
		{
			connect(okCorBtn, &QPushButton::clicked, this, &CT::updMulti);
			connect(canCorBtn, &QPushButton::clicked, this, &CT::updMulti);
			connect(smaSpin, SIGNAL(valueChanged(int)), this, SLOT(updSpin(int)));
			connect(sma2Spin, SIGNAL(valueChanged(int)), this, SLOT(updMainSpin(int)));
			for (quint64 i = 0; i < 6; i++)
				for (quint64 j = 0; j < 4; j++)
				{
					//akA[i][j]->setValue(CORR[i][j]);
					connect(akA[i][j], SIGNAL(valueChanged(double)), this, SLOT(updVals(double)));
				}
			/*mults[0] = CORR[0][4];
			d0->setValue(mults[0]);
			
			mults[1] = CORR[1][4];
			d1->setValue(mults[1]);
			
			mults[2] = CORR[2][4];
			d2->setValue(mults[2]);
			
			mults[3] = CORR[3][4];
			d3->setValue(mults[3]);

			mults[4] = CORR[4][4];
			d4->setValue(mults[4]);

			mults[5] = CORR[5][4];
			d5->setValue(mults[5]);*/
			//loaded = true;
		}
	}

	// Подгрузка меню
	file.setFileName(UI_MENU_WINDOW);
	ok = file.open(QIODevice::ReadOnly | QIODevice::Text);
	qDebug() << "Opening menu window form: " << ok;
	menuWid = (QDialog*)_loader.load(&file, this);
	file.close();
	if (menuWid != Q_NULLPTR)
	{
		//menuWid->setWindowFlags(Qt::FramelessWindowHint);
		menuWid->hide();
		mbRep = menuWid->findChild<QPushButton*>("printResultsButton");
		mbCorr = menuWid->findChild<QPushButton*>("correctTargetsButton");
		mbExit = menuWid->findChild<QPushButton*>("exitButton");
		mbShutdown = menuWid->findChild<QPushButton*>("shutdownButton");

		connect(mbRep, &QPushButton::clicked, this, &CT::mbReport);
		connect(mbCorr, &QPushButton::clicked, this, &CT::showCorr);
		connect(mbExit, &QPushButton::clicked, this, &CT::exitButton);
		connect(mbShutdown, &QPushButton::clicked, this, &CT::exitAndShutdown);
	}
}

CT::~CT()
{
}

void CT::button0Pressed()
{
	if (currentIndex != 0)
	{
		currentIndex = 0;
		switchScene();
	}
}

void CT::button1Pressed()
{
	if (currentIndex != 1)
	{
		currentIndex = 1;
		switchScene();
	}
}

void CT::replotGraphs()
{
	//pl->graph(0)->setData(*vx, *v0);
	//pl->graph(1)->setData(*vx, *v1);
	//pl->graph(2)->setData(*vx, *v2);
	//pl->graph(3)->setData(*vx, *v3);
	//pl->replot();
	if (currentIndex < KE_IN_PORTS)
		redrawPlots(currentIndex);
	//w->update();
	//w->repaint();
}

void CT::button2Pressed()
{
	if (currentIndex != 2)
	{
		currentIndex = 2;
		switchScene();
	}
}

void CT::button3Pressed()
{
	if (currentIndex != 3)
	{
		currentIndex = 3;
		switchScene();
	}
}

void CT::button4Pressed()
{
	if (currentIndex != 4)
	{
		currentIndex = 4;
		switchScene();
	}
}

void CT::button5Pressed()
{
	if (currentIndex != 5)
	{
		currentIndex = 5;
		switchScene();
	}
}

void CT::showDialog()
{
	if (_dlg != Q_NULLPTR)
	{
		_dlg->show();
	}
}

void CT::switchScene()
{
	if (/*targtes != Q_NULLPTR &&*/ currentIndex >= 0 && targets != Q_NULLPTR && currentIndex < KE_IN_PORTS && caption.size()>0)
	{
		targets->setVisible(true);
		if (targets->widget(currentIndex) != 0)
			targets->setCurrentIndex(currentIndex);
		if (caption.at(currentIndex) != Q_NULLPTR)
			caption.at(currentIndex)->setText(tr(DEF_TARGET_WIDGET_CAPTION) + QString::number(currentIndex + 1));
		update();
	}

	if (currentIndex < practiceStarted.size() && currentIndex < connectedTargets.size())
		if (!practiceStarted.at(currentIndex) && connectedTargets.at(currentIndex))
			showDialog();
}

void CT::initTargetView(QGraphicsView * view, QGraphicsScene* scene)
{
	if (view != Q_NULLPTR && scene != Q_NULLPTR)
	{
		view->setScene(scene);
		int vvv = (SCENE_RECT_X - TARGET_Y_SIZE) / 2;
		//QGraphicsRectItem* r1 = new QGraphicsRectItem(0, round((TARGET_Y_SIZE)*1.0 / 3.0), TARGET_X_SIZE, round((TARGET_Y_SIZE)*2.0 / 3.0));
		QGraphicsRectItem* r1 = new QGraphicsRectItem(vvv, vvv+round((TARGET_Y_SIZE)*1.0 / 3.0), TARGET_X_SIZE, round((TARGET_Y_SIZE)*2.0 / 3.0));
		//QGraphicsRectItem* r2 = new QGraphicsRectItem(round((TARGET_X_SIZE)*1.0 / 4.0), 0, round((TARGET_X_SIZE) / 2.0), round((TARGET_Y_SIZE)*1.0 / 3.0));
		QGraphicsRectItem* r2 = new QGraphicsRectItem(vvv+round((TARGET_X_SIZE)*1.0 / 4.0), vvv, round((TARGET_X_SIZE) / 2.0), round((TARGET_Y_SIZE)*1.0 / 3.0));
		r1->setPen(QPen(Qt::darkGreen, 2, Qt::SolidLine, Qt::RoundCap));
		r2->setPen(QPen(Qt::darkGreen, 2, Qt::SolidLine, Qt::RoundCap));
		r1->setBrush(QBrush(Qt::darkGreen));
		r2->setBrush(QBrush(Qt::darkGreen));
		scene->addItem(r1);
		scene->addItem(r2);
		QGraphicsTextItem* t;
		int mod = TARGET_X_SIZE / 5;

		t = new QGraphicsTextItem("10");
		t->setPos(round(TARGET_X_SIZE * 0.399), round(TARGET_Y_SIZE * 0.498));
		t->setRotation(315.0);
		t->setDefaultTextColor(Qt::white);
		t->setScale(3.0);
		scene->addItem(t);

		t = new QGraphicsTextItem("9");
		t->setPos(round(TARGET_X_SIZE * 0.315), round(TARGET_Y_SIZE * 0.585));
		t->setRotation(315.0);
		t->setDefaultTextColor(Qt::white);
		t->setScale(3.0);
		scene->addItem(t);

		t = new QGraphicsTextItem("8");
		t->setPos(round(TARGET_X_SIZE * 0.243), round(TARGET_Y_SIZE * 0.66));
		t->setRotation(315.0);
		t->setDefaultTextColor(Qt::white);
		t->setScale(3.0);
		scene->addItem(t);

		t = new QGraphicsTextItem("7");
		t->setPos(round(TARGET_X_SIZE * 0.178), round(TARGET_Y_SIZE * 0.73));
		t->setRotation(315.0);
		t->setDefaultTextColor(Qt::white);
		t->setScale(3.0);
		scene->addItem(t);

		t = new QGraphicsTextItem("6");
		t->setPos(round(TARGET_X_SIZE * 0.110), round(TARGET_Y_SIZE * 0.797));
		t->setRotation(315.0);
		t->setDefaultTextColor(Qt::white);
		t->setScale(3.0);
		scene->addItem(t);

		t = new QGraphicsTextItem("5");
		t->setPos(round(TARGET_X_SIZE * 0.044), round(TARGET_Y_SIZE * 0.87));
		t->setRotation(315.0);
		t->setDefaultTextColor(Qt::white);
		t->setScale(3.0);
		scene->addItem(t);


		for (quint64 i = 0; i <= 5; i++)
			scene->addEllipse(SCENE_RECT_X / 2 - i*mod / 2, SCENE_RECT_Y/ 2 - i*mod / 2, i*mod, i*mod, QPen(Qt::white, 2, Qt::SolidLine, Qt::RoundCap));

		//scene->addLine(0, 0, TARGET_X_SIZE/2, TARGET_Y_SIZE/2, QPen(Qt::magenta, 2, Qt::SolidLine, Qt::RoundCap));
		//scene->addLine(0, 0, SCENE_RECT_X / 2, SCENE_RECT_Y / 2, QPen(Qt::magenta, 2, Qt::SolidLine, Qt::RoundCap));
		//update();
	}
	else
		qDebug() << "Error while creating new GraphicsView for TargetWidget!";
}

void CT::initScenes(QVector<QGraphicsScene*> scenes)
{
	if (scenes.size() > 0)
	{
		for (quint64 i = 0; i < scenes.size(); i++)
			if (scenes.at(i) != Q_NULLPTR)
			{
				scenes[i]->clear();
			}
	}
}

void CT::redrawPlots(quint64 index)
{
	if (xx.size() > index && xx.at(index).size() <= CORE_GRAPH_COUNT_IN_TARGET &&
		yy.size() > index && yy.at(index).size() <= CORE_GRAPH_COUNT_IN_TARGET
		&& _graphs.size() >= CORE_GRAPH_COUNT_IN_TARGET)
	{
		for (quint64 i = 0; i < CORE_GRAPH_COUNT_IN_TARGET; i++)
		{
			if (_graphs.at(index).at(i) != Q_NULLPTR && xx.at(index).at(i).size()>0)
			{
				_graphs.at(index).at(i)->graph(0)->setData(xx.at(index).at(i), yy.at(index).at(i));
				_graphs.at(index).at(i)->rescaleAxes();
				//_graphs.at(index).at(i)->graph(0)->setData()
				_graphs.at(index).at(i)->replot();
			}
		}
	}
	else
		qDebug() << "Some graph data is null or invalid! id = " << index;
}

void CT::clearGraphData()
{
	QVector<qreal> vx;
	QVector<qreal> vy;
	for (quint64 i = 0; i < KE_IN_PORTS; i++)
	{
		for (quint64 j = 0; j < CORE_GRAPH_COUNT_IN_TARGET; j++)
		{
			if (xx[i][j].size()>0)
				xx[i][j].clear();
			if (yy[i][j].size()>0)
				yy[i][j].clear();
		}
	}
	//xx.clear();
	//yy.clear();
	for (quint64 i = 0; i < KE_IN_PORTS; i++)
	{
		for (quint64 j = 0; j < CORE_GRAPH_COUNT_IN_TARGET; j++)
		{
			//vx = new QVector<qreal>();
			//vy = new QVector<qreal>();
			xx[i][j].append(QVector<qreal>());
			yy[i][j].append(QVector<qreal>());
		}
	}
}

void CT::prevShotButtonPressed()
{
	if (currentIndex < sceneSizes.size() && currentIndex < sceneIndexes.size() && currentIndex>0)
	{
		if (sceneIndexes.at(currentIndex) > 0)
		{
			if (tarViews.at(currentIndex) != Q_NULLPTR)
			{
				sceneIndexes[currentIndex]--;
				tarViews.at(currentIndex)->setScene(scenes.at(currentIndex).at(sceneIndexes.at(currentIndex)));
			}
			else
				qDebug() << "Current Graphics View is null! Id = " << currentIndex;
		}
	}
	else
		qDebug() << "Scenes size is less then selected index = " << currentIndex;
}

void CT::zoomLessButtonPressed()
{
	if (currentIndex < tarViews.size() && tarViews.at(currentIndex) != Q_NULLPTR && zoom.at(currentIndex)>CORE_ZOOM_MIN)
	{
		tarViews.at(currentIndex)->scale(1.0 - CORE_ZOOM_STEP, 1.0 - CORE_ZOOM_STEP);
		zoom[currentIndex] -= CORE_ZOOM_STEP;
	}
	else
		qDebug() << "Wrong graphics view index while zoom- operation: " << currentIndex;
}

void CT::zoomMoreButtonPressed()
{
	if (currentIndex < tarViews.size() && tarViews.at(currentIndex) != Q_NULLPTR && zoom.at(currentIndex) < CORE_ZOOM_MAX)
	{
		tarViews.at(currentIndex)->scale(1.0 + CORE_ZOOM_STEP, 1.0 + CORE_ZOOM_STEP);
		zoom[currentIndex] += CORE_ZOOM_STEP;
	}
	else
		qDebug() << "Wrong graphics view index while zoom+ operation: " << currentIndex;
}

void CT::nextShotButtonPressed()
{
	if (currentIndex < sceneSizes.size() && currentIndex < sceneIndexes.size())
	{
		if (sceneIndexes.at(currentIndex) < (sceneSizes.at(currentIndex)- 1))
		{
			if (tarViews.at(currentIndex) != Q_NULLPTR)
			{
				sceneIndexes[currentIndex]++;
				tarViews.at(currentIndex)->setScene(scenes.at(currentIndex).at(sceneIndexes.at(currentIndex)));
			}
			else
				qDebug() << "Current Graphics View is null! Id = " << currentIndex;
		}
	}
	else
		qDebug() << "Scenes size is less then selected index = " << currentIndex;
}

void CT::reportManagerButtonPressed()
{
	if (reportWizard != Q_NULLPTR)
	{
		if (srcTree != Q_NULLPTR)
			srcTree->clearSelection();
		if (destTree != Q_NULLPTR)
		{
			srcTree->clearSelection();
			destTree->clear();
		}
		completeSrcTree();
		reportWizard->show();
	}
	else
		qDebug() << "Could not launch report wizard, because reportWizard is null!";
}

void CT::newPracticeButtonPressed()
{
	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, tr(CLOSE_PRACTICE_CAPTION),
		tr(CLOSE_PRACTICE_MESSAGE),
		QMessageBox::Yes | QMessageBox::No);
	if (reply == QMessageBox::Yes)
		showDialog();
}

void CT::menuShowPressed()
{
// show menu
	menuWid->setModal(Qt::WindowModality::WindowModal);
	menuWid->show();
}

void CT::dlgOkClicked()
{
	if (_dlgVal > 0)
	{
		sceneUpdating[currentIndex] = true;
		_dlg->hide();
		emit newPracticeSelected(currentIndex, _dlgVal);
	}
	//_dlgVal = 0;
}

void CT::dlgSpinValCHanged(int val)
{
	_dlgVal = val % CORE_MAX_SHOTS_COUNT;
}

// Из менеджера отчетов

void CT::showInExplorerPressed()
{
	if (!ls)
	{
		QString ss = "@start " + destTree->selectedItems().at(0)->text().replace(tr(ARC_FN), "");
		QProcess::startDetached(ss);
	}	
}

void CT::completeSrcTree()	
{
	//QDir d;
	// Заполняем дерево исходных отчетов 1 раз при запуске
	QString path;
		//QDirIterator it(, QDirIterator::Subdirectories);
		QDirIterator it(QDir::currentPath() + "\\" + tr(ARC_DIR), QStringList() <<ARC_FN, QDir::Files, QDirIterator::Subdirectories);
		while (it.hasNext())
		{
			QListWidgetItem* t = new QListWidgetItem(srcTree);
			t->setText(it.next());
			//srcTree->addItem(new QListWidgetItem(it.next()));
		}
	
}

void CT::scanFiles(QString fName)
{
	QDir d;
	if (d.cd(fName))
	{
		if (d.entryList().contains(ARC_FN))
		{
			srcTree->addItem(new QListWidgetItem(d.path()));
		}
		else
		{
			if (d.entryList().length()>0)
				for (quint64 i = 0; i < d.entryInfoList().length(); i++)
				{
					if (d.entryInfoList().at(i).isDir() && d.entryList().at(i)!=".")
						scanFiles(d.path() + "\\" + d.entryList().at(i));
				}
		}
	}
}

void CT::toRigthButtonPressed()
{
	if (ls)
	{
		if (sl >= 0 && srcTree->count() > 0)
		{
			destTree->addItem(srcTree->takeItem(sl));
			sl--;
			if (sl < 0 && srcTree->count() > 0) sl = 0;
		}
		srcTree->repaint();
		destTree->repaint();
	}
}

void CT::toLeftButtonPressed()
{
	if (!ls)
	{
		if (sr >= 0 && destTree->count()>0)
			srcTree->addItem(destTree->takeItem(sr));
		sr--;
		if (sr < 0 && destTree->count() > 0) sr = 0;
		srcTree->repaint();
		destTree->repaint();
	}
}

void CT::formReportPressed()
{
	if (destTree->selectedItems().size() > 0)
	{
		quint64 size = destTree->count();
		//QList<QFile> files;
		//QList<QByteArray> ll;
		QList<QStringList> ss;
		QStringList res;
		quint64 size2 = 0;
		for (quint64 i = 0; i < size; i++)
		{
			QFile file(destTree->item(i)->text()); //+ "\\" + tr(ARC_FN));
			file.open(QIODevice::ReadOnly);

			ss.append(QString::fromUtf8(file.readAll()).split('\n'));
			if (ss.last().size() > size2)
				size2 = ss.last().size();
			//res.append("");
		}	

		for (quint64 i = 0; i < size2; i++)
		{
			res.append("");
			for (quint64 j = 0; j < size; j++)
			{
				if (ss.at(j).size() > i)
					res[i].append(ss[j][i]+"       ");
				else
					res[i].append("   |          ");
			}
		}

		//qDebug() << res;
		QPrinter printer;
		QPrintDialog printDialog(&printer, this);
		if (printDialog.exec() == QDialog::Accepted) {
			QPainter painter(&printer);
			for (quint64 i=0; i<res.size();i++)
				painter.drawText(50, 50+i*50, res.at(i));
		}
	}
}

void CT::leftSelected(int row)
{
	ls = true;
	sl = row;
}

void CT::rigthSelected(int row)
{
	ls = false;
	sr = row;
}

void CT::nextShot(quint64 index, guiShotData* pRes)
{
	// Создаем новую сцену и перерисовываем графики
	if (index < tarViews.size() && tarViews.at(index) != Q_NULLPTR)
	{
		if (!firstTime) {
			//QGraphicsScene* foo = new QGraphicsScene(this); // Создаем новую сцену
			sceneSizes[index]++;
			initTargetView(tarViews[index], scenes[index][sceneSizes[index] - 1]); // Рисуем на ней мишень
			sceneIndexes[index] = sceneSizes.at(index) - 1; // Говорим что текущая выбранная сцена в данном виджете - последняя
			fillValues(index, pRes);
			redrawPlots(index);
			lines[index].clear();
			lids[index]->clear();
		}
		else
			firstTime = false;
	}
	else
		qDebug() << "Wrong data index received from Practice: " << index;
}

void CT::fillValues(quint64 index, guiShotData* pRes)
{
	if (pRes != Q_NULLPTR && index < KE_IN_PORTS)
	{
		// Заполним поля формы

		if (index < averageResult.size() && averageResult.at(index) != Q_NULLPTR)
			averageResult.at(index)->setText(QString::number(pRes->result));

		if (index < averageMark.size() && averageMark.at(index) != Q_NULLPTR)
			averageMark.at(index)->setText(QString::number(pRes->averageResult));

		if (index < recommendations.size() && recommendations.at(index) != Q_NULLPTR)
			recommendations.at(index)->setText(pRes->recomendations);

		if (index < currentShotInfo.size() && currentShotInfo.at(index) != Q_NULLPTR)
		{
			currentShotInfo.at(index)->clear();
			for (quint64 i = 0; i < pRes->shotInfo.size(); i++)
				currentShotInfo.at(index)->appendPlainText(pRes->shotInfo.at(i));
		}

		if (index < allShotsInfo.size() && allShotsInfo.at(index) != Q_NULLPTR)
		{
			allShotsInfo.at(index)->clear();
			for (quint64 i = 0; i < pRes->allShotsInfo.size(); i++)
				allShotsInfo.at(index)->appendPlainText(pRes->allShotsInfo.at(i));
		}

		// Заполним массивы для графиков

		//if (xx.at(index).at(0) != Q_NULLPTR)
			xx[index][0].append(pRes->shotNum);
		//if (yy.at(index).at(0) != Q_NULLPTR)
			yy[index][0].append(pRes->result);

		//if (xx.at(index).at(1) != Q_NULLPTR)
			xx[index][1].append(pRes->shotNum);
		//if (yy.at(index).at(1) != Q_NULLPTR)
			yy[index][1].append(pRes->shotTime);

		//if (xx.at(index).at(2) != Q_NULLPTR)
			xx[index][2].append(pRes->shotNum);
		//if (yy.at(index).at(2) != Q_NULLPTR)
			yy[index][2].append(pRes->shotKren);

		//if (xx.at(index).at(3) != Q_NULLPTR)
			xx[index][3].append(pRes->shotNum);
		//if (yy.at(index).at(3) != Q_NULLPTR)
			yy[index][3].append(pRes->shotTraectory);
	}
	else
		qDebug() << "Information recieved from Practice class was corrupted!";
}

void CT::practiceEnded(quint64 index, guiShotData* pRes)
{
	// Создаем новую сцену и перерисовываем графики
	if (index < tarViews.size() && tarViews.at(index) != Q_NULLPTR)
	{
		saveAllImages(index, pRes);

		fillValues(index, pRes);
		redrawPlots(index);
		if (index < practiceStarted.size())
			practiceStarted[index] = false;
	}
	else
		qDebug() << "Wrong data index received from Practice: " << index;
}

void CT::newPractice(quint64 index, quint64 shotCount)
{

	// Чистим данные и обновляем виджеты
	if (index < KE_IN_PORTS)
	{
		if (!firstRun)
			firstTime = true;
		else
			firstRun = false;
		sceneUpdating[index] = true;
		if (index < fioField.size() && fioField.at(index) != Q_NULLPTR)
			fioField.at(index)->clear();

		if (index < zvanie.size() && zvanie.at(index) != Q_NULLPTR)
			zvanie.at(index)->clear();

		if (index < addInfo.size() && addInfo.at(index) != Q_NULLPTR)
			addInfo.at(index)->clear();

		if (index < averageResult.size() && averageResult.at(index) != Q_NULLPTR)
			averageResult.at(index)->clear();

		if (index < averageMark.size() && averageMark.at(index) != Q_NULLPTR)
			averageMark.at(index)->clear();

		if (index < recommendations.size() && recommendations.at(index) != Q_NULLPTR)
			recommendations.at(index)->clear();

		if (index < currentShotInfo.size() && currentShotInfo.at(index) != Q_NULLPTR)
		{
			currentShotInfo.at(index)->clear();
		}

		if (index < allShotsInfo.size() && allShotsInfo.at(index) != Q_NULLPTR)
		{
			allShotsInfo.at(index)->clear();
		}

// Очищаем сцену
		if (index < scenes.size())
		{
			sceneSizes[index] = 1;
			sceneIndexes[index] = 0;
			initScenes(scenes[index]);
			initTargetView(tarViews[index], scenes[index][0]);

			//sceneSizes[index] = 1;
			//sceneIndexes[index] = 0;
		}

		if (index < practiceStarted.size())
			practiceStarted[index] = true;

#ifndef TEST_MODE
		clearGraphData();
#endif
		sceneUpdating[index] = false;
	}
	else
		qDebug() << "Index for cleaning from Practice class was corrupted!";
}

/*qreal CT::transX(qreal x, quint64 index)
{
	qreal res = SCENE_RECT_X / 2 + x*COM_ADC_POWER*mults.at(index);
	qDebug() << " x2 = " << res << " x = " << x*COM_ADC_POWER*mults.at(index);
	return res;
}

qreal CT::transY(qreal y, quint64 index)
{
	qreal res = SCENE_RECT_Y/ 2 + y*COM_ADC_POWER*mults.at(index);
	qDebug() << " y2 = " << res;
	return res;
}*/

void CT::targetConnected(quint64 index)
{
	if (index >= 0 && index < KE_IN_PORTS)
	{
		connectedTargets.replace(index, true);
		if (!practiceStarted.at(index) && currentIndex  == index)
			showDialog();
	}
	else
		qDebug() << "Signal targetConnected has invalid target id = " << index;
}

void CT::targetDisconnected(quint64 index)
{
	if (index >= 0 && index < KE_IN_PORTS)
	{
		connectedTargets.replace(index, false);
	}
	else
		qDebug() << "Signal targetDisconnected has invalid target id = " << index;
}

void CT::plotGraph0(quint64 id, qreal x)
{
#ifdef TEST_MODE
	if (id == currentIndex)
	{
		//yy[currentIndex][0]->append(x);
		//yy[currentIndex][0]->pop_front();
	}
#endif // TEST_MODE
}

void CT::plotGraph1(quint64 id, qreal x)
{
#ifdef TEST_MODE
	if (id == currentIndex)
	{
		//yy[currentIndex][1]->append(x);
		//yy[currentIndex][1]->pop_front();
	}
#endif // TEST_MODE
}

void CT::plotGraph2(quint64 id, qreal x)
{
#ifdef TEST_MODE
	if (id == currentIndex)
	{
		//yy[currentIndex][2]->append(x);
		//yy[currentIndex][2]->pop_front();
	}
#endif // TEST_MODE
}

void CT::plotGraph3(quint64 id, qreal x)
{
#ifdef TEST_MODE
	if (id == currentIndex)
	{
		//yy[currentIndex][3]->append(x);
		//yy[currentIndex][3]->pop_front();
	}
#endif // TEST_MODE
}

void CT::logTo(quint64 index, QString mes)
{
	if (index<KE_IN_PORTS)
	{ 
		//currentShotInfo.at(index)->appendPlainText(mes);
	}
}

void CT::removeLine(quint64 index, quint64 dotId)
{ // not work
	if (index == currentIndex)
	{
		if (lids.at(currentIndex) != Q_NULLPTR)
		{
			bool ok = false;
			for (quint64 i = 0; i < lids.at(currentIndex)->size(); i++)
			{
				if (lids.at(currentIndex)->at(i) == dotId)
				{
					ok = true;
					scenes.at(currentIndex).at(sceneIndexes.at(currentIndex)-1)->removeItem(lines.at(currentIndex).at(i));
					lines[currentIndex].removeAt(i);
					lids[currentIndex]->removeAt(i);
				}
			}
			if (!ok)
				qDebug() << "Could not remove dot: id not found, id = " << dotId;
		}
	}
}


void CT::repaintDot(quint64 index, quint64 id, quint64 dotId)
{
	if (index < lids.size())
	{
		if (lids.at(index) != Q_NULLPTR)
		{
			bool ok = false;
			for (quint64 i = 0; i < lids.at(index)->size(); i++)
			{
				if (lids.at(index)->at(i) == dotId)
				{
					ok = true;
					lines.at(index).at(i)->setPen(QPen(COLOR_VALUES.at(id), THIN_VALUES.at(id), Qt::SolidLine, Qt::RoundCap));
				}
			}
			if (!ok)
				qDebug() << "Could not repaint dot: id not found, id = " << dotId;
		}
	}
}

void  CT::moveDot(quint64 index, quint64 id, qreal newx, qreal newy, qreal newx2, qreal newy2, quint64 dotId)
{
	if (index < lids.size())
	{
		if (lids.at(index) != Q_NULLPTR)
		{
			bool ok = false;
			for (quint64 i = 0; i < lids.at(index)->size(); i++)
			{
				if (lids.at(index)->at(i) == dotId)
				{
					ok = true;
					//lines.at(index).at(i)->setLine(transX(newx, index), transY(newy, index), transX(newx2, index), transY(newy2, index));
					lines[index][i]->setLine(SCENE_RECT_X / 2.0 + newx, SCENE_RECT_X / 2.0 + newy, SCENE_RECT_X / 2.0 + newx2 , SCENE_RECT_X / 2.0 + newy2);
					lines[index][i]->setPen(QPen(COLOR_VALUES.at(id), THIN_VALUES.at(id), Qt::SolidLine));
				}
			}
			if (!ok)
				qDebug() << "Could not move dot: id not found, id = " << dotId;
		}
	}
}

void CT::paintDot(quint64 index, quint64 id, qreal x, qreal y, qreal x2, qreal y2, quint64 dotId)
{
	if (practiceStarted.at(index) && !sceneUpdating.at(index))
	{
		if (index < sceneSizes.size() && sceneSizes.at(index)>0)
		{
			QGraphicsScene* sc = scenes.at(index).at(sceneSizes.at(index) - 1);

			QGraphicsLineItem* line;
				//line = new QGraphicsLineItem(newX, newY, newX2, newY2);
			line = new QGraphicsLineItem(SCENE_RECT_X / 2.0 + x, SCENE_RECT_X / 2.0 + y, SCENE_RECT_X / 2.0 + x2, SCENE_RECT_X / 2.0 + y2);
				line->setPen(QPen(COLOR_VALUES.at(id), THIN_VALUES.at(id), Qt::SolidLine));
				lines[index].push_back(line);
				lids[index]->append(dotId);
				sc->addItem(line);
	
			//tarViews[currentIndex]->update();
		}
		else
			qDebug() << "Could not draw point, unknown ID = " << id;

	}

}

bool CT::event(QEvent *event)
{
	if (event->type() == QEvent::KeyPress) {
		QKeyEvent *ke = static_cast<QKeyEvent *>(event);
		if (ke->key() == Qt::Key::Key_F11) {
			if (cor != Q_NULLPTR)
				if (cor->isHidden())
					cor->show();
				else
					cor->hide();
			return true;
		}
	}

	return QWidget::event(event);
}

void CT::okBtn()
{
	if (cor != Q_NULLPTR)
	{
		for (quint64 i = 0; i < 6; i++)
			for (quint64 j = 0; j < 4; j++)
				CORR[i][j] = akA[i][j]->value();
		updMulti(0);
		emit saveCorrection(CORR);
	}
}

void CT::canBtn()
{
	if (cor != Q_NULLPTR)
	{
		inWork = true;
		cor->hide();
		for (quint64 i = 0; i < 6; i++)
			for (quint64 j = 0; j < 4; j++)
			{
				//qDebug() << "row " << i << " col " << j << " name " << akA[i][j]->objectName() << " val " << CORR[i][j];
				akA[i][j]->setValue(CORR[i][j]);
			}
		//mults[0] = CORR[0][4];
		d0->setValue(mults[0]);
		//mults[1] = CORR[1][4];
		d1->setValue(mults[1]);
		//mults[2] = CORR[2][4];
		d2->setValue(mults[2]);
		//mults[3] = CORR[3][4];
		d3->setValue(mults[3]);
		//mults[4] = CORR[4][4];
		d4->setValue(mults[4]);
		//mults[5] = CORR[5][4];
		d5->setValue(mults[5]);
		inWork = false;
		loaded = true;
	}
}

void CT::updVals(double val)
{
	if (!inWork && loaded && cor != Q_NULLPTR)
	{
		for (quint64 i = 0; i < 6; i++)
			for (quint64 j = 0; j < 4; j++)
				CORR[i][j] = akA[i][j]->value();
		emit setCorrection(CORR);
	}
}

void CT::updSpin(int val)
{
	emit setSmaPer(val);
}

void CT::updMainSpin(int val)
{
	emit setUserSmaPer(val);
}

void CT::updMulti(double val)
{
	if (loaded)
	{
		mults[0] = d0->value();
		mults[1] = d1->value();
		mults[2] = d2->value();
		mults[3] = d3->value();
		mults[4] = d4->value();
		mults[5] = d5->value();

		CORR[0][4] = mults[0];
		CORR[1][4] = mults[1];
		CORR[2][4] = mults[2];
		CORR[3][4] = mults[3];
		CORR[4][4] = mults[4];
		CORR[5][4] = mults[5];

		emit setMult(mults);
		emit saveCorrection(CORR);
	}
}

/*void CT::runExplorer()
{
	QProcess *process = new QProcess(this);
	QString file = EXPLORER_PATH;
	process->start(file);
}*/

void CT::exitAndShutdown()
{
	QProcess::startDetached("shutdown -s -f -t 10");
	this->close();
}

void CT::loadCorrection(QList<QList<qreal>> v)
{
	for (quint64 i = 0; i < 6; i++)
		for (quint64 j = 0; j < 5; j++)
		{
			CORR[i][j] = v[i][j];
		}
	mults[0] = CORR[0][4];
	//d0->setValue(mults[0]);
	mults[1] = CORR[1][4];
	//d1->setValue(mults[1]);
	mults[2] = CORR[2][4];
	//d2->setValue(mults[2]);
	mults[3] = CORR[3][4];
	//d3->setValue(mults[3]);
	mults[4] = CORR[4][4];
	//d4->setValue(mults[4]);
	mults[5] = CORR[5][4];
	//d5->setValue(mults[5]);
	emit setCorrection(CORR);
	canBtn();
}

void CT::mbReport()
{
	menuWid->hide();
	reportManagerButtonPressed();
}

void CT::showCorr()
{
	menuWid->hide();
	if (cor != Q_NULLPTR)
	{
		cor->setWindowModality(Qt::WindowModality::WindowModal);
		if (cor->isHidden())
			cor->show();
		else
			cor->hide();
	}
}

void CT::exitButton()
{
	menuWid->hide();
	QApplication::exit();
}


void CT::playRifle(quint64 id)
{
	if (id == currentIndex)
	{
		//qDebug() << "Emit1";
		emit playRifle2(id);
	}
}

void CT::playResults(quint64 id, QString res, QString hour)
{
	if (id == currentIndex)
	{
		//qDebug() << "Emit2";
		emit playResults2(id, res, hour);
	}
}

void CT::saveAllImages(quint64 index, guiShotData* dat)
{
	QDateTime dt = QDateTime::currentDateTime();
	QString path = checkDirs(index, dt);

	if (sceneSizes.at(index) > 0)
	{
		for (quint64 i = 0; i < sceneSizes.at(index); i++)
		{
			QGraphicsScene* cur = scenes.at(index).at(i);
			const qreal width = cur->width();
			const qreal height = cur->height();
			QImage image(width, height, QImage::Format_RGB888);
			QPainter painter(&image);
			painter.setRenderHint(QPainter::Antialiasing);
			cur->render(&painter, QRectF(0, 0, width, height), QRect(0, 0, width, height));
			image.save(path + "\\" + QString::number(i + 1) + ".png");
		}
	}

	QFile f(path + "\\" + tr(ARC_FN));
	if (f.open(QIODevice::ReadWrite))
	{
		f.write(tr(MW).toUtf8());
		f.write(tr("\n").toUtf8());
		f.write((dt.toString("yyyy-MM-dd hh:mm:ss_") + fioField[index]->text()).toUtf8());
		f.write(tr("\n").toUtf8());
		f.write(tr(MW2).toUtf8());
		f.write(tr("\n").toUtf8());
		if (dat != Q_NULLPTR && dat->res.size() > 0)
			for (quint64 i = 0; i < dat->res.size(); i++)
			{
				f.write((QString::number(i)+" | "+QString::number(dat->res.at(i))).toUtf8());
				f.write(tr("\n").toUtf8());
			}
		f.flush();
		f.close();
	}
	else
		qDebug() << "Could not open file for results: " << path;
}

QString CT::checkDirs(quint64 index, QDateTime dt)
{
	QDir d;
	QStringList s;
	s.append(ARC_DIR);
	s.append(dt.toString("yyyy"));
	s.append(dt.toString("MM"));
	s.append(dt.toString("dd"));
	s.append(dt.toString("hh"));
	s.append(dt.toString("mm-ss_")+fioField[index]->text());

	for (quint64 i = 0; i < s.size(); i++)
	{
		if (!d.cd(s.at(i)))
		{
			if (!d.mkdir(s.at(i)))
				qDebug() << "Could not create archieve directory: " << s.at(i);
			else
				d.cd(s.at(i));
					
		}
	}
	return d.path();
}