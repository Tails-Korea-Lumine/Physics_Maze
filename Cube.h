#pragma warning(disable:4996)
#pragma once
#include "Shape.h"

//3�����Z�ʑ̃N���X

class Cube : public Shape3D
{
public:
	//�|���S�����o��
	virtual void Get_Triangle_Box3D(std::vector<Triangle>* result) const override;
	//�����蔻��
	virtual bool Hit(std::vector<Collision_Data>* result, Shape3D* other) override;

	//�R���X�g���N�^
	Cube(const ML::Vec3& center, const ML::Vec3& length_H, const ML::QT& qt);
};