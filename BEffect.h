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
	//0番：テレポートアウト
	//1番：テレポートイン
	//2番：ゲームクリア
	//3番：壊れるエフェクト
	//4番：破片散らばし
	//---------------------------------------------------------------------------------------------
	enum effType
	{
		EFF_CLEAR = -1,		
		TeleportOut =0,
		Teleportin,		
		Game_Clear,
		Breaking,
		Crashed,
	};
protected:
	//effType playing_EffectHandle;
	int effect_Life;
	
	ML::Vec3 pos;//エフェクト再生位置
	ML::Vec3 target;//ターゲット位置(ある場合だけもらう)
	ML::Vec3 angle;//エフェクト再生角度
	ML::Vec3 scale;//エフェクトの大きさ
	ML::Vec3 moveVec;//エフェクトの移動速度
	float alpha;//透明度

	string meshName;//メッシュの名前
	string sound_Name;//サウンドエフェクトの名前

public:
	//メソッド
	//初期化でメッシュを読み込む関数
	//void LoadEffect(const effType& handle);	

	//仮想関数
	virtual void Effect_Update() = 0;
	virtual void Effect_Draw() const = 0;

	//メソッド
	//エフェクトの残り時間を減少させる
	void Dec_Eff();
	//エフェクトが生きているかを確認する
	bool Is_Alive() const;	
	//表示角度をクォータニオンで計算する
	ML::QT Calc_Qt() const;

	//生成消滅
	//ゼロクリア
	BEffect() :
		pos(0, 0, 0),
		target(0,0,0),
		angle(0, 0, 0),
		scale(0,0,0),
		moveVec(0,0,0),
		//playing_EffectHandle(EFF_CLEAR),
		effect_Life(0),
		alpha(1)
	{
		
	}
	//コンストラクタ
	//生成場所だけで行うエフェクト
	//引数 : (生成位置、生成向き)
	BEffect(const ML::Vec3& pos, const ML::Vec3& angle);
	//目的地まで移動しながら処理するエフェクト
	//引数 : (生成位置、目的地、生成向き)
	BEffect(const ML::Vec3& pos, const ML::Vec3& target, const ML::Vec3& angle);

	virtual ~BEffect(){}
};