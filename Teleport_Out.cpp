#include "Teleport_Out.h"
#include "MyPG.h"
#include "Dm2008_Ver1_3.h"


void Teleport_Out::Effect_Update()
{
	//フレーム毎薄く、大きくなる
	this->alpha -= 0.008f;

	this->scale.y += 100.0f;

	//透明度は0以下に落ちないようにする
	this->alpha = max(this->alpha, 0.0f);
}

void Teleport_Out::Effect_Draw() const
{
	if (this->effect_Life <= 0)
	{
		return;
	}
	ML::Mat4x4 matS;
	matS.Scaling(this->scale);
	ML::Mat4x4 matT;
	matT.Translation(this->pos);

	//透明度を設定
	auto bsBuf = DG::EffectState().BS_Get();
	DG::EffectState().BS_Alpha();
	//エフェクトが持つ透明度で描画
	DG::EffectState().param.objectColor = ML::Color(this->alpha, 1, 1, 1);
	DG::EffectState().param.matWorld = matS * matT;
	DG::Mesh_Draw(this->meshName);

	//透明度を元に戻す
	DG::EffectState().BS_Set(bsBuf);
	DG::EffectState().param.objectColor = ML::Color(1, 1, 1, 1);	
}

Teleport_Out::Teleport_Out(const ML::Vec3& pos, const ML::Vec3& angle, const string& mesh_name, const string& sound_Name)
	:BEffect(pos,angle)
{
	this->meshName = mesh_name;
	this->sound_Name = sound_Name;	
	this->alpha = 0.8f;
	this->effect_Life = 80;
	this->scale = ML::Vec3(200, 100, 200);

	//サウンドエフェクト再生
	if (this->sound_Name != "")
	{
		DM::Sound_Play(this->sound_Name, false);
	}
}


