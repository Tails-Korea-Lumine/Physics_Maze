#pragma once
#include "myLib.h"
#include "MyMath.h"

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
	string collision_Id;
	ML::Vec3 normal;

	After_Collision()
	{
		this->collision_Flag = false;
		this->collision_Id = "";
		this->normal = ML::Vec3(0, 0, 0);
	}

	//比較演算子オーバーロード
	bool operator==(const After_Collision& ac)
	{
		float check;
		MyMath::Vector_Dot(&check, this->normal, ac.normal);
		return check >= _CMATH_::cosf(ML::ToRadian(359));
	}

	bool operator!=(const After_Collision& ac)
	{
		return *this == ac ? false : true;
	}
};

class Collision
{
private:
	//あたり判定基準値
	const float judge;
	//次のフレームに精密判定をするのかを確認するフラグ
	bool pricision_Flag;

	//メソッド	
	//一個の点と三角形の衝突判定を行う
	bool Check_Collision(const Triangle& tri, const ML::Vec3& p) const;

public:
	//実際ほかのプログラムで呼び出す関数

	//球体と立方体衝突判定関数
	//引数：(結果を保存するstd::vectorのアドレス、判定するマスの三角形、ボックスの中心点、ボックスの長さ、ボールの外角ドット、ボールの中心点、ボールの半直径、ボールの移動ベクトル、ワールド回転量)
	bool Hit_Check(std::vector<After_Collision>* result,const std::vector<Triangle>& all_Tri, const ML::Vec3 box_Center, const float& box_Length, std::vector<ML::Vec3>& all_Points, const ML::Vec3& ball_Pos, const float& r, const ML::Vec3& speed);
	
	
	Collision() :judge(_CMATH_::cosf(ML::ToRadian(359)))
	{
		this->pricision_Flag = false;
	}

	Collision operator= (const Collision& c)
	{
		return c;
	}
};

//static Collision col = Collision();