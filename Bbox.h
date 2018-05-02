#pragma once
//Base of Box Class

#include "GameEngine_Ver3_7.h"
#include "Collision.h"
#include "MyPG.h"

enum BoxType
{
	Clear = -1, //何もない
	Road = 0, //道
	Wall = 1, //壁
	Obstacle0 = 2, //障害物０番(未定)
	Obstacle1 = 3, //障害物０番(未定)
	Obstacle2 = 4, //障害物０番(未定)
	Obstacle3 = 5, //障害物０番(未定)

	Core = 40, //真ん中のでかいキューブ
	Barrier = 50, //外に飛ばさないようにする透明な壁
	//追加の余地あり
};

class Bbox 
{
private:
	BoxType chip;//マスのマップチップ
	ML::Vec3 pos;//中心点の座標
	ML::Box3D collision_Base;//一個一個の判定範囲
	ML::QT boxQT;//マップの回転量
	Collision col;
	
public:

	//メソッド	
	//回転関数　引数 : (回転した位置 , 回転量)
	void Rotate_Box(ML::Vec3 pos, ML::QT q);
	
	//道かどうかを確認する
	BoxType What_Type_Is_this_Box();
	//位置を返す関数
	ML::Vec3 Get_Pos();
	//衝突判定
	std::vector<After_Collision> Get_Collision_Poligon(ML::Vec3 pos, float r, ML::Vec3 speed);

	//コンストラクタ・デストラクタ
	Bbox();
	Bbox(BoxType chip, ML::Vec3 pos, ML::Box3D base, ML::QT qt);
	virtual ~Bbox(){}
};