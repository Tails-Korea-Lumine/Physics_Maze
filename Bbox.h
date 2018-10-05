#pragma warning(disable:4996)
#pragma once
//Base of Box Class

//#include "GameEngine_Ver3_7.h"
#include "Collision.h"
//#include "MyPG.h"

enum BoxType
{
	Clear = -1, //何もない
	Road = 0, //道
	Wall = 1, //壁
	Goal = 2, //ゴール位置
	Teleportaion = 3, //テレポーテーション
	LightSwitch = 4, //ライトのスイッチ//スイッチは絶対に行き止まりに設置しない
	Obstacle3 = 5, //障害物3番(未定)

	Core = 40, //真ん中のでかいキューブ
	Barrier = 50, //外に飛ばさないようにする透明な壁
	Start = 60,//スタート位置
	//追加の余地あり
};

class Bbox 
{
private:
	BoxType chip;//マスのマップチップ
	ML::Vec3 pos;//中心点の座標
	ML::Box3D collision_Base;//一個一個の判定範囲
	ML::QT boxQT;//マップの回転量
	Collision col;//あたり判定コンポネント
	
	//衝突判定
	//判定の正不正だけをもらう関数
	bool Get_Collision_Bool(const ML::Vec3& pos, const float& r, const ML::Vec3& speed);
	
public:

	//メソッド	
	//回転関数　引数 : (回転した位置 , 回転量)
	void Rotate_Box(const ML::Vec3& pos, const ML::QT& q);
	
	//BoxTypeを確認する
	BoxType What_Type_Is_this_Box() const;
	//位置を返す関数
	ML::Vec3 Get_Pos() const;
	//衝突判定
	//判定の結果値をもらう関数
	void Get_Collision_Poligon(std::vector<After_Collision>* result, const ML::Vec3& pos, const float& r, const ML::Vec3& speed);

	//ゲームクリア判定
	bool Player_was_Clear_the_Game(const ML::Vec3& pos, const float& r, const ML::Vec3 speed);
	//テレポートするかを判定
	bool Player_was_Hit_the_Door(const ML::Vec3& pos, const float& r, const ML::Vec3 speed);
	//ライトのスイッチを押したかを判別
	bool Player_Turnoff_the_Switch(const ML::Vec3& pos, const float& r, const ML::Vec3 speed);

	//コンストラクタ・デストラクタ
	//引数なしコンストラクタ(ゼロクリア)
	Bbox();
	//引数 : (箱のタイプ,位置,あたり判定矩形,初期回転量)
	Bbox(BoxType chip, ML::Vec3 pos, ML::Box3D base, ML::QT qt);
	//コピーコンストラクタ
	Bbox operator= (const Bbox& b)
	{		
		this->boxQT = b.boxQT;
		this->chip = b.chip;
		this->collision_Base = b.collision_Base;
		this->pos = b.pos;
		this->col = Collision();

		return b;
	}

	~Bbox(){}
};