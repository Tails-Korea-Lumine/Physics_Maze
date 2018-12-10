#pragma once
#include "Bbox.h"

//障害物テレポートクラス

class Teleportation : public Bbox
{
private:
	//存在している面の番号
	unsigned int side_Number;
public:
	//あたり判定の行動
	bool Collision_Action(std::vector<Collision_Data>* result, Shape3D* ball) const override;
	//回転
	void Rotate_Box(ML::Mat4x4* mat, const ML::QT& q) override;

	//コンストラクタ
	Teleportation(const ML::Vec3& pos, const ML::Vec3& half_Of_Length, const ML::QT& qt, const string id, const int& side_Number);
};