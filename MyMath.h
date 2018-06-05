#pragma once
#pragma warning(disable:4996)
#include "GameEngine_Ver3_7.h"

//内積・外積を行うクラス
class MyMath
{
public:
	//内積、外積
	void Vector_Dot(float* result, const ML::Vec3& v1, const ML::Vec3& v2) const;//引数：（a,b)　a・b
	void Vector_Cross(float* result, const ML::Vec3& v1, const ML::Vec3& v2) const;//引数：（a,b)　a X b
	void Get_Normal_to_Vector_Cross(ML::Vec3* normal, const ML::Vec3& v1, const ML::Vec3& v2) const;//法線ベクトルを求める関数
	void Get_Vector_Dot(float* result, const ML::Vec3& v1, const ML::Vec3& v2) const;//内積を求める関数
};