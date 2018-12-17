#include "Game_Clear.h"
#include "MyPG.h"

void Game_Clear::Effect_Update()
{	
	if (this->effect_Life >= 20)
	{
		//�c��20�t���[���܂ŉ�]���Ȃ���傫���Ȃ�
		this->scale += ML::Vec3(8.0f, 8.0f, 8.0f);
		this->angle.y += ML::ToRadian(3.0f);
		this->alpha += 0.8f;
	}	
	else if (this->effect_Life < 20)
	{
		//���̌��C�ɏ������Ȃ�
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

	//���C�e�B���O�𖳌��ɂ���
	DG::EffectState().param.lightsEnable = false;
	//�����x�ݒ�
	auto bsBuf = DG::EffectState().BS_Get();
	DG::EffectState().BS_Alpha();
	DG::EffectState().param.objectColor = ML::Color(this->alpha, 1, 1, 1);
	DG::EffectState().param.matWorld = matS * matR * matT;
	DG::Mesh_Draw(this->meshName);

	//�����x,���C�e�B���O�����ɖ߂�
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
	//�T�E���h�G�t�F�N�g�Đ�
	if (this->sound_Name != "")
	{
		DM::Sound_Play(this->sound_Name, false);
	}
}
