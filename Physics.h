#pragma once
#pragma warning(disable:4996)
#include "MyMath.h"


//---------------------------------------------------------------

namespace Physics
{

	//const static ML::Vec3 G_acceleration = ML::Vec3(0, -9.8f / 60.0f / 60.0f, 0);
	const static ML::Vec3 G_acceleration = ML::Vec3(0, -980.0f, 0);

	//����������
	void Gravity_Accelerate(ML::Vec3* speed, const float& Weight);
	//�ǂƂԂ�������̈ړ������Ƒ��x��Ԃ�
	void Diagonal_Accelerate(ML::Vec3* speed, const ML::Vec3& normal);

	//���˃x�N�g�������߂�
	void Reflaction_Vector(ML::Vec3* force, const ML::Vec3& normal);
};