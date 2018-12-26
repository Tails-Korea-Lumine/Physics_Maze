#include "Teleport_In.h"
#include "MyPG.h"

void Teleport_In::Effect_Update()
{
	this->moveVec = this->target - this->pos;//進行方向をもらう
											 //移動
	this->pos += (this->moveVec * 0.25f);

	this->alpha -= 0.01f;
	//透明度は0以下に落ちないようにする
	this->alpha = max(this->alpha, 0.0f);
}

void Teleport_In::Effect_Draw() const
{
	if (this->effect_Life <= 0)
	{
		return;
	}
	//行列の生成
	ML::Mat4x4 matT, matS;
	matT.Translation(this->pos);
	matS.Scaling(this->scale);

	//透明度設定(光なので加算合成)
	auto bsBuf = DG::EffectState().BS_Get();
	DG::EffectState().BS_AlphaAdd();
	DG::EffectState().param.objectColor = ML::Color(this->alpha, 1, 1, 1);
	//描画処理
	DG::EffectState().param.matWorld = matS * matT;
	DG::Mesh_Draw(this->meshName);

	//透明度を元に戻す
	DG::EffectState().BS_Set(bsBuf);
	DG::EffectState().param.objectColor = ML::Color(1, 1, 1, 1);
}


Teleport_In::Teleport_In(const ML::Vec3& pos, const ML::Vec3& target, const ML::Vec3& angle, const string& mesh_name, const string& sound_Name)
	:BEffect(pos,target,angle)
{
	this->meshName = mesh_name;
	this->sound_Name = sound_Name;
	this->alpha = 1.0f;
	this->effect_Life = 0.3f;
	this->scale = ML::Vec3(80.0f, 80.0f, 80.0f);
	this->pos.y += 100.0f;

	//サウンドエフェクト再生
	if (this->sound_Name != "")
	{
		DM::Sound_Play(this->sound_Name, false);
	}
}