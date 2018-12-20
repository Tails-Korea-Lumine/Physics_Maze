#pragma once
#include "BEffect.h"

//エフェクトテレポートアウトクラス

class Teleport_Out : public BEffect
{
public:

	//エフェクト行動
	void Effect_Update() override;
	//描画
	//void Effect_Draw() const override;
	

	//コンストラクタ
	Teleport_Out(const ML::Vec3& pos, const ML::Vec3& angle, const string& mesh_name, const string& sound_Name);
};