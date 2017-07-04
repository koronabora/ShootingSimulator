#include "Practice.h"

Practice::Practice()
{}

Practice::~Practice()
{}

void Practice::newPosition(quint64 targetId, qreal x, qreal y, quint64 time)
{

	lastUpdTime = time;
	bool move = false;
	if (id == targetId)
	{
		if ((state == STATES::AFTER && currentShotCount == practiceShotCount)||(currentShotCount < practiceShotCount))
		{
			if (state != STATES::NC && currentShot != Q_NULLPTR)
			{
				x = transX(x, id);
				y = transY(y, id);

				//if (abs(abs(x) - abs(y)) <= TARGET_X_SIZE)
				{
					if (firstTime || (lastX == 0 && lastY == 0))
					{
						firstTime = false;
						lastX = x;
						lastY = y;
						currentShot->startTime = time;
					}
					else if (x != lastX || y != lastY)
					{
						if (currentShot->id->size() > 0 && currentShot->time->first() < (time - MAX_AIMING_TIME) && state == STATES::BEFORE)
						{
							currentShot->x->append(x);
							currentShot->x->pop_front();
							currentShot->y->append(y);
							currentShot->y->pop_front();
							currentShot->time->append(time);
							currentShot->time->pop_front();
							currentShot->id->append(currentShot->id->takeAt(0));
							move = true;
						}
						else
						{
							currentShot->x->append(x);
							currentShot->y->append(y);
							currentShot->time->append(time);
							currentShot->id->append(itemId++);
						}

						quint64 col = 0;

						qreal val = sqrt(x*x + y*y); // длина вектора
						switch (state)
						{
						case STATES::BEFORE:
							// вычисляем разброс по горизонтали
							if (x > currentShot->maxValin1)
								currentShot->maxValin1 = x;
							else if (x < currentShot->minValin1)
								currentShot->minValin1 = x;

							// вычисляем разброс по вертикали
							if (y > currentShot->maxValin2)
								currentShot->maxValin2 = y;
							else if (y < currentShot->minValin2)
								currentShot->minValin2 = y;
							col = COLORS::BEFORE_SHOT;
							break;
						case STATES::INS:
							// вычисляем разброс по горизонтали
							if (x > currentShot->maxValin1)
								currentShot->maxValin1 = x;
							else if (x < currentShot->minValin1)
								currentShot->minValin1 = x;

							// вычисляем разброс по вертикали
							if (y > currentShot->maxValin2)
								currentShot->maxValin2 = y;
							else if (y < currentShot->minValin2)
								currentShot->minValin2 = y;

							// считаем разницу по времени выстрела
							if (currentShot->bX == 0 && currentShot->bY == 0)
							{
								currentShot->bX = x;
								currentShot->bY = y;
							}
							else
							{
								currentShot->eX = x;
								currentShot->eY = y;
							}
							col = COLORS::IN_TIME_OF_SHOT;
							bufX.append(x);
							bufY.append(y);
							bufId.append(itemId++);
							break;
						case STATES::AFTER:
							// вычисляем крен - максимальный угол первые 100 мс относительно выстрела
							if (time - currentShot->shotTime <= KREN_PERIOD)
							{
								//qreal ppp = posToGrad(x, y)-posToGrad(bufX.first(),bufY.first());
								//if (currentShot->bX != 0)
								//{
								//	qreal ppp = abs(x - currentShot->bX);
									//qDebug() << ppp;
								//	if (currentShot->maxValin3 < ppp)
								//		currentShot->maxValin3 = ppp;
								//}
								if (x > currentShot->maxValin3)
									currentShot->maxValin3 = x;
								else if (x < currentShot->minValin3)
									currentShot->minValin3 = x;

							}

							if (val > TARGET_X_SIZE / 2)
								col = COLORS::AFTER_SHOT_OUTSIDE;
							else
								col = COLORS::AFTER_SHOT_INSIDE;
							break;
						}

						if (move)
							emit moveDot(id, col, lastX, lastY, x, y, currentShot->id->last());
						else
							emit paintDot(id, col, lastX, lastY, x, y, itemId - 1);
						lastX = x;
						lastY = y;
					}
				}
			}
		}
	}
}

