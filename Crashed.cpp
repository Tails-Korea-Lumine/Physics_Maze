#include "Crashed.h"
#include "MyPG.h"

void Crashed::Effect_Update()
{
	if (this->effect_Life <= 0)
	{
		return;
	}

	//�j�Ђ����ɔ�тȂ���i�X�����ɂȂ�
	//�ړ����x�ƕ����𐶐�
	ML::Vec3 speed = ML::Vec3(0, -this->effect_Life, 0);
	//��]�N�H�[�^�j�I���𐶐�����
	ML::QT qt = this->Calc_Qt();
	//��]�s�񐶐�
	ML::Mat4x4 mat;
	mat.RotationQuaternion(qt);
	//���x�����̕���]����
	speed = mat.TransformCoord(speed);

	this->pos += speed;
	//�����x�͂͂�����Ȃ����u�Ԃ��瓧���ɂȂ��Ă���
	this->alpha = sinf(this->effect_Life);
}


Crashed::Crashed(const ML::Vec3& pos, const ML::Vec3& angle, const string& mesh_name, const string& sound_Name)
	:BEffect(pos,angle)
{
	this->meshName = mesh_name;
	this->sound_Name = sound_Name;
	this->effect_Life = 2.5f;
	this->alpha = 1.0f;
	this->scale = ML::Vec3(100, 100, 100);

	if (this->sound_Name != "")
	{
		DM::Sound_Play(this->sound_Name, false);
	}
}