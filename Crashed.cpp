#include "Crashed.h"
#include "MyPG.h"

void Crashed::Effect_Update()
{
	if (this->effect_Life <= 0)
	{
		return;
	}

	//破片が後ろに飛びながら段々透明になる
	//移動速度と方向を生成
	ML::Vec3 speed = ML::Vec3(0, -this->effect_Life, 0);
	//回転クォータニオンを生成する
	ML::QT qt = this->Calc_Qt();
	//回転行列生成
	ML::Mat4x4 mat;
	mat.RotationQuaternion(qt);
	//速度をその分回転する
	speed = mat.TransformCoord(speed);

	this->pos += speed;
	//透明度ははっきりなった瞬間から透明になっていく
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