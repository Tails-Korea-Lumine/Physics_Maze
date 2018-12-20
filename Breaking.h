#pragma once
#include "BEffect.h"

//壊れていくエフェクトクラス

class Breaking : public BEffect
{
public:
	//エフェクト行動
	void Effect_Update() override;
	//描画
	void Effect_Draw() const override;


	//コンストラクタ
	Breaking(const ML::Vec3& pos, const ML::Vec3& angle, const string& mesh_name, const string& sound_Name);
};