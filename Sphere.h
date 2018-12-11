#pragma warning(disable:4996)
#pragma once
#include "Shape.h"

//���̃N���X

class Sphere : public Shape3D
{
public:
	//�|���S�����o�����ݏ����Ȃ�(2018/12/10)
	virtual void Get_Triangle_Box3D(std::vector<Triangle>* result) const override;
	//�����蔻��
	virtual bool Hit(std::vector<Collision_Data>* result, Shape3D* other) override;

	//�R���X�g���N�^
	Sphere(const ML::Vec3& center, const ML::Vec3& length_H, const ML::QT& qt);
};