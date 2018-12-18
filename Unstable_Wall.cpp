#include "Unstable_Wall.h"
#include "MyPG.h"
#include "Task_Ball.h"

bool Unstable_Wall::Collision_Action(std::vector<Collision_Data>* result, Shape3D* ball) 
{
	//���C�t���c���Ă���Ƃ��̂ݏ���������
	if (this->life <= 0)
	{
		return false;
	}
	//�����蔻�茋�ʂ�result�ɕۑ�
	if (!this->collision_Base->Hit(result, ball))
	{
		//������Ȃ������������[���x�N�g�����U���g��pushback���ďI������
		result->push_back(Collision_Data());
		return false;
	}	
	//�V�������茋�ʂɂ�ID�����Ă���
	for (auto& r : *result)
	{
		if (r.collision_Id == "")
		{
			r.collision_Id = this->box_Id;
		}
	}
	//�{�[�����x�̃X�J���[�ʕ����C�t������	
	this->Decrease_Life(ge->GetTask_One_G<Ball::Object>("�{�[��")->Get_Speed().Length());
}

void Unstable_Wall::Decrease_Life(const float& dec)
{
	this->life -= dec;
}

Unstable_Wall::Unstable_Wall(const ML::Vec3& pos, const ML::Vec3& half_Of_Length, const ML::QT& qt, const string id)
	:Bbox(pos,half_Of_Length,qt,id)
{
	this->chip = Bbox::BoxType::Unstable_Wall;
	this->life = 18.0f;
}