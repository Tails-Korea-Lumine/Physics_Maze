#pragma once
#include "Bbox.h"

//壊れる壁クラス


class Unstable_Wall : public Bbox
{
private:
	//壊れるまでのライフ
	float life;
public:
	//あたり判定の行動
	bool Collision_Action(std::vector<Collision_Data>* result, Shape3D* ball) override;
	//レンダリング処理
	void Rendering()const override;
	//ライフ減少処理
	void Decrease_Life(const float& dec);

	//コンストラクタ
	Unstable_Wall(const ML::Vec3& pos, const ML::Vec3& half_Of_Length, const ML::QT& qt, const string& id, const string& mesh_Name);

};