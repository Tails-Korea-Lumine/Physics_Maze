#pragma once
#pragma warning(disable:4996)
#include "GameEngine_Ver3_7.h"

//重力クラス
//---------------------------------------------------------------

class Gravity
{
private:
	ML::Vec3 G_acceleration;

public:
	//内積、外積
	float Vector_Dot(const ML::Vec3& v1, const ML::Vec3& v2);//引数：（a,b)　a・b
	float Vector_Cross(const ML::Vec3& v1, const ML::Vec3& v2);//引数：（a,b)　a X b
	void Get_Normal_to_Vector_Cross(ML::Vec3* normal, const ML::Vec3& v1, const ML::Vec3& v2);//法線ベクトルを求める関数
	float Get_Vector_Dot(const ML::Vec3& v1, const ML::Vec3& v2);//内積を求める関数	


	//加速させる
	ML::Vec3 Accelerate(const float& Weight);
	//壁とぶつかった後の移動方向と速度を返す
	void CollisionOver_Accelerate(ML::Vec3* speed, const ML::Vec3& normal);

	//反射ベクトルを求める
	void Reflaction_Vector(ML::Vec3* force, const ML::Vec3& normal, const float& weight);


//生成消滅字
	Gravity() : G_acceleration(0,-9.8f/60.0f/60.0f,0)
	{

	}

	virtual ~Gravity()
	{

	}
};