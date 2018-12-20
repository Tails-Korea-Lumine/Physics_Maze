//-------------------------------------------------------------------------
//エフェクト基礎クラス
//-------------------------------------------------------------------------
#include "BEffect.h"
#include "easing.h"
#include "MyMath.h"

void BEffect::Dec_Eff()
{
	//残り時間減少
	this->effect_Life--;
}

bool BEffect::Is_Alive() const
{
	return this->effect_Life > 0;
}

ML::QT BEffect::Calc_Qt() const
{
	//回転軸計算
	ML::Vec3 yplus = ML::Vec3(0, 1, 0);
	ML::Vec3 anker;
	MyMath::Get_Normal_to_Vector_Cross(&anker, yplus, this->angle);
	//回転量計算
	float cos;
	MyMath::Vector_Dot(&cos, yplus, this->angle);
	//クォータニオンの生成とアフィン変換
	return ML::QT(anker, acos(cos));
}


BEffect::BEffect(const ML::Vec3& pos, const ML::Vec3& angle)
{
	this->pos = pos;
	this->angle = angle;
	
}
BEffect::BEffect(const ML::Vec3& pos, const ML::Vec3& target, const ML::Vec3& angle)
{
	this->pos = pos;
	this->target = target;
	this->angle = angle;	
}