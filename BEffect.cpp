//-------------------------------------------------------------------------
//エフェクト基礎クラス
//-------------------------------------------------------------------------
#include "BEffect.h"
#include "easing.h"
#include "MyMath.h"
#include "MyPG.h"


void BEffect::Effect_Draw()const
{
	if (this->effect_Life <= 0)
	{
		return;
	}
	//描画用行列生成
	ML::Mat4x4 matS, matRT;
	matS.Scaling(this->scale);
	//回転クォータニオン
	ML::QT qt = this->Calc_Qt();
	D3DXMatrixAffineTransformation(&matRT, 1.0f, NULL, &qt, &this->pos);

	//透明度を設定
	auto bsBuf = DG::EffectState().BS_Get();
	DG::EffectState().BS_Alpha();
	//エフェクトが持つ透明度で描画
	DG::EffectState().param.objectColor = ML::Color(this->alpha, 1, 1, 1);
	DG::EffectState().param.matWorld = matS * matRT;
	DG::Mesh_Draw(this->meshName);

	//透明度を元に戻す
	DG::EffectState().BS_Set(bsBuf);
	DG::EffectState().param.objectColor = ML::Color(1, 1, 1, 1);
}

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