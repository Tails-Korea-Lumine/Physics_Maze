#pragma once
#pragma warning(disable : 4996)

//-------------------------------------------------------------------------
//エフェクト基礎クラス
//-------------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"


class BEffect 
{
	//変更不可------------------------------------------------
public:
	typedef  shared_ptr<BEffect>	SP;
	typedef  weak_ptr<BEffect>		WP;

	//変更可能---------------------------------------------------

public:
	//変数
	//---------------------------------------------------------------------------------------------
	//エフェクトハンドル
	//0番：キャラクタ生成
	//1番：アイテム生成
	//2番：アイテム消滅
	//3番：エネミー着地
	//4番：エネミーと衝突
	//5番：壁と衝突
	//6番：しっぽ生成
	//7番：しっぽ消滅
	//8番：しっぽと衝突
	//9番：キャラクタ死亡
	//---------------------------------------------------------------------------------------------
	enum effType
	{
		CLEAR = -1,
		CreateCharactor = 0,
		CreateItem,
		DestroyItem,
		EnemyLanding,
		Hit_to_Enemy,
		Hit_to_Wall,
		CreateTail,
		DestroyTail,
		Hit_to_Tail,
		DestroyCharactor
	};
	effType playing_EffectHandle;
	int effect_Life;
	
	ML::Vec3 pos;//エフェクト再生位置
	ML::Vec3 angle;//エフェクト再生角度
	ML::Vec3 scale;//エフェクトの大きさ
	ML::Vec3 moveVec;//エフェクトの移動速度
	float alpha;//透明度

	string meshName;//メッシュの名前
	string filePath;//メッシュのパス

	//メソッド
	//初期化でメッシュを読み込む関数
	void LoadEffect(effType handle);	
	//初期化関数
	void Eff_Initialize(ML::Vec3 pos, ML::Vec3 angle, effType handle);

	//生成消滅
	BEffect() :
		pos(0, 0, 0),
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