void Practice::newShotBegin(quint64 targetId, quint64 time)
{
	if (id == targetId)
	{
		if (state!=STATES::NC && currentShotCount < practiceShotCount)
		{
			if (currentShotCount+1 == practiceShotCount && (PORT_TYPES.at(targetId) == WEAPON_TYPES::AK || PORT_TYPES.at(targetId) == WEAPON_TYPES::PM)) // отключаем отдачу
				emit changeBoomState(targetId, false);
			
			qDebug() << "Shots begin: " << currentShotCount;
			if (afterTimer->isActive())
			{
				afterTimer->stop();
				rotateShots();
			}
			inShot = true;
			state = STATES::INS;
			currentShot->startTime = time;
		}
		//else if(PORT_TYPES.at(targetId) == WEAPON_TYPES::AK || PORT_TYPES.at(targetId) == WEAPON_TYPES::PM) // отключаем отдачу
			//emit changeBoomState(targetId, false);
	}
}

void Practice::newShotEnd(quint64 targetId, quint64 time)
{
	if (id == targetId)
	{

		if (currentShotCount < practiceShotCount)
		{
			qDebug() << "Shots begin: " << currentShotCount;
			inShot = false;
			//shotRepTimer->stop();
			state = STATES::AFTER;
#ifdef TEST_MODE
			emit logTo(id, QTime::currentTime().toString("hh:mm:ss - ") + QString(SE_MESSAGE));
#else
			qDebug() << "Shot ends: id = " << id << ", time = " << time;
#endif
			if (bufX.size() > 0 && bufY.size() > 0 && bufId.size() > 0)
			{
				shotX.append(bufX.at(bufX.size() / 2));
				shotY.append(bufY.at(bufY.size() / 2));

				emit playRifle(targetId);

				emit paintDot(targetId, COLORS::SHOT, shotX.last(), shotY.last(), shotX.last(), shotY.last(), itemId++);

				currentShot->shotTime = currentShot->startTime+(time - currentShot->startTime)/2;
				currentShot->endTime = time;
				qreal ttt1 = currentShot->bX;
				qreal ttt2 = currentShot->bY;
				qreal ttt3 = currentShot->eX;
				qreal ttt4 = currentShot->eY;

				// Дисперсия при нажатии на курок - концентрация
				currentShot->res.dispAll = sqrt((ttt1-ttt3)*(ttt1 - ttt3)+(ttt2-ttt4)*(ttt2 - ttt4)); // vector ((ttt1,ttt2);(ttt3,ttt4))
				
				// Максимальные горизонатльные отклонения																									  
				currentShot->res.dispHor = currentShot->maxValin1 - currentShot->minValin1;
				
				// Максимальные вертикальные отклонения
				currentShot->res.dispVert = currentShot->maxValin2-currentShot->minValin2;
				// Крен
				currentShot->res.kren = currentShot->maxValin3 - currentShot->minValin3;

				qDebug() << "Disp = "<<currentShot->res.dispAll << " Disp hor = " << currentShot->res.dispHor << "Disp vert = "<< currentShot->res.dispVert;

				// проигрывание звука
				qreal angle;
				qreal xx = shotX.last();
				qreal yy = -shotY.last();
				qreal val = sqrt(xx*xx + yy*yy); // длина вектора				
				

				angle = posToGrad(xx, yy);
				//qDebug() << "Angle " << angle;
				//qDebug() << "Hour" << angle / (360 / 12);
				quint64 result = 0;
				if (val < TARGET_X_SIZE / 2.0)
				{
					result = 10-trunc((val )/ ((TARGET_X_SIZE / 2.0)/5.0));
				}
				qDebug() << "Result: " << result;
	
				currentShot->res.res = result;
				// и
				QString f1;
				if (result>=5)
					f1 = ":/Sound/" + QString::number(result) + ".wav";
				else
					f1 = ":/Sound/0.wav";
				QString f2;
				if (trunc(angle / (360 / 12))>0)
					f2 = ":/Sound/hour" + QString::number(trunc(angle / (360 / 12))) + ".wav";
				else
					f2 = ":/Sound/hour12.wav";

				emit playResults(targetId, f1, f2);

			}
			else
			{
				qDebug() << "Could not process shot!";
			}
			bufX.clear();
			bufId.clear();
			bufY.clear();
			//state = STATES::AFTER;
			afterTimer->setSingleShot(true);
			afterTimer->start(AFTER_SHOT_TIME);
			currentShotCount++;
		}
		//else
		//{
		//	if (PORT_TYPES.at(targetId) == WEAPON_TYPES::AK || PORT_TYPES.at(targetId) == WEAPON_TYPES::PM) // отключаем отдачу
		//		emit changeBoomState(targetId, false);
		//}
	}
}

