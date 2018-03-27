#pragma once
#pragma warning(disable : 4996)

//---------------------------------------------------------------------------------
//エフェクト描画クラス
//---------------------------------------------------------------------------------

#include "BEffect.h"


class Effect : public BEffect
{
public:
	typedef  shared_ptr<Effect>	SP;
	typedef  weak_ptr<Effect>	WP;
	//消滅関数
	void Finalize();


	//生成・消滅
	Effect() {}

	~Effect() {}

	//初期化関数
	void Load_Eff(ML::Vec3 pos, ML::Vec3 angle, effType handle);

	//描画メソッド
	//マネージャーで呼ぶのはこれらだけ
	//描画
	void Playing_Effect(effType ef);
	//アップデート
	void UpDate_Effect(effType ef);

private:
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
	
};