#include "Gravity.h"
#include <cmath>

//法線ベクトルを求める
ML::Vec3 Gravity::Get_Normal_to_Vector_Cross(ML::Vec3 v1, ML::Vec3 v2)
{
	ML::Vec3 Normal;
	Normal.x = (v1.y*v2.z) - (v1.z*v2.y);
	Normal.y = (v1.z * v2.x) - (v1.x*v2.z);
	Normal.z = (v1.x*v2.y) - (v1.y*v2.x);

	return Normal;
}
//内積を求める
float Gravity::Get_Vector_Dot(ML::Vec3 v1, ML::Vec3 v2)
{
	float Dot_to_Position;
	Dot_to_Position = (v1.x*v2.x) + (v1.y*v2.y) + (v1.z*v2.z);

	return Dot_to_Position;
}

//内積でコサイン値を求める
float Gravity::Vector_Dot(ML::Vec3 v1, ML::Vec3 v2)
{
	//ベクトルの長さ
	float v1_Lenth, v2_Lenth;
	v1_Lenth = v1.Length();
	v2_Lenth = v2.Length();

	//座標上の内積
	float Dot_to_Position;
	Dot_to_Position = Gravity::Get_Vector_Dot(v1, v2);

	//コサイン値を求めて返す
	float cos;

	cos = Dot_to_Position / (v1_Lenth*v2_Lenth);

	return cos;
}

//外積でサイン値を求める
float Gravity::Vector_Cross(ML::Vec3 v1, ML::Vec3 v2)
{
	//ベクトルの長さ
	float v1_Lenth, v2_Lenth;

	v1_Lenth = v1.Length();
	v2_Lenth = v2.Length();	

	//座標上の外積(法線ベクトル)
	ML::Vec3 Cross_to_Position;
	Cross_to_Position = Gravity::Get_Normal_to_Vector_Cross(v1, v2);

	float sin;

	sin = Cross_to_Position.Length() / (v1_Lenth * v2_Lenth);

	return sin;
}

float Gravity::Cheak_angle(ML::Vec3 normal)
{
	return Gravity::Vector_Dot(this->G_acceleration, normal);
}

ML::Vec3 Gravity::Accelerate(float Weight)
{
	return Weight * this->G_acceleration;
}

ML::Vec3 Gravity::CollisionOver_Accelerate(ML::Vec3 speed, ML::Vec3 normal, float weight)
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
	//このうえはver.1

	float fn;
	fn = Gravity::Get_Vector_Dot(speed, normal);
	//fn *= speed.Length();
	//絶対値に変える
	if (fn < 0)
	{
		fn *= -1;
	}
	////マスとマス接触面でおかしい加速を防ぐ
	//float cosSN = Gravity::Vector_Dot((speed - Gravity::Accelerate(weight)), normal);
	////cos値が1ということは内角が0度だということ、つまり物理的にあり得ない衝突
	//if (cosSN >=_CMATH_::cos(ML::ToRadian(2)))
	//{
	//	//なので加速なしでreturn
	//	//return speed - Gravity::Accelerate(weight);
	//	return speed;
	//}
	
	//長さを力に合わせた後のベクトルの大きさ
	ML::Vec3 after_Normal;

	after_Normal = normal * fn;

	//反射角に変換したベクトル
	ML::Vec3 after_Collision;

	after_Collision = speed + after_Normal;
	//重さが加速に影響を与える
	return after_Collision;
}

ML::Vec3 Gravity::Reflaction_Vector(ML::Vec3 force, ML::Vec3 normal, float weight)
{	
	//壁面の法線ベクトルの大きさを変更
	float fn;
	fn = Gravity::Get_Vector_Dot(force, normal);
	//fn *= force.Length();
	//絶対値に変える
	if (fn < 0)
	{
		fn *= -1;
	}
	
	////マスとマス接触面でおかしい加速を防ぐ
	//float cosSN = Gravity::Vector_Dot((force - Gravity::Accelerate(weight)), normal);
	////cos値が1ということは内角が0度だということ、つまり物理的にあり得ない衝突
	//if (cosSN >= _CMATH_::cos(ML::ToRadian(2)))
	//{
	//	//なので加速なしでreturn
	//	//return force - Gravity::Accelerate(weight);
	//	return force;
	//}

	//長さを力に合わせた後のベクトルの大きさ
	ML::Vec3 after_Normal;

	after_Normal = normal * fn;

	//反射角に変換したベクトル
	ML::Vec3 after_Reflection;
	
	after_Reflection = force + (2 * after_Normal);

	//重さに応じて減らして返す
	return after_Reflection*0.6f;
}

void Gravity::Rotation_on_Gravity(float angle, ML::Vec3 centor)
{
	//マップの設計が終わった後に実装予定(2018/03/15)
}