void Practice::init(quint64 targetId)
{
	lastX = 0;
	lastY = 0;
	id = targetId;
	firstTime = true;
	connected = false;
	lastUpdTime = QTime::currentTime().msecsSinceStartOfDay();
	state = STATES::NC;
	inShot = false;
	shotX.clear();
	shotY.clear();
	for (quint64 i = 0; i < KE_IN_PORTS; i++)
		mults.append(3.0);
}

void Practice::run()
{
	state = STATES::NC;
	afterTimer = new QTimer();
	connect(afterTimer, &QTimer::timeout, this, &Practice::timer2);

	//p = new QMediaPlayer();
	//pl = new QMediaPlaylist();
	//p->setPlaylist(pl);
}

void Practice::targetOnline(quint64 targetId)
{
	if (targetId == id)
	{
		if (!connected)
		{
			//qDebug() << "Target connected to practice #" << id << ", Targetid = " << targetId;
#ifdef TEST_MODE
			emit logTo(id, QString("Target connected to practice #") + QString::number(id) + QString(", Targetid = ") + QString::number(targetId));
#else
			qDebug() << "Target connected to practice #" << id << ", Targetid = " << targetId;
#endif
			connected = true;
			emit targetConnected(id);
		}
		else
			qDebug() << "Reconnecting target without disconnection! id = " << id;
	}
}

void Practice::targetOffline(quint64 targetId)
{
	if (targetId == id)
	{
		if (connected)
		{
#ifdef TEST_MODE
			emit logTo(id, QString("Target disconnected from practice #") + QString::number(id) + QString(", Targetid = ") + QString::number(targetId));
#else
			qDebug() << "Target disconnected, id = " << id;
#endif
			connected = false;
			emit targetDisconnected(id);
		}
		else
			qDebug() << "Reconnecting target without disconnection! id = " << id;
	}
}

void Practice::newPracticeSelected(quint64 index, quint64 shotCount)
{
	if (index == id)
	{
		shotX.clear();
		shotY.clear();
		itemId = 0;
		lastX = 0;
		lastY = 0;
		state = STATES::BEFORE;
		//lastUpdTime = QTime::currentTime().msecsSinceStartOfDay();
		emit newPractice(index, shotCount);
		currentShotCount = 0;
		practiceShotCount = shotCount;
		if (PORT_TYPES.at(id) == WEAPON_TYPES::AK || PORT_TYPES.at(id) == WEAPON_TYPES::PM) // можем делать очереди
			emit changeBoomState(id, true);
		rotateShots();
	}
}

/*void Practice::timer1() // закончили запись инфы
{
	state = STATES::AFTER;
	afterTimer->setSingleShot(true);
	afterTimer->start(AFTER_SHOT_TIME);
}*/

void Practice::timer2()
{
	// Записываем данные выстрела
	//firstTime = true;
	lastUpdTime = QTime::currentTime().msecsSinceStartOfDay();
	if (currentShotCount < practiceShotCount)
		rotateShots();
	else
	{
		// закончили упражнение
		currentShot->endTime = lastUpdTime;
		shots.append(currentShot);
		guiShotData* res = new guiShotData();
		fillRes(res);
		emit practiceEnded(id, res);
	}
}

void Practice::rotateShots()
{
	state = STATES::BEFORE;
	lastUpdTime = QTime::currentTime().msecsSinceStartOfDay();
	// сохраняем текущий выстрел
	if (!firstTime)
		if (currentShot != Q_NULLPTR)
		{
			currentShot->endTime = lastUpdTime;
			shots.append(currentShot);
			calcResults();
			for (quint64 i=0; i<shotX.size(); i++)
				emit paintDot(id, COLORS::OTHER_SHOTS, shotX.at(i), shotY.at(i), shotX.at(i), shotY.at(i), itemId++);
		}
	if (currentShotCount < practiceShotCount)
	{
		if (currentShotCount > 0)
			qDebug() << "New shot: " << currentShotCount + 1 << " of " << practiceShotCount;
		// заводим новый выстрел
		currentShot = new shot();
		currentShot->sceneId = id;
		currentShot->startTime = lastUpdTime;

		currentShot->x = new QVector<qreal>();
		currentShot->y = new QVector<qreal>();
		currentShot->time = new QVector<quint64>();
		currentShot->id = new QVector<quint64>();

		state = STATES::BEFORE;

		lastX = 0;
		lastY = 0;
	}
	else
		state = STATES::NC;
}

void Practice::calcResults()
{
	if (currentShot != Q_NULLPTR)
	{
		currentShot->res.dispAll;
		// закончили выстрел
		
		guiShotData* res = new guiShotData();
		fillRes(res);
		emit nextShot(id, res);
		currentShot->bX = 0;
		currentShot->bY = 0;
	}
}

