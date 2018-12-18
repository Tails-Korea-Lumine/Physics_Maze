#include "Goal.h"
#include "MyPG.h"

bool Goal::Collision_Action(std::vector<Collision_Data>* result, Shape3D* ball) 
{	
	//�����蔻�茋�ʂ�result�ɕۑ�
	if (!this->collision_Base->Hit(result, ball))
	{
		//������Ȃ���������false��Ԃ�
		return false;
	}

	//�Q�[���^�X�N�ɃN���A������������
	ge->game.lock()->Game_Clear();
	return true;
}

Goal::Goal(const ML::Vec3& pos, const ML::Vec3& half_Of_Length, const ML::QT& qt, const string id)
	:Bbox(pos, half_Of_Length, qt, id)
{
	this->chip = Bbox::BoxType::Goal;
}