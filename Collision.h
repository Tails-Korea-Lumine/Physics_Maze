#pragma once
#include "GameEngine_Ver3_7.h"
#include "Gravity.h"

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
};

class Collision : public Gravity
{
private:
	//SUKAプログラムのBox3dから12個の三角形を取り出す
	std::vector<Triangle> Get_Triangle_Box3D(ML::Box3D box, ML::QT rotation);
	//ボールの座標から6個の頂点を取り出す
	std::vector<ML::Vec3> Get_Poionts_to_Sphere(ML::Vec3 pos, float r, ML::QT rotation);
	//一個の点と三角形の衝突判定を行う
	bool Check_Collision(Triangle tri, ML::Vec3 p);

public:
	//実際ほかのプログラムで呼び出す関数

	//球体と立方体衝突判定関数
	//引数：(判定するマス、ボールの位置、ボールの直径の半分、ボールの移動ベクトル、ワールド回転量)
	After_Collision Hit_Check(ML::Box3D box, ML::Vec3 pos, float r , ML::Vec3 speed, ML::QT worldR);
	//めり込んだ時の位置補正関数
	//引数 : ()
	void Fix_Position();
};