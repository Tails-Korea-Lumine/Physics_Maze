#pragma once
#include "GameEngine_Ver3_7.h"

//重力クラス
//---------------------------------------------------------------

class Gravity
{
private:
	ML::Vec3 G_acceleration;

public:
	//内積、外積
	float Vector_Dot(ML::Vec3 v1, ML::Vec3 v2);//引数：（a,b)　a・b
	float Vector_Cross(ML::Vec3 v1, ML::Vec3 v2);//引数：（a,b)　a X b
	ML::Vec3 Get_Normal_to_Vector_Cross(ML::Vec3 v1, ML::Vec3 v2);//法線ベクトルを求める関数
	float Get_Vector_Dot(ML::Vec3 v1, ML::Vec3 v2);//内積を求める関数
	

	//外壁の面の傾きを調べる
	float Cheak_angle(ML::Vec3 Normal);
	//重力に対して回転させるf
	void Rotation_on_Gravity(float angle,ML::Vec3 centor);

	//加速させる
	ML::Vec3 Accelerate(ML::Vec3 speed);
	//壁とぶつかった後の移動方向と速度を返す
	void CollisionOver_Accelerate(ML::Vec3& speed, ML::Vec3 normal);

	//反射ベクトルを求める
	ML::Vec3 Reflaction_Vector(ML::Vec3 force, ML::Vec3 normal);


//生成消滅字
	Gravity() : G_acceleration(0,-9.8f/60.0f/60.0f,0)
	{

	}

	~Gravity()
	{

	}
};