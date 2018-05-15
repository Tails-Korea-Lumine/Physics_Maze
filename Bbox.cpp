#include "Bbox.h"


void Bbox::Rotate_Box(ML::Vec3 pos, ML::QT q)
{
	this->pos = pos;
	this->boxQT = q;
}

void Bbox::Get_Collision_Poligon(std::vector<After_Collision>* result, const ML::Vec3& pos, const float& r, const ML::Vec3& speed)
{
	//������Ȃ������������[���x�N�g�����U���g��pushback����
	if (!this->col.Hit_Check(result, this->collision_Base.OffsetCopy(this->pos), pos, r, speed, this->boxQT))
	{
		result->push_back(After_Collision());
	}
}

BoxType Bbox::What_Type_Is_this_Box()
{
	return this->chip;
}

ML::Vec3 Bbox::Get_Pos()
{
	return this->pos;
}

Bbox::Bbox()
{
	this->chip = BoxType::Clear;
	this->pos = ML::Vec3(0, 0, 0);
	this->collision_Base = ML::Box3D(0, 0, 0, 0, 0, 0);
	this->boxQT = ML::QT(0.0f);
}

Bbox::Bbox(BoxType chip, ML::Vec3 pos, ML::Box3D base, ML::QT qt)
{
	this->chip = chip;
	this->pos = pos;
	this->collision_Base = base;
	this->boxQT = qt;
}

//�N���A����
bool Bbox::Player_was_Clear_the_Game(const ML::Vec3& pos, const float& r, const ML::Vec3 speed)
{
	return this->Get_Collision_Bool(pos, r, speed);
}

//�e���|�[�g�����邩�𔻒�
bool Bbox::Player_was_Hit_the_Door(const ML::Vec3& pos, const float& r, const ML::Vec3 speed)
{
	return this->Get_Collision_Bool(pos, r, speed);
}

//���C�g���I�t���邩�𔻒�
bool Bbox::Player_Turnoff_the_Switch(const ML::Vec3& pos, const float& r, const ML::Vec3 speed)
{
	return this->Get_Collision_Bool(pos, r, speed);
}

bool Bbox::Get_Collision_Bool(const ML::Vec3& pos, const float& r, const ML::Vec3& speed)
{
	//�����蔻��̌��ʂ����炤
	std::vector<After_Collision> check;
	this->col.Hit_Check(&check, this->collision_Base.OffsetCopy(this->pos), pos, r, speed, this->boxQT);
	//���ʂ̃T�C�Y���O�ł͂Ȃ��Ȃ�N���A
	if (check.size() != 0)
	{
		return true;
	}
	return false;
}