#pragma once
#include "BEffect.h"

//エフェクトゲームクリアクラス

class Game_Clear : public BEffect
{

public:

	//エフェクト行動
	void Effect_Update() override;
	//描画
	void Effect_Draw() const override;
	

	//コンストラクタ
	Game_Clear(const ML::Vec3& pos, const ML::Vec3& angle, const string& mesh_name, const string& sound_Name);
};