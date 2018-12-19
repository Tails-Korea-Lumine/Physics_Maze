#include "Wall.h"


bool Wall::Collision_Action(std::vector<Collision_Data>* result, Shape3D* ball) 
{	
	//�����蔻�茋�ʂ�result�ɕۑ�
	if (!this->collision_Base->Hit(result, ball))
	{
		//������Ȃ������������[���x�N�g�����U���g��pushback����
		result->push_back(Collision_Data());
	}
	//�V�������茋�ʂɂ�ID�����Ă���
	for (auto& r : *result)
	{
		if (r.collision_Id == "")
		{
			r.collision_Id = this->box_Id;
		}
	}
	//0�Ԃ̃t���O��߂�l�ŕԂ�
	return result->at(0).collision_Flag;
}

Wall::Wall(const ML::Vec3& pos, const ML::Vec3& half_Of_Length, const ML::QT& qt, const string& id, const string& mesh_Name)
	:Bbox(pos,half_Of_Length,qt,id,mesh_Name)
{
	this->chip = Bbox::BoxType::Wall;
}