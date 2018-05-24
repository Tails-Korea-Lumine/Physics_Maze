#pragma once
#pragma warning(disable:4996)
#include "GameEngine_Ver3_7.h"
#include "MyMath.h"

//重力クラス
//---------------------------------------------------------------

class Gravity : public MyMath
{
private:
	ML::Vec3 G_acceleration;

public:
	//加速させる
	void Accelerate(ML::Vec3* speed, const float& Weight);
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