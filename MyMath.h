#pragma once
#pragma warning(disable:4996)
#include "GameEngine_Ver3_7.h"

//内積・外積を行うクラス
class MyMath
{
public:
	//内積、外積
	float Vector_Dot(const ML::Vec3& v1, const ML::Vec3& v2);//引数：（a,b)　a・b
	float Vector_Cross(const ML::Vec3& v1, const ML::Vec3& v2);//引数：（a,b)　a X b
	void Get_Normal_to_Vector_Cross(ML::Vec3* normal, const ML::Vec3& v1, const ML::Vec3& v2);//法線ベクトルを求める関数
	float Get_Vector_Dot(const ML::Vec3& v1, const ML::Vec3& v2);//内積を求める関数
};