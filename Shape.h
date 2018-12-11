#pragma warning(disable:4996)
#pragma once
#include "MyPG.h"
//#include "Collision.h"


//3D図形の基本形

class Shape3D
{
public:
	enum Shape3D_Type
	{
		Type_Clear = -1,//ポリモフされてない時
		Type_Cube = 1,//六面体
		Type_Sphere = 2,//球
	};

	//中心点
	ML::Vec3 center;
	//各軸の長さの半分
	ML::Vec3 half_of_Length;
	//回転量
	ML::QT quaternion;
	//どのものにポリモフされたか確認するメンバー
	Shape3D_Type type;
	//三角形で判定を行う場合、使えない三角形を確認するフラグ
	bool unusable_Triangle[12];

private:	

	
public:
	//ゲッター
	//中心点
	ML::Vec3 Get_Center() const;
	//長さ成分
	ML::Vec3 Get_Length() const;
	//回転量
	ML::QT Get_Quaternion() const;	

	//仮想関数
	//構成ポリゴン
	virtual void Get_Triangle_Box3D(std::vector<Triangle>* result) const =0;
	//あたり判定
	//引数 : (判定を保存するヴェクター、判定を行う3次元図形、例外的に判定をしない条件のラムダ式)
	virtual bool Hit(std::vector<Collision_Data>* result, Shape3D* other) =0;

	//移動
	//引数 : (移動量)
	void Offset(const ML::Vec3& vec);
	//位置の上書き
	//引数 : (上書きする場所)
	void Set_Position(const ML::Vec3& pos);

	//回転
	//引数 : (回転量)
	void Rotation(ML::Mat4x4* mat, const ML::QT& qt);

	//判定しないポリゴンにマーキングしておく処理
	void Marking_On_Unsuable_Poligon(const size_t&);

	//コンストラクタ
	//引数 : (中心点、長さの半分、初期の回転量)
	Shape3D(const ML::Vec3& center, const ML::Vec3& length_H, const ML::QT& qt);
	//デストラクタ
	virtual ~Shape3D(){}
};