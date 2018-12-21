#include "Crashed.h"
#include "MyPG.h"

void Crashed::Effect_Update()
{
	if (this->effect_Life <= 0)
	{
		return;
	}
	//移動量は残り寿命が影響する
	float speed = this->effect_Life / 30.0f;

	//破片が後ろに飛びながら段々透明になる
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