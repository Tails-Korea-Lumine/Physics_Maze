#pragma once
#include "BEffect.h"

//壊れる壁が壊れるときのエフェクトクラス

class Crashed : public BEffect
{
public:
	//エフェクト行動
	void Effect_Update() override;
	//描画
	//void Effect_Draw() const override;


	//コンストラクタ
	Crashed(const ML::Vec3& pos, const ML::Vec3& angle, const string& mesh_name, const string& sound_Name);
};