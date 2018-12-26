#include "Breaking.h"
#include "MyPG.h"


void Breaking::Effect_Update()
{
	if (this->effect_Life <= 0)
	{
		return;
	}

	//XZ方向に100まで大きくなる
	this->scale.x += 20.0f;
	this->scale.z += 20.0f;
	this->scale.x = min(this->scale.x, 100.0f);
	this->scale.z = min(this->scale.z, 100.0f);

	//移動速度と方向を生成
	ML::Vec3 speed = ML::Vec3(0.0f, sinf(this->effect_Life) * 17.0f, 0.0f);
	//回転クォータニオンを生成する
	ML::QT qt = this->Calc_Qt();
	//回転行列生成
	ML::Mat4x4 mat;
	mat.RotationQuaternion(qt);
	//速度をその分回転する
	speed = mat.TransformCoord(speed);

	this->pos += speed;
	//透明度ははっきりなった瞬間から透明になっていく
	this->alpha = sinf(this->effect_Life);
}

//void Breaking::Effect_Draw() const
//{
//	if (this->effect_Life <= 0)
//	{
//		return;
//	}
//
//	//描画用行列生成
//	ML::Mat4x4 matS, matRT;
//	matS.Scaling(this->scale);
//	//回転クォータニオン
//	ML::QT qt = this->Calc_Qt();
//	D3DXMatrixAffineTransformation(&matRT, 1.0f, NULL, &qt, &this->pos);
//
//	//透明度を設定
//	auto bsBuf = DG::EffectState().BS_Get();
//	DG::EffectState().BS_Alpha();
//	//エフェクトが持つ透明度で描画
//	DG::EffectState().param.objectColor = ML::Color(this->alpha, 1, 1, 1);
//	DG::EffectState().param.matWorld = matS * matRT;
//	DG::Mesh_Draw(this->meshName);
//
//	//透明度を元に戻す
//	DG::EffectState().BS_Set(bsBuf);
//	DG::EffectState().param.objectColor = ML::Color(1, 1, 1, 1);
//}

Breaking::Breaking(const ML::Vec3& pos, const ML::Vec3& angle, const string& mesh_name, const string& sound_Name)
	:BEffect(pos, angle)
{
	this->meshName = mesh_name;
	this->sound_Name = sound_Name;
	this->alpha = 1.0f;
	this->effect_Life = 0.5f;
	this->scale = ML::Vec3(50, 50, 50);

	if (this->sound_Name != "")
	{
		DM::Sound_Play(this->sound_Name, false);
	}
}