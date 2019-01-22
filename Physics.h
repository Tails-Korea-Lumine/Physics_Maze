#pragma once
#pragma warning(disable:4996)
#include "MyMath.h"


//---------------------------------------------------------------

class Physics
{
private:
	//力の量に対して法線ベクトルの長さを調整する
	static void Resizing_Normal(ML::Vec3* result, const ML::Vec3& force, const ML::Vec3& normal);

public:

	//加速させる
	static void Gravity_Accelerate(ML::Vec3* speed, const float& Weight);
	//壁とぶつかった後の移動方向と速度を返す
	static void Diagonal_Accelerate(ML::Vec3* speed, const ML::Vec3& normal);

	//反射ベクトルを求める
	static void Reflaction_Vector(ML::Vec3* force, const ML::Vec3& normal);
};