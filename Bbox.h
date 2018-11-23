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

//ボックスの面
//あたり判定不要ポリゴン表示に使用する
enum Box_Side
{
	Xplus, Xminus,
	Yplus, Yminus,
	Zplus, Zminus,
};

class Bbox 
{
private:
	BoxType chip;//マスのマップチップ
	ML::Vec3 pos;//中心点の座標
	ML::Box3D collision_Base;//一個一個の判定範囲
	ML::QT boxQT;//マップの回転量
	Collision col;//あたり判定コンポネント
	//ボックスのID
	string box_Id;
	//ほかのボックスと重なって使えない面を確認するフラグ
	bool unusable_Triagle[12];
	
	
	//あたり判定範囲を三角形で変換させる
	//SUKAプログラムのBox3dから12個の三角形を取り出す
	void Get_Triangle_Box3D(std::vector<Triangle>* result, const ML::Box3D& box, const ML::QT& rotation) const;
	//Box3dと球体の最短距離の点を取る 引数 : (ボックスのあたり判定範囲, 球の中心, 半直径)
	void Get_ShortisetPoints_Box_to_Sphere(std::vector<ML::Vec3>* result, const ML::Box3D& box) const;
	
public:

	//メソッド	
	//回転関数　引数 : (回転した位置 , 回転量)
	void Rotate_Box(ML::Mat4x4* mat, const ML::QT& q);

	//あたり判定に必要ない三角形を表示しておく
	void Marking_On_Unusable_Poligon(const Box_Side&);
	
	//BoxTypeを確認する
	BoxType What_Type_Is_this_Box() const;
	//位置を返す関数
	const ML::Vec3& Get_Pos() const;
	//idを返す関数
	string Get_Id() const;

	//衝突判定
	//判定の結果値をもらう関数
	bool Get_Collision_Poligon(std::vector<After_Collision>* result,std::vector<ML::Vec3> all_Points , const ML::Vec3& pos, const float& r, const ML::Vec3& speed);

	//コンストラクタ・デストラクタ
	//引数なしコンストラクタ(ゼロクリア)
	Bbox();
	//引数 : (箱のタイプ,位置,あたり判定矩形,初期回転量,ボックスのID)
	Bbox(const BoxType& chip, const ML::Vec3& pos, const ML::Box3D& base, const ML::QT& qt, const string id);
	//コピーコンストラクタ
	Bbox operator= (const Bbox& b)
	{		
		this->boxQT = b.boxQT;
		this->chip = b.chip;
		this->collision_Base = b.collision_Base;
		this->box_Id = b.box_Id;
		this->pos = b.pos;
		for (int i = 0; i < 12; i++)
		{
			this->unusable_Triagle[i] = b.unusable_Triagle[i];
		}
		this->col = Collision();

		return b;
	}

	~Bbox(){}
};