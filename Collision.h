#pragma once
#include "myLib.h"

//三角形の基本情報
//頂点と法線ベクトル
struct Triangle
{
	ML::Vec3 a, b, c;
	ML::Vec3 normal;
};

//あたり判定の後に返す構造体
struct After_Collision
{
	bool collision_Flag;
	ML::Vec3 normal;

	After_Collision()
	{
		this->collision_Flag = false;
		this->normal = ML::Vec3(0, 0, 0);
	}
};

class Collision
{
private:
	//あたり判定基準値
	const float judge;

	//メソッド
	//SUKAプログラムのBox3dから12個の三角形を取り出す
	void Get_Triangle_Box3D(std::vector<Triangle>* result, const ML::Box3D& box, const ML::QT& rotation) const;
	//ボールの座標から6個の頂点を取り出す
	void Get_Poionts_to_Sphere(std::vector<ML::Vec3>* result, const ML::Vec3& pos, const float& r) const;
	//Box3dと球体の最短距離の点を取る 引数 : (ボックスのあたり判定範囲, 球の中心, 半直径)
	void Get_ShortisetPoints_BoxtoSphere(std::vector<ML::Vec3>* result, const ML::Box3D& box, const ML::Vec3& pos, const float& r) const;
	//一個の点と三角形の衝突判定を行う
	bool Check_Collision(const Triangle& tri, const ML::Vec3& p) const;

public:
	//実際ほかのプログラムで呼び出す関数

	//球体と立方体衝突判定関数
	//引数：(結果を保存するstd::vectorのアドレス、判定するマス、ボールの位置、ボールの直径の半分、ボールの移動ベクトル、ワールド回転量)
	bool Hit_Check(std::vector<After_Collision>* result, const ML::Box3D& box, const ML::Vec3& pos, const float& r, const ML::Vec3& speed, const ML::QT& worldR) const;
	
	
	Collision() :judge(_CMATH_::cosf(ML::ToRadian(359)))
	{

	}
};

static Collision col = Collision();