#include "MyMath.h"

//法線ベクトルを求める
void MyMath::Get_Normal_to_Vector_Cross(ML::Vec3* normal, const ML::Vec3& v1, const ML::Vec3& v2)
{
	ML::Vec3 Normal;
	Normal.x = (v1.y*v2.z) - (v1.z*v2.y);
	Normal.y = (v1.z * v2.x) - (v1.x*v2.z);
	Normal.z = (v1.x*v2.y) - (v1.y*v2.x);

	*normal = Normal;
}
//内積を求める
void MyMath::Get_Vector_Dot(float* result, const ML::Vec3& v1, const ML::Vec3& v2)
{	
	*result = (v1.x*v2.x) + (v1.y*v2.y) + (v1.z*v2.z);
	
}

//内積でコサイン値を求める
void MyMath::Vector_Dot(float* result, const ML::Vec3& v1, const ML::Vec3& v2)
{
	//ベクトルの長さ
	float v1_Lenth, v2_Lenth;
	v1_Lenth = v1.Length();
	v2_Lenth = v2.Length();

	//座標上の内積
	float Dot_to_Position;
	MyMath::Get_Vector_Dot(&Dot_to_Position, v1, v2);

	//コサイン値を求めて返す
	*result = Dot_to_Position / (v1_Lenth*v2_Lenth);
	
}

//外積でサイン値を求める
void MyMath::Vector_Cross(float* result, const ML::Vec3& v1, const ML::Vec3& v2)
{
	//ベクトルの長さ
	float v1_Lenth, v2_Lenth;

	v1_Lenth = v1.Length();
	v2_Lenth = v2.Length();

	//座標上の外積(法線ベクトル)
	ML::Vec3 Cross_to_Position;
	MyMath::Get_Normal_to_Vector_Cross(&Cross_to_Position, v1, v2);

	//float sin;

	*result = Cross_to_Position.Length() / (v1_Lenth * v2_Lenth);	
}
