#pragma warning(disable:4996)
#pragma once
#include "myLib.h"
#include "MyMath.h"
#include "Shape.h"

class Collision
{
private:
	//一個の点と三角形の衝突判定を行う
	static bool Check_Collision_Triangle_Point(const Triangle& tri, const ML::Vec3& p);
	//OBBをAABBに戻して一番近い点を算出する処理
	static void Intersect_OBB_Sphere(ML::Vec3* result, const Shape3D* OBB, const Shape3D* Sphere);	
public:
	//球体と六面体衝突判定関数
	//引数：(結果を保存するstd::vectorのアドレス、3次元図形の判定する側、される側)
	static bool Check_Collision_Cube_Sphere(std::vector<Collision_Data>* result, const Shape3D* cube, const Shape3D* Sphere);
	
};