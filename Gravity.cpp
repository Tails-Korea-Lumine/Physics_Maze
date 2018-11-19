#include "Gravity.h"
#include "MyPG.h"

void Gravity::Accelerate(ML::Vec3* speed, const float& Weight) const
{
	*speed += (Weight * this->G_acceleration );
}

void Gravity::CollisionOver_Accelerate(ML::Vec3* speed, const ML::Vec3& normal) const
{
	float fn;
	MyMath::Get_Vector_Dot(&fn, *speed, normal);
	//fn *= speed.Length();
	//絶対値に変える
	fn = abs(fn);
	
	//長さを力に合わせた後のベクトルの大きさ
	ML::Vec3 after_Normal;

	after_Normal = normal * fn;

	//反射角に変換したベクトル
	//ML::Vec3 after_Collision;

	*speed += after_Normal;
	//重さが加速に影響を与える
	//return after_Collision;
}

void Gravity::Reflaction_Vector(ML::Vec3* force, const ML::Vec3& normal, const float& weight) const
{	
	//壁面の法線ベクトルの大きさを変更
	float fn;
	MyMath::Get_Vector_Dot(&fn, *force, normal);
	//fn *= force.Length();
	//絶対値に変える
	fn = abs(fn);

	//長さを力に合わせた後のベクトルの大きさ
	ML::Vec3 after_Normal;

	after_Normal = normal * fn;

	//反射角に変換したベクトル
	//ML::Vec3 after_Reflection;
	
	*force += (2 * after_Normal);
	*force *= 0.75f;

	//重さに応じて減らして返す
	//return after_Reflection *0.6f;
}
