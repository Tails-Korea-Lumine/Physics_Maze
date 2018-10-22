#pragma once
#pragma warning(disable : 4996)
#include "myLib.h"

//-------------------------------------------------------------------------
//エフェクト基礎クラス
//-------------------------------------------------------------------------
//前作で使用してた今回の作品には使用しないデータも含まれている


class BEffect 
{	

public:
	//変数
	//---------------------------------------------------------------------------------------------
	//エフェクトハンドル
	//0番：キャラクタ生成
	//1番：テレポートアウト
	//2番：テレポートイン
	//3番：エネミー着地
	//4番：エネミーと衝突
	//5番：壁と衝突
	//6番：しっぽ生成
	//7番：しっぽ消滅
	//8番：しっぽと衝突
	//9番：クリアエフェクト
	//---------------------------------------------------------------------------------------------
	enum effType
	{
		CLEAR = -1,
		CreateCharactor = 0,
		TeleportOut,
		Teleportin,
		EnemyLanding,
		Hit_to_Enemy,
		Hit_to_Wall,
		CreateTail,
		DestroyTail,
		Hit_to_Tail,
		Game_Clear,
	};
	effType playing_EffectHandle;
	int effect_Life;
	
	ML::Vec3 pos;//エフェクト再生位置
	ML::Vec3 target;//ターゲット位置(ある場合だけもらう)
	ML::Vec3 angle;//エフェクト再生角度
	ML::Vec3 scale;//エフェクトの大きさ
	ML::Vec3 moveVec;//エフェクトの移動速度
	float alpha;//透明度

	string meshName;//メッシュの名前
	//string filePath;//メッシュのパス



	//メソッド
	//初期化でメッシュを読み込む関数
	void LoadEffect(const effType& handle);

	//初期化メソッド
	//その場で全部処理するエフェクト
	void Eff_Initialize(const ML::Vec3& pos, const ML::Vec3& angle);
	//目的地まで移動しながら処理するエフェクト
	void Eff_Initialize(const ML::Vec3& pos, const ML::Vec3& target, const ML::Vec3& angle);

	//生成消滅
	//ゼロクリア
	BEffect() :
		pos(0, 0, 0),
		target(0,0,0),
		angle(0, 0, 0),
		scale(0,0,0),
		moveVec(0,0,0),
		playing_EffectHandle(CLEAR),
		effect_Life(0),
		alpha(1)
	{
		
	}

	virtual ~BEffect(){}
};