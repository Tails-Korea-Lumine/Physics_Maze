#pragma once
#pragma warning(disable:4996)
#include "myLib.h"

#define ROOT3 1.732050f

//���ρE�O�ς��s���l�[���X�y�[�X
namespace MyMath
{
	//���ρA�O��
	//�R�T�C����Ԃ�
	//�����F�ia,b)�@a�Eb
	void Vector_Dot(float* result, const ML::Vec3& v1, const ML::Vec3& v2);

	//�T�C����Ԃ�
	//�����F�ia,b)�@a X b
	void Vector_Cross(float* result, const ML::Vec3& v1, const ML::Vec3& v2);

	//�@���x�N�g�������߂�֐�
	void Get_Normal_to_Vector_Cross(ML::Vec3* normal, const ML::Vec3& v1, const ML::Vec3& v2);

	//���ό��ʂ����߂�֐�
	void Get_Vector_Dot(float* result, const ML::Vec3& v1, const ML::Vec3& v2);

	//���x�����]�p�x�����߂鏈��
	//���� : (���ʂ�ۑ�����ꏊ�̃|�C���^�[�A�����a�A���x(�f���^�^�C��))
	void Get_Rotation_By_Speed(float* result, const float& r, const float& speed);
};