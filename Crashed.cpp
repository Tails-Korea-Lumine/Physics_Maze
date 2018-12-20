#include "Crashed.h"
#include "MyPG.h"

void Crashed::Effect_Update()
{
	if (this->effect_Life <= 0)
	{
		return;
	}
	//ˆÚ“®—Ê‚ÍŽc‚èŽõ–½‚ª‰e‹¿‚·‚é
	float speed = this->effect_Life / 10.0f;

	//”j•Ð‚ªŒã‚ë‚É”ò‚Ñ‚È‚ª‚ç’iX“§–¾‚É‚È‚é
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