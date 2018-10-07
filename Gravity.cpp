#include "Gravity.h"
#include "MyPG.h"

void Gravity::Accelerate(ML::Vec3* speed, const float& Weight) const
{
	*speed += (Weight * this->G_acceleration );
}

void Gravity::CollisionOver_Accelerate(ML::Vec3* speed, const ML::Vec3& normal) const
{
	/*//法線ベクトルと速度の角度とサイン値
	float sin , seta;

	sin = Gravity::Vector_Dot(-speed, normal);

	seta = _CMATH_::asin(sin);

	//方向が変わった後の力の量
	float force;

	force = this->G_acceleration.y * sin;

	//回転軸は壁面の法線ベクトルと速度ベクトルの外積でもらえる別の法線ベクトル
	//回転量は反時計回りなので-(90-θ) = θ-90

	//回転用行列
	ML::Mat4x4 rotation;

	rotation.RotationAxis(Gravity::Get_Normal_to_Vector_Cross(-speed, normal), (seta - 90));

	rotation.TransformNormal(speed);
	
	//各要素に変化後の力の量を掛ける
	speed *= force;*/

	//----------------------------------------------------------------------------
	//このうえはver0.1

	float fn;
	MyMath::Get_Vector_Dot(&fn, *speed, normal);
	//fn *= speed.Length();
	//絶対値に変える
	if (fn < 0)
	{
		fn *= -1;
	}	
	
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
	if (fn < 0)
	{
		fn *= -1;
	}	

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
