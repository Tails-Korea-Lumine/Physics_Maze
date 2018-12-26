#include "Teleport_In.h"
#include "MyPG.h"

void Teleport_In::Effect_Update()
{
	this->moveVec = this->target - this->pos;//�i�s���������炤
											 //�ړ�
	this->pos += (this->moveVec * 0.25f);

	this->alpha -= 0.01f;
	//�����x��0�ȉ��ɗ����Ȃ��悤�ɂ���
	this->alpha = max(this->alpha, 0.0f);
}

void Teleport_In::Effect_Draw() const
{
	if (this->effect_Life <= 0)
	{
		return;
	}
	//�s��̐���
	ML::Mat4x4 matT, matS;
	matT.Translation(this->pos);
	matS.Scaling(this->scale);

	//�����x�ݒ�(���Ȃ̂ŉ��Z����)
	auto bsBuf = DG::EffectState().BS_Get();
	DG::EffectState().BS_AlphaAdd();
	DG::EffectState().param.objectColor = ML::Color(this->alpha, 1, 1, 1);
	//�`�揈��
	DG::EffectState().param.matWorld = matS * matT;
	DG::Mesh_Draw(this->meshName);

	//�����x�����ɖ߂�
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

	//�T�E���h�G�t�F�N�g�Đ�
	if (this->sound_Name != "")
	{
		DM::Sound_Play(this->sound_Name, false);
	}
}