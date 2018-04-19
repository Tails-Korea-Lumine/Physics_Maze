#pragma once
//Base of Box Class

#include "GameEngine_Ver3_7.h"
#include "Collision.h"
#include "MyPG.h"

struct mapData
{
	int chip;//マスのマップチップ
	ML::Vec3 pos;//中心点の座標
	ML::Box3D collision_Base;//一個一個の判定範囲
};

class Bbox : public  BTask
{
public:

	//変数
	mapData arr[30][30][30];
	int sizeZ, sizeY, sizeX;
	float chipSize;
	ML::Box3D hitBase;//マップ全体の判定範囲
	string chipName[10];
	After_Collision collision_Tri;//ボールのほうに渡す情報
	std::vector< After_Collision> col_Poligons;

	ML::QT map_QT;//マップの回転量
	Collision col;


	//メソッド
	bool Map_Load(string f_);
	bool Map_Check_Hit(const ML::Box3D& hit);
	void Map_Check_Hit(ML::Vec3 pos, float r, ML::Vec3 speed);//球とマップのあたり判定		
	void Map_Rotate();//Updateで呼ぶ関数
	

	//コンストラクタ・デストラクタ
	Bbox();
	virtual ~Bbox(){}
};