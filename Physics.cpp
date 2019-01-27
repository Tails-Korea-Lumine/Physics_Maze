#include "Physics.h"
#include "MyPG.h"


const static ML::Vec3 G_acceleration = ML::Vec3(0, -980.0f, 0);

void Physics::Gravity_Accelerate(ML::Vec3* speed, const float& Weight)
{
	//9.8m/s^2
	float delta2 = ge->g_Time.Delta_Time() * ge->g_Time.Delta_Time();
	*speed += (Weight * (G_acceleration * delta2));	
}

void Physics::Resizing_Normal(ML::Vec3* result, const ML::Vec3& force, const ML::Vec3& normal)
{
	//壁面の法線ベクトルの大きさを変更
	float fn;
	MyMath::Get_Vector_Dot(&fn, force, normal);
	//絶対値に変える
	fn = abs(fn);

	*result = normal * fn;
}

void Physics::Diagonal_Accelerate(ML::Vec3* speed, const ML::Vec3& normal)
{
	//長さを力に合わせた後のベクトル
	ML::Vec3 after_Normal;

	Physics::Resizing_Normal(&after_Normal, *speed, normal);

	//ベクトル合成
	*speed += after_Normal;	
}

void Physics::Reflaction_Vector(ML::Vec3* force, const ML::Vec3& normal)
{	
	//長さを力に合わせた後のベクトル
	ML::Vec3 after_Normal;

	Physics::Resizing_Normal(&after_Normal, *force, normal);

	//反射角に変換したベクトル
	*force += (1.5f * after_Normal);
	//消耗される力を考えて60％にする
	//*force *= 0.8f;	
}
