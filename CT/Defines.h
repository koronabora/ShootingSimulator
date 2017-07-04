#pragma once

#include <QList>
#include <QColor>
#include <QVector>
#include <QSerialPort>
#include <QObject>
#include <QSettings>

// Параметры COM порта
#define COM_TARGET_BAUDRATE 250000 // Скорость соединения с портом мишеней
#define COM_TARGET_DIVIDER " " // Разделитель для данных от мишеней
#define COM_TARGET_NAME "USB-SERIAL CH340" // Имя com порта мишени
#define COM_CONTROLLER_NAME "USB Serial Port" // Имя com порта контроллера
#define COM_CONTROLLER_NAME2 "Kernelchip COM-USB" // Имя com порта контроллера
#define COM_CONTROLLER_BAUDRATE 38400 // Имя com порта контроллера
#define COM_ADC_POWER 1024 // разрядность АЦП мишени

// параметры контроллера Ke-USB24R
#define PORTS_UPD_TIMER 1000 // время обновления для поиска нового контроллера
#define KE_UPD_TIMER 10 // Таймер чтения данных с корнтроллера о нажатой кнопке
#define KE_TEST_COMMAND "$KE" // Тестируем модуль
#define KE_TEST_ANSWER "#OK" // Ответ на тест
#define KE_READ_COMMAND "$KE,RD,ALL" // Читаем все
#define KE_READ_NOIN 'x'
#define KE_READ_HIGH '1'
#define KE_READ_LOW '0'
#define KE_READ_ANSWER "#RD" // Ответ на чтение всех
#define KE_REL_COMMAND "$KE,REL," // Управляем реле
#define KE_REL_ANSWER1 "#REL" // Ответ на управление реле
#define KE_REL_ANSWER2 "OK" // Ответ на управление реле
#define KE_DIVIDER "," // Разделитель при формировании команд
#define KE_ERR "#ERR"
#define KE_INIT_SET_LINE_PREF "$KE,IO,SET,"
#define KE_INIT_SET_LINE_SUFF ",1,S"
#define KE_IN_PORTS 6
#define KE_BOOM_PORTS 4
//#define KE_BOOM_DELAY 80

#define CORE_GRAPH_COUNT_IN_TARGET 4
#define CORE_MAX_SHOTS_COUNT 128
#define CORE_ZOOM_STEP 0.2
#define	CORE_ZOOM_MIN 0.1
#define CORE_ZOOM_MAX 5.0
#define CORE_MAX_LINES_IN_TARGET 256
#define CORE_TARGET_THRESHOLD 128
#define CORE_MAX_POINTS  45
#define NO_SHOT_COUNT_SELECTED "Неверное значение!"
#define CLOSE_PRACTICE_CAPTION "Обратите внимание"
#define CLOSE_PRACTICE_MESSAGE "Действительно остановить выполнение упражнения?"
#define DEF_TARGET_WIDGET_CAPTION "Мишень  № "
#define SB_MESSAGE "НАЧАЛО ВЫСТРЕЛА"
#define SE_MESSAGE "конец"
#define MARK_MESS "Внимание на: "
#define MARK_MESS_NO "Нет замечаний по траектории"
#define MM_1 "координацию, "
#define MM_2 "гор. устойчивость, "
#define MM_3 "верт. устойчивость, "
#define MW "Мишень №"
#define MW2 "№ выстрела | Результат"

enum KE
{
	TEST,
	INIT,
	READ,
	REL
};

#define CORE_DATA_SIZE 128 // !!!
#define CORE_SMA_PERIOD 10 // Сколько амплитуд усредняется для вывода
#define CORE_SMA2 20

static int TARGET_X_SIZE = 600;
static int TARGET_Y_SIZE = 600;
static int THE_TARGET_SIZE = 5;

#define SCENE_RECT_X 600
#define SCENE_RECT_Y 600

static int GRAPH_X_SIZE = 575;
static int GRAPH_Y_SIZE = 140;

static qreal MULT = COM_ADC_POWER*3;

static qreal RX = (TARGET_X_SIZE*1.0)/(2.0*MULT);
static qreal RY = (TARGET_Y_SIZE*1.0)/(2.0*MULT);

#define REDRAW_PERIOD 500 // таймер перерисовки графиков
#define CORE_GRAPHS_ALIASED false
#define GRAPGS_SIZE 8

#define _PI 3.14159265

#define OUT_DIR "D://"
#define TEXT_FILE_NAME "Результаты.txt"
#define IMAGE_FILE_NAME "Мишень.png"

#define UI_REPORT_WIZARD_FILE ":/Resources/ReportsWizard.ui"
#define UI_COUNT_CHOOSER_FILE ":/Resources/ShotCountChooser.ui"
#define UI_TARGET_FILE ":/Resources/Target.ui"
#define UI_CORRECTIONS ":/Resources/Settings.ui"
#define UI_MENU_WINDOW ":/Resources/MenuWindow.ui"

#define EXPLORER_PATH "C://Windows//explorer.exe"
#define ARC_DIR "Архив"
#define ARC_FN "Результаты.txt"

#define LOG_DIR "logs"
#define LOG_DATE_FORMAT "yyyy - MM - dd"
#define LOG_TIME_FORMAT "hh"

#define DEF_COFIG "config.ini"

