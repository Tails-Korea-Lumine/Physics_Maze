#include "Physics.h"
#include "MyPG.h"

void Physics::Gravity_Accelerate(ML::Vec3* speed, const float& Weight)
{
	float delta2 = ge->g_Time.Delta_Time() * ge->g_Time.Delta_Time();
	*speed += (Weight * (G_acceleration * delta2));	
}

void Physics::Diagonal_Accelerate(ML::Vec3* speed, const ML::Vec3& normal)
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

void Physics::Reflaction_Vector(ML::Vec3* force, const ML::Vec3& normal)
{
	//0.5m/sより力が少ない場合は斜め加速をさせる
	if (force->Length() <= 50.0f)
	{
		Diagonal_Accelerate(force, normal);
		return;
	}

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
	*force += (2 * after_Normal);
	//慣性を考えて60％にする
	*force *= 0.6f;	
}
