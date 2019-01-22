#pragma once
#pragma warning(disable:4996)
#include "MyMath.h"


//---------------------------------------------------------------

class Physics
{
private:
	//�̗͂ʂɑ΂��Ė@���x�N�g���̒����𒲐�����
	static void Resizing_Normal(ML::Vec3* result, const ML::Vec3& force, const ML::Vec3& normal);

public:

	//����������
	static void Gravity_Accelerate(ML::Vec3* speed, const float& Weight);
	//�ǂƂԂ�������̈ړ������Ƒ��x��Ԃ�
	static void Diagonal_Accelerate(ML::Vec3* speed, const ML::Vec3& normal);

	//���˃x�N�g�������߂�
	static void Reflaction_Vector(ML::Vec3* force, const ML::Vec3& normal);
};