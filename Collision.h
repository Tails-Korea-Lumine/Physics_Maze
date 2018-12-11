#pragma warning(disable:4996)
#pragma once
#include "myLib.h"
#include "MyMath.h"
#include "Shape.h"

namespace Collision
{
	//あたり判定基準値
	static const float judge = _CMATH_::cosf(ML::ToRadian(359));	

	//一個の点と三角形の衝突判定を行う
	bool Check_Collision_Triangle_Point(const Triangle& tri, const ML::Vec3& p);

	//OBBをAABBに戻して一番近い点を算出する処理
	void Intersect_OBB_Sphere(ML::Vec3* result, const Shape3D* OBB, const Shape3D* Sphere);
	

	//球体と六面体衝突判定関数
	//引数：(結果を保存するstd::vectorのアドレス、3次元図形の判定する側、される側)
	bool Check_Collision_Cube_Sphere(std::vector<Collision_Data>* result, const Shape3D* cube, const ML::Vec3& nearest_point);
	
};

//static Collision col = Collision();