#pragma once

// Параметры COM порта
#define COM_TARGET_BAUDRATE 115200 // Скорость соединения с портом мишеней
#define COM_TARGET_DIVIDER " " // Разделитель для данных от мишеней
#define COM_TARGET_NAME "USB-SERIAL CH340" // Имя com порта мишени
#define COM_CONTROLLER_NAME "Ke-USB24R" // Имя com порта контроллера
#define COM_CONTROLLER_BAUDRATE 9600 // Имя com порта контроллера
#define COM_ADC_POWER 1024 // разрядность АЦП мишени
//#define COM_READ_TIMER 1 // время чтения мище

// параметры контроллера Ke-USB24R
#define PORTS_UPD_TIMER 3000 // время обновления для поиска нового контроллера
#define KE_UPD_TIMER 100 // Таймер чтения данных с корнтроллера о нажатой кнопке
#define KE_TEST_COMMAND "$KE" // Тестируем модуль
#define KE_TEST_ANSWER "#OK" // Ответ на тест
#define KE_READ_COMMAND "$KE,RD,ALL" // Читаем все
#define KE_READ_NOIN "x"
#define KE_READ_ANSWER "#RD," // Ответ на чтение всех
#define KE_REL_COMMAND "$KE,REL," // Управляем реле
#define KE_REL_ANSWER "#REL,OK" // Ответ на управление реле
#define KE_DIVIDER "," // Разделитель при формировании команд
#define KE_ERR "#ERR"
#define KE_SUFFIX "\\r\\n"
#define KE_INIT_SET_LINE_PREF "$KE,IO,SET,"
#define KE_INIT_SET_LINE_SUFF ",1,S"
#define KE_IN_PORTS 6

enum KE
{
	TEST,
	INIT,
	READ,
	REL
};

//
#define CORE_TARGET_TIMER 200 // таймер вычисления координат прицела
#define CORE_PLOT_TIMER 100 // таймер отправки данных на рисование
#define CORE_PEAK_TRESH 50 // max - min have to be >, if we want to found peak

#define CORE_DATA_SIZE 256 // !!!
#define CORE_AMP_COUNT 3 // Сколько амплитуд усредняется для вывода

#define CORE_SMA_PER 30
#define CORE_SIGN_FR 10 // частота сигнала
#define CORE_MOD_FR 100 // частота несущей
#define CORE_IQGETER_FR 1000 // частота гетеродина при разложении на I и Q
#define CORE_IQGETER_MULT CORE_IQGETER_FR/CORE_MOD_FR // множитель количества элементов в выборках I b Q
#define CORE_MSECS_IN_SEC 1000
#define CORE_FIR_DEPTH 32
#define CORE_FIR_BAND_FREQ 20
#define CORE_FIR_ATTUN_FREQ 50

#define CORE_WAVELET_STAGES 2
#define CORE_WAVELET_NAME "db4"
#define CORE_WAVELET_FILTER_MARGIN COM_ADC_POWER/30

static int TARGET_X_SIZE = 700;
static int TARGET_Y_SIZE = 700;
static int THE_TARGET_SIZE = 25;

static int TARGET_WIND_X_SIZE = 710;
static int TARGET_WIND_Y_SIZE = 710;

static int GRAPH_X_SIZE = 400;
static int GRAPH_Y_SIZE = 100;

#define GRAPH_SPEED 10
#define GRPAH_INTERVAL 500 // таймер перерисовки прицела в виджете
#define REDRAW_PERIOD 500 // таймер перерисовки графиков
#define CORE_GRAPHS_ALIASED false

#define EMULATE_TIMER 500
#define EMULATE_COUNT 256

#define _PI 3.14159265

#define OUT_DIR "D://"
#define TEXT_FILE_NAME "Результаты.txt"
#define IMAGE_FILE_NAME "Мишень.png"

#define COLOR_AFTER_SHOT_OUTSIDE Qt::red
#define COLOR_AFTER_SHOT_INSIDE Qt::yellow
#define COLOR_IN_SHOT Qt::green
#define COLOR_BEFORE_SHOT Qt::blue

//#define _EMULATE

