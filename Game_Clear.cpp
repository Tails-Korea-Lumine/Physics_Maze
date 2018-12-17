#include "Game_Clear.h"
#include "MyPG.h"

void Game_Clear::Effect_Update()
{	
	if (this->effect_Life >= 20)
	{
		//残り20フレームまで回転しながら大きくなる
		this->scale += ML::Vec3(8.0f, 8.0f, 8.0f);
		this->angle.y += ML::ToRadian(3.0f);
		this->alpha += 0.8f;
	}	
	else if (this->effect_Life < 20)
	{
		//その後一気に小さくなる
		this->scale *= sinf(ML::ToRadian((float)this->effect_Life));
	}
}

void Game_Clear::Effect_Draw() const
{
	if (this->effect_Life < 0)
	{
		return;
	}
	ML::Mat4x4 matS, matR, matT;
	matS.Scaling(this->scale);
	matR.RotationY(this->angle.y);
	matT.Translation(this->pos);

	//ライティングを無効にする
	DG::EffectState().param.lightsEnable = false;
	//透明度設定
	auto bsBuf = DG::EffectState().BS_Get();
	DG::EffectState().BS_Alpha();
	DG::EffectState().param.objectColor = ML::Color(this->alpha, 1, 1, 1);
	DG::EffectState().param.matWorld = matS * matR * matT;
	DG::Mesh_Draw(this->meshName);

	//透明度,ライティングを元に戻す
	DG::EffectState().param.lightsEnable = true;
	DG::EffectState().BS_Set(bsBuf);
	DG::EffectState().param.objectColor = ML::Color(1, 1, 1, 1);
}


Game_Clear::Game_Clear(const ML::Vec3& pos, const ML::Vec3& angle, const string& mesh_name, const string& sound_Name)
	:BEffect(pos,angle)
{
	this->sound_Name = sound_Name;
	this->meshName = mesh_name;
	this->alpha = 0.0f;
	this->effect_Life = 60;
	this->scale = ML::Vec3(0.0f, 0.0f, 0.0f);
	//サウンドエフェクト再生
	if (this->sound_Name != "")
	{
		DM::Sound_Play(this->sound_Name, false);
	}
}
