#include "Light_Switch.h"
#include "MyPG.h"
#include "Task_CameraMan.h"

bool Light_Switch::Collision_Action(std::vector<Collision_Data>* result, Shape3D* ball) const
{	
	//�����蔻�茋�ʂ�result�ɕۑ�
	if (!this->collision_Base->Hit(result, ball, this->unusable_Triagle))
	{
		//������Ȃ���������false��Ԃ�
		return false;
	}
	//�J�����}���Ƀ��C�g��3�b�ԃI�t���閽�߂𑗂�
	ge->GetTask_One_G<CameraMan::Object>("�J�����}��")->Turnoff_the_Light();
	//����͓��������̂�true��Ԃ�
	return true;
}

Light_Switch::Light_Switch(const ML::Vec3& pos, const ML::Vec3& half_Of_Length, const ML::QT& qt, const string id)
	:Bbox(pos, half_Of_Length, qt, id)
{
	this->chip = Bbox::BoxType::LightSwitch;	
}