#pragma once
#include "BEffect.h"

//エフェクトテレポートインのクラス

class Teleport_In : public BEffect
{
public:

	//エフェクト行動
	void Effect_Update() override;
	//描画
	void Effect_Draw() const override;	

	//コンストラクタ
	Teleport_In(const ML::Vec3& pos, const ML::Vec3& target, const ML::Vec3& angle, const string& mesh_name, const string& sound_Name);
};