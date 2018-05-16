#pragma once
#pragma warning(disable:4996)
#include "GameEngine_Ver3_7.h"

//�d�̓N���X
//---------------------------------------------------------------

class Gravity
{
private:
	ML::Vec3 G_acceleration;

public:
	//���ρA�O��
	float Vector_Dot(const ML::Vec3& v1, const ML::Vec3& v2);//�����F�ia,b)�@a�Eb
	float Vector_Cross(const ML::Vec3& v1, const ML::Vec3& v2);//�����F�ia,b)�@a X b
	void Get_Normal_to_Vector_Cross(ML::Vec3* normal, const ML::Vec3& v1, const ML::Vec3& v2);//�@���x�N�g�������߂�֐�
	float Get_Vector_Dot(const ML::Vec3& v1, const ML::Vec3& v2);//���ς����߂�֐�	


	//����������
	ML::Vec3 Accelerate(const float& Weight);
	//�ǂƂԂ�������̈ړ������Ƒ��x��Ԃ�
	void CollisionOver_Accelerate(ML::Vec3* speed, const ML::Vec3& normal);

	//���˃x�N�g�������߂�
	void Reflaction_Vector(ML::Vec3* force, const ML::Vec3& normal, const float& weight);


//�������Ŏ�
	Gravity() : G_acceleration(0,-9.8f/60.0f/60.0f,0)
	{

	}

	virtual ~Gravity()
	{

	}
};