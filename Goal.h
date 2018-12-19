#pragma once
#include "Bbox.h"

//目的地のクラス

class Goal : public Bbox
{
public:
	//あたり判定の行動
	bool Collision_Action(std::vector<Collision_Data>* result, Shape3D* ball) override;
	//レンダリング処理
	void Rendering()const override;
	//コンストラクタ
	Goal(const ML::Vec3& pos, const ML::Vec3& half_Of_Length, const ML::QT& qt, const string& id, const string& mesh_Name);
};