#define COMPANY_NAME "ООО Котвич" 
#define SOFT_NAME "CT"

//#define TEST_MODE

#define MAX_AIMING_TIME 5000 // 5 секунд. Во время прицеливания считывается только последняя минута
#define AFTER_SHOT_TIME 2000 // 2 секунды. Время после выстрела, которое мы считаем как 
#define IN_SHOT_TIME 100 // интервал значений, считающихся временем выстрела 
#define IN_TURN_TIMEOUT 300 // значения выстрела в очереди
#define SOUND_TIMER 1000 // период обработки звуковых файлов
#define KREN_PERIOD 500 // период подсчета крена после выстрела
#define MARK_DIVIDER 8.0 // Чем больше, тем сложнее получить хорошие оценки по стрельбе
#define ANGLE_DIVIDER 3.0 // делитель

enum COLORS // по совместительству тип отображаемой точки и обозначение цвета
{
	AFTER_SHOT_OUTSIDE, // Статистика выстрела. Траектория при выходе за границы мишени
	AFTER_SHOT_INSIDE, // Статистика выстрела. Траектория внутри мишени
	IN_TIME_OF_SHOT, // Статистика выстрела. Траектория во время выстрела
	BEFORE_SHOT, // Текущий результат, линия прицеливания
	SHOT, // Место текущего попадания
	OTHER_SHOTS // Место других попаданий
};

static const QList<QColor> COLOR_VALUES
{
	QColor(Qt::GlobalColor::red), 
	QColor(Qt::GlobalColor::yellow),
	QColor(Qt::GlobalColor::green),
	QColor(Qt::GlobalColor::magenta),
	QColor(Qt::GlobalColor::red),
	QColor(Qt::GlobalColor::black)
};

static const QList<quint64> THIN_VALUES
{
	6,
	6,
	6,
	6,
	12,
	12
};

/*struct shotPoint
{
	qreal x; //
	qreal y; //
	qreal x2; //
	qreal y2; //
	quint64 time; //
	quint64 id;
};*/

struct shotResults
{
	quint8 res; //
	quint64 shotTime; //
	qreal dispAll; //
	qreal dispVert; //
	qreal dispHor; //
	qreal kren; //
};

struct shot
{
	QVector<qreal>* x;
	QVector<qreal>* y;
	QVector<quint64>* time;
	QVector<quint64>* id;

	quint64 startTime = 0;
	quint64 shotTime = 0; // Время регистрации нажатия на спусковой крючек
	quint64 endTime = 0;
	quint64 sceneId = 0;
	shotResults res;

	qreal maxValin1 = -32000; 
	qreal minValin1 = +32000;

	qreal maxValin2 = -32000;
	qreal minValin2  = + 32000;

	qreal maxValin3 = -32000;
	qreal minValin3 = +32000;

	qreal bX = 0;
	qreal bY = 0;
	qreal eX = 0;
	qreal eY = 0;

};

struct guiShotData // для передачи в интерфейс данных о выстреле
{
	quint64 result = 0; // Результат текущего выстрела. Также используется для графиков
	qreal averageResult = 0; // Средний результат.
	QString recomendations; // Рекомендации
	QStringList shotInfo; // Информация о выстреле
	QStringList allShotsInfo; // Информация о всех выстрелах
	qreal shotTime = 0; // даннные для графика "Время обработки выстрела"
	quint64 shotKren = 0; // данные для графика "Устойчивость к крену"
	quint64 shotTraectory = 0; // данные для графика "Устойчивость траектории"
	quint64 shotNum = 0;
	quint64 prom;
	qreal avTraectory;
	qreal avKren;
	QList<quint8> res;
};

enum STATES // состояния задания
{
	NC,
	BEFORE,
	INS,
	AFTER
};
#define CORRECTION_FILE "Corrections"

enum WEAPON_TYPES
{
	AK,
	PM,
	GR
};

static const QList<quint8> PORT_TYPES
{
	WEAPON_TYPES::AK,
	WEAPON_TYPES::AK,
	WEAPON_TYPES::PM,
	WEAPON_TYPES::PM,
	WEAPON_TYPES::GR,
	WEAPON_TYPES::GR
};

static const QStringList SHOT_SOUNDS
{
	":/Sound/ak.wav",
	":/Sound/ak.wav",
	":/Sound/pm.wav",
	":/Sound/pm.wav",
	":/Sound/pm.wav",
	":/Sound/pm.wav"
};


static const QList<bool> BOOM_AVALIABLE
{
	true,
	true,
	true,
	true,
	false,
	false
};

static const QList<bool> BURST_AVALIABLE
{
	true,
	true,
	false,
	false,
	false,
	false
};

static const QList<quint16> BOOM_DELAY
{
	140,
	140,
	120,
	120,
	120,
	120
};

static const QList<quint16> SHOTS_DELAY
{
	200,
	200,
	300,
	300,
	200,
	200
};

static const QStringList spins = {
"akA", "akB", "akC", "akD", "ak2A", "ak2B", "ak2C", "ak2D", "pmA", 
"pmB", "pmC", "pmD", "pm2A", "pm2B", "pm2C", "pm2D", "yaA", "yaB",
"yaC", "yaD", "ya2A", "ya2B", "ya2C", "ya2D"};