void Practice::fillRes(guiShotData* res)
{
	res->result = currentShot->res.res;
	res->averageResult = 0;
	quint64 mol = 0;
	quint64 sum = 0;
	for (int i = 0; i < shots.size(); i++)
	{
		if (shots.at(i)->res.res == 0)
			mol++;
		res->averageResult += shots.at(i)->res.res;
		res->res.append(shots.at(i)->res.res);
	}
	sum = res->averageResult;
	//res->averageResult /= shots.size();
	res->shotTime = (currentShot->shotTime - currentShot->startTime);
	res->shotNum = shots.size();
	quint64 k1 = 1; // координация все по 1\8 от ширины мишени
	quint64 k2 = 1; // устойчивость по вертикали
	quint64 k3 = 1; // устойчивость по диагонали
	qreal v = (TARGET_X_SIZE / MARK_DIVIDER);

	//if (currentShot->res.kren > v/ ANGLE_DIVIDER)
	//	res->shotKren = 0;
	//else
//		res->shotKren = 4 - round(currentShot->res.kren / ANGLE_DIVIDER);
	res->shotKren = 1;
	if (currentShot->res.kren < v / ANGLE_DIVIDER)
		res->shotKren = 4;
	else if (currentShot->res.kren < v / ANGLE_DIVIDER * 2)
		res->shotKren = 3;
	else if (currentShot->res.kren < v / ANGLE_DIVIDER * 3)
		res->shotKren = 2;

	if (currentShot->res.dispAll < v)
		k1 = 4;
	else if (currentShot->res.dispAll < v * 2)
		k1 = 3;
	else if (currentShot->res.dispAll < v * 3)
		k1 = 2;

	if (currentShot->res.dispVert < v)
		k2 = 4;
	else if (currentShot->res.dispVert < v * 2)
		k2 = 3;
	else if (currentShot->res.dispVert < v * 3)
		k2 = 2;

	if (currentShot->res.dispHor < v)
		k3 = 4;
	else if (currentShot->res.dispHor < v * 2)
		k3 = 3;
	else if (currentShot->res.dispHor < v * 3)
		k3 = 2;

	res->shotTraectory = (k1 + k2 + k3) / 3;
	if (res->shotTraectory == 4)
		res->recomendations.append(tr(MARK_MESS_NO));
	else
	{
		res->recomendations.append(tr(MARK_MESS));
		if (k1 < 4)
		{
			res->recomendations.append(tr(MM_1));
		}
		if (k2 < 4)
		{
			res->recomendations.append(tr(MM_2));
		}
		if (k3 < 4)
		{
			res->recomendations.append(tr(MM_3));
		}
	}

	res->shotInfo.append(tr("Координация: ") + QString::number(k1));
	res->shotInfo.append(tr("Устойчивость по вер.: ") + QString::number(k2));
	res->shotInfo.append(tr("Устойчивость по гор.: ") + QString::number(k3));
	res->shotInfo.append(tr("Крен: ") + QString::number(res->shotKren));

	res->avKren *= shots.size() - 1;
	res->avKren += res->shotKren;
	res->avKren /= shots.size()*1.0;

	res->avTraectory *= shots.size() - 1;
	res->avTraectory += res->shotTraectory;
	res->avTraectory /= shots.size()*1.0;


	res->allShotsInfo.append(tr("Количество выстрелов: ") + QString::number(shots.size()));
	res->allShotsInfo.append(tr("Количество очков: ") + QString::number(sum));
	res->allShotsInfo.append(tr("Количество промахов: ") + QString::number(mol));
	res->allShotsInfo.append(tr("Устойчивость траектории: ") + QString::number(res->avTraectory));
	res->allShotsInfo.append(tr("Устойчивость к крену: ") + QString::number(res->avKren));
}

qreal Practice::transX(qreal x, quint64 index)
{
	qreal res = x*COM_ADC_POWER*mults.at(index);
	//qDebug() << " x1 = " << x;
	return res;
}

qreal Practice::transY(qreal y, quint64 index)
{
	qreal res = y*COM_ADC_POWER*mults.at(index);
	//qDebug() << " y1 = " << y;
	return res;
}

void Practice::setMult(QVector<qreal> mult)
{
	mults.clear();
	for (quint64 i=0; i<mult.size(); i++)
		mults.append(mult.at(i));
}

qreal Practice::posToGrad(qreal x, qreal y)
{
	qreal res = 0;
	qreal val = sqrt(x*x + y*y);
	res = acos(y/val)*180.0/_PI;
	if (x < 0)
		res = 180 + (180 - res);
	return res;
}