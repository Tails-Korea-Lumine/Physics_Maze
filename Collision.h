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

	After_Collision()
	{
		this->collision_Flag = false;
		this->normal = ML::Vec3(0, 0, 0);
	}
};

class Collision : public Gravity
{
private:
	//SUKA�v���O������Box3d����12�̎O�p�`�����o��
	void Get_Triangle_Box3D(std::vector<Triangle>* result, const ML::Box3D& box, const ML::QT& rotation);
	//�{�[���̍��W����6�̒��_�����o��
	void Get_Poionts_to_Sphere(std::vector<ML::Vec3>* result, const ML::Vec3& pos, const float& r, const ML::QT& rotation);
	//Box3d�Ƌ��̂̍ŒZ�����̓_����� ���� : (�{�b�N�X�̂����蔻��͈�, ���̒��S, �����a)
	void Get_ShortisetPoints_BoxtoSphere(std::vector<ML::Vec3>* result, const ML::Box3D& box, const ML::Vec3& pos, const float& r);
	//��̓_�ƎO�p�`�̏Փ˔�����s��
	bool Check_Collision(const Triangle& tri, const ML::Vec3& p);

public:
	//���ۂق��̃v���O�����ŌĂяo���֐�

	//���̂Ɨ����̏Փ˔���֐�
	//�����F(���ʂ�ۑ�����std::vector�̃A�h���X�A���肷��}�X�A�{�[���̈ʒu�A�{�[���̒��a�̔����A�{�[���̈ړ��x�N�g���A���[���h��]��)
	bool Hit_Check(std::vector<After_Collision>* result, const ML::Box3D& box, const ML::Vec3& pos,const float& r , const ML::Vec3& speed, const ML::QT& worldR);
	//�߂荞�񂾎��̈ʒu�␳�֐�
	//���� : ()
	void Fix_Position();
	
};