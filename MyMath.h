#pragma once
#pragma warning(disable:4996)
#include "GameEngine_Ver3_7.h"

//���ρE�O�ς��s���N���X
class MyMath
{
public:
	//���ρA�O��
	float Vector_Dot(const ML::Vec3& v1, const ML::Vec3& v2);//�����F�ia,b)�@a�Eb
	float Vector_Cross(const ML::Vec3& v1, const ML::Vec3& v2);//�����F�ia,b)�@a X b
	void Get_Normal_to_Vector_Cross(ML::Vec3* normal, const ML::Vec3& v1, const ML::Vec3& v2);//�@���x�N�g�������߂�֐�
	float Get_Vector_Dot(const ML::Vec3& v1, const ML::Vec3& v2);//���ς����߂�֐�
};