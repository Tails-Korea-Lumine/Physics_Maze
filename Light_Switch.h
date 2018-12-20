#pragma once
#include "Bbox.h"

//障害物ライトスイッチクラス

class Light_Switch : public Bbox
{
public:
	//あたり判定の行動
	bool Collision_Action(std::vector<Collision_Data>* result, Shape3D* ball) override;
	//レンダリング
	void Rendering()const override;

	//コンストラクタ
	Light_Switch(const ML::Vec3& pos, const ML::Vec3& half_Of_Length, const ML::QT& qt, const string& id, const string& mesh_Name);
};