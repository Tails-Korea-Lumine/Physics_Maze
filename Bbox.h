#pragma warning(disable:4996)
#pragma once
//Base of Box Class

//#include "GameEngine_Ver3_7.h"
#include "Shape.h"
//#include "MyPG.h"

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
public:
	enum class BoxType
	{
		Clear = -1, //何もない
		Road = 0, //道
		Wall = 1, //壁
		Goal = 2, //ゴール位置
		Teleportation = 3, //テレポーテーション
		LightSwitch = 4, //ライトのスイッチ//スイッチは絶対に行き止まりに設置しない
		Obstacle3 = 5, //障害物3番(未定)

		Core = 40, //真ん中のでかいキューブ
		Barrier = 50, //外に飛ばさないようにする透明な壁
		Start = 60,//スタート位置
				   //追加の余地あり
	};
protected:
	BoxType chip;//マスのマップチップ
	//ML::Vec3 pos;//中心点の座標
	Shape3D* collision_Base;//一個一個の判定範囲
	//ML::QT boxQT;//マップの回転量
	//ボックスのID
	string box_Id;	
	
public:
	//ほかのボックスと重なって使えない面を確認するフラグ
	bool unusable_Triagle[12];
	//メソッド	
	//回転関数　引数 : (回転した位置 , 回転量)
	virtual void Rotate_Box(ML::Mat4x4* mat, const ML::QT& q);

	//あたり判定に必要ない三角形を表示しておく
	void Marking_On_Unusable_Side(const Box_Side&);
	//表示したあたり判定に必要ない三角形を確認する
	bool Get_Marking_Unsuable_Side(const int&);
	
	//BoxTypeを確認する
	BoxType What_Type_Is_this_Box() const;
	//位置を返す関数
	ML::Vec3 Get_Pos() const;
	//idを返す関数
	string Get_Id() const;

	//衝突判定
	//判定の結果値をもらう関数
	virtual bool Collision_Action(std::vector<Collision_Data>* result, Shape3D* ball) const =0;

	//コンストラクタ・デストラクタ
	//引数なしコンストラクタ(ゼロクリア)
	Bbox();
	//引数 : (箱のタイプ,位置,あたり判定矩形,初期回転量,ボックスのID)
	Bbox(const ML::Vec3& pos, const ML::Vec3& half_Of_Length, const ML::QT& qt, const string id);
	

	virtual ~Bbox()
	{
		if (this->collision_Base != nullptr)
		{
			delete this->collision_Base;
		}
	}
};