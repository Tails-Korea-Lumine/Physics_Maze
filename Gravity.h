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
	float Vector_Dot(ML::Vec3 v1, ML::Vec3 v2);//�����F�ia,b)�@a�Eb
	float Vector_Cross(ML::Vec3 v1, ML::Vec3 v2);//�����F�ia,b)�@a X b
	ML::Vec3 Get_Normal_to_Vector_Cross(ML::Vec3 v1, ML::Vec3 v2);//�@���x�N�g�������߂�֐�
	float Get_Vector_Dot(ML::Vec3 v1, ML::Vec3 v2);//���ς����߂�֐�
	

	//�O�ǂ̖ʂ̌X���𒲂ׂ�
	float Cheak_angle(ML::Vec3 Normal);
	//�d�͂ɑ΂��ĉ�]������f
	void Rotation_on_Gravity(float angle,ML::Vec3 centor);

	//����������
	ML::Vec3 Accelerate(float Weight);
	//�ǂƂԂ�������̈ړ������Ƒ��x��Ԃ�
	ML::Vec3 CollisionOver_Accelerate(ML::Vec3 speed, ML::Vec3 normal , float weight);

	//���˃x�N�g�������߂�
	ML::Vec3 Reflaction_Vector(ML::Vec3 force, ML::Vec3 normal, float weight);


//�������Ŏ�
	Gravity() : G_acceleration(0,-9.8f/60.0f/60.0f,0)
	{

	}

	virtual ~Gravity()
	{

	}
};