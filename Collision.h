#pragma once
#include "GameEngine_Ver3_7.h"
#include "Gravity.h"

//�O�p�`�̊�{���
//���_�Ɩ@���x�N�g��
struct Triangle
{
	ML::Vec3 a, b, c;
	ML::Vec3 normal;
};

//�����蔻��̌�ɕԂ��\����
struct After_Collision
{
	bool collision_Flag;
	ML::Vec3 normal;
};

class Collision : public Gravity
{
private:
	//SUKA�v���O������Box3d����12�̎O�p�`�����o��
	std::vector<Triangle> Get_Triangle_Box3D(ML::Box3D box, ML::QT rotation);
	//�{�[���̍��W����6�̒��_�����o��
	std::vector<ML::Vec3> Get_Poionts_to_Sphere(ML::Vec3 pos, float r, ML::QT rotation);
	//Box3d�Ƌ��̂̍ŒZ�����̓_����� ���� : (�{�b�N�X�̂����蔻��͈�, ���̒��S, �����a)
	std::vector<ML::Vec3> Get_ShortisetPoints_BoxtoSphere(ML::Box3D box, ML::Vec3 pos, float r);
	//��̓_�ƎO�p�`�̏Փ˔�����s��
	bool Check_Collision(Triangle tri, ML::Vec3 p);

public:
	//���ۂق��̃v���O�����ŌĂяo���֐�

	//���̂Ɨ����̏Փ˔���֐�
	//�����F(���肷��}�X�A�{�[���̈ʒu�A�{�[���̒��a�̔����A�{�[���̈ړ��x�N�g���A���[���h��]��)
	std::vector<After_Collision> Hit_Check(const ML::Box3D& box, const ML::Vec3& pos,const float& r , const ML::Vec3& speed, const ML::QT& worldR);
	//�߂荞�񂾎��̈ʒu�␳�֐�
	//���� : ()
	void Fix_Position();
	
};