#pragma once
#pragma warning(disable:4996)
#include "myLib.h"

//内積・外積を行うネームスペース
namespace MyMath
{
	//内積、外積
	void Vector_Dot(float* result, const ML::Vec3& v1, const ML::Vec3& v2);//引数：（a,b)　a・b
	void Vector_Cross(float* result, const ML::Vec3& v1, const ML::Vec3& v2);//引数：（a,b)　a X b
	void Get_Normal_to_Vector_Cross(ML::Vec3* normal, const ML::Vec3& v1, const ML::Vec3& v2);//法線ベクトルを求める関数
	void Get_Vector_Dot(float* result, const ML::Vec3& v1, const ML::Vec3& v2);//内積を求める関数
};