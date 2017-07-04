#include "TKalman.h"
//---------------------------------------------------------------------------
void TKalman::KalmanInit(qreal q, qreal r, qreal f, int intial_value)
{
	this->q = q;
	this->r = r;
	this->p = p;
	this->x = intial_value;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void TKalman::KalmanUpdate(int data)
{
	//prediction update
	//omit x = x
	p = p + q;

	//measurement update
	k = p / (p + r);
	x = x + k * (data - x);
	p = (1 - k) * p;
}
//---------------------------------------------------------------------------
int TKalman::GetResult(int data)
{
	KalmanUpdate(data);
	return x;
}
//---------------------------------------------------------------------------
TKalman::TKalman()
{

}
//---------------------------------------------------------------------------
TKalman::~TKalman()
{

}