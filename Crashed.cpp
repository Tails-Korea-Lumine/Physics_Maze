#include "Crashed.h"
#include "MyPG.h"

void Crashed::Effect_Update()
{
	if (this->effect_Life <= 0)
	{
		return;
	}
	//�ړ��ʂ͎c��������e������
	float speed = this->effect_Life / 10.0f;

	//�j�Ђ����ɔ�тȂ���i�X�����ɂȂ�
	this->pos.y -= speed;
	this->alpha -= 0.005f;
}

void Crashed::Effect_Draw() const
{
	if (this->effect_Life <= 0)
	{
		return;
	}
	//�`��p�s�񐶐�
	ML::Mat4x4 matS, matRT;
	matS.Scaling(this->scale);
	//��]�N�H�[�^�j�I��
	ML::QT qt = this->Calc_Qt();
	D3DXMatrixAffineTransformation(&matRT, 1.0f, NULL, &qt, &this->pos);

	//�����x��ݒ�
	auto bsBuf = DG::EffectState().BS_Get();
	//DG::EffectState().BS_Alpha();
	//�G�t�F�N�g���������x�ŕ`��
	DG::EffectState().param.objectColor = ML::Color(this->alpha, 1, 1, 1);
	DG::EffectState().param.matWorld = matS * matRT;
	DG::Mesh_Draw(this->meshName);

	//�����x�����ɖ߂�
	DG::EffectState().BS_Set(bsBuf);
	DG::EffectState().param.objectColor = ML::Color(1, 1, 1, 1);
}


Crashed::Crashed(const ML::Vec3& pos, const ML::Vec3& angle, const string& mesh_name, const string& sound_Name)
	:BEffect(pos,angle)
{
	this->meshName = mesh_name;
	this->sound_Name = sound_Name;
	this->effect_Life = 150;
	this->alpha = 1.0f;
	this->scale = ML::Vec3(100, 100, 100);

	if (this->sound_Name != "")
	{
		DM::Sound_Play(this->sound_Name, false);
	}
}