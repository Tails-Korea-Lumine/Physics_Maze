#pragma once
#pragma warning(disable:4996)
#include "GameEngine_Ver3_7.h"

//���ρE�O�ς��s���N���X
class MyMath
{
public:
	//���ρA�O��
	void Vector_Dot(float* result, const ML::Vec3& v1, const ML::Vec3& v2);//�����F�ia,b)�@a�Eb
	void Vector_Cross(float* result, const ML::Vec3& v1, const ML::Vec3& v2);//�����F�ia,b)�@a X b
	void Get_Normal_to_Vector_Cross(ML::Vec3* normal, const ML::Vec3& v1, const ML::Vec3& v2);//�@���x�N�g�������߂�֐�
	void Get_Vector_Dot(float* result, const ML::Vec3& v1, const ML::Vec3& v2);//���ς����߂�֐�
};