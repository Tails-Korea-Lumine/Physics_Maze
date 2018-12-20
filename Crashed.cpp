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