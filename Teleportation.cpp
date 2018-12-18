#include "Teleportation.h"
#include "MyPG.h"
#include "Task_Ball.h"

bool Teleportation::Collision_Action(std::vector<Collision_Data>* result, Shape3D* ball) 
{	
	//�����蔻�茋�ʂ�result�ɕۑ�
	if (!this->collision_Base->Hit(result, ball))
	{
		//������Ȃ���������false��Ԃ�
		return false;
	}
	//�e���|�[�g�A�E�g�ꏊ
	ML::Vec3 exitpos;
	//�e���|�[�g���ł��Ȃ�����false��Ԃ�
	if (ge->TM.Find_Exit(this->side_Number, &exitpos))
	{
		//�{�[���^�X�N�Ƀe���|�[�g�̖��߂𑗂�
		ge->GetTask_One_G<Ball::Object>("�{�[��")->Teleportation(exitpos);

		auto eff = ge->eff_Manager.lock();
		//�e���|�[�g�C���̃G�t�F�N�g
		eff->Add_Effect(ball->Get_Center(), this->Get_Pos(), ML::Vec3(0, 0, 0), BEffect::effType::Teleportin);		
		//�e���|�[�g�A�E�g�̃G�t�F�N�g
		eff->Add_Effect(exitpos, exitpos - this->Get_Pos(), BEffect::effType::TeleportOut);
		return true;
	}
	return false;
}

void Teleportation::Rotate_Box(ML::Mat4x4* mat, const ML::QT& q)
{
	//��]�ʂƈʒu���X�V
	this->collision_Base->Rotation(mat, q);
	//�e���|�[�g���̈ʒu�X�V
	ge->TM.Update_Door_Position(this->side_Number, this->Get_Pos());
}

Teleportation::Teleportation(const ML::Vec3& pos, const ML::Vec3& half_Of_Length, const ML::QT& qt, const string id, const int& side_Number)
	:Bbox(pos, half_Of_Length, qt, id)
{
	this->chip = Bbox::BoxType::Teleportation;
	this->side_Number = side_Number;
	ge->TM.Update_Door_Position(this->side_Number, pos);
}