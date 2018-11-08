#pragma once
#pragma warning(disable : 4996)
#include "BEffect.h"

//---------------------------------------------------------------------------------
//エフェクトクラス
//---------------------------------------------------------------------------------


class Effect : private BEffect
{
private:
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
	//触接呼ばなくてもよい関数
	//描画
	void Playing_EF0();
	void Playing_EF1();
	void Playing_EF2();
	void Playing_EF3();
	void Playing_EF4();
	void Playing_EF5();
	void Playing_EF6();
	void Playing_EF7();
	void Playing_EF8();
	void Playing_EF9();
	//アップデート
	void UpDate_EF0();
	void UpDate_EF1();
	void UpDate_EF2();
	void UpDate_EF3();
	void UpDate_EF4();
	void UpDate_EF5();
	void UpDate_EF6();
	void UpDate_EF7();
	void UpDate_EF8();
	void UpDate_EF9();


public:

	//生成・消滅
	Effect(const effType& handle) 
	{
		this->playing_EffectHandle = handle;
		this->LoadEffect(handle);
	}

	~Effect() 
	{
		
	}

	//初期化関数
	//その場で全部処理するエフェクト
	void Load_Eff(const ML::Vec3& pos, const ML::Vec3& angle);
	//目的地まで移動しながら処理するエフェクト
	void Load_Eff(const ML::Vec3& pos, const ML::Vec3& target, const ML::Vec3& angle);
	

	//メソッド	
	//描画
	void Playing_Effect();
	//アップデート
	void UpDate_Effect();	
	//エフェクトの残り時間を減少させる
	void Dec_Eff();
	//エフェクトが生きているかを確認する
	bool Is_Alive() const;
	//エフェクトタイプをもらう
	effType Get_Type() const;
	//removeで削除する判定式
	bool Eff_Judge() const;
	
};