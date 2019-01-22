#pragma once
#pragma warning(disable:4996)
#include "myLib.h"

#define ROOT3 1.732050f

//内積・外積を行うネームスペース
class MyMath
{

public:
	//内積、外積
	//コサインを返す
	//引数：（a,b)　a・b
	static void Vector_Dot(float* result, const ML::Vec3& v1, const ML::Vec3& v2);

	//サインを返す
	//引数：（a,b)　a X b
	static void Vector_Cross(float* result, const ML::Vec3& v1, const ML::Vec3& v2);

	//法線ベクトルを求める関数
	static void Get_Normal_to_Vector_Cross(ML::Vec3* normal, const ML::Vec3& v1, const ML::Vec3& v2);

	//内積結果を求める関数
	static void Get_Vector_Dot(float* result, const ML::Vec3& v1, const ML::Vec3& v2);

	//速度から回転角度を求める処理
	//引数 : (結果を保存する場所のポインター、半直径、速度(デルタタイム))
	static void Get_Rotation_By_Speed(float* result, const float& r, const float& speed);
};