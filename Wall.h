#pragma once
#include "Bbox.h"

//障害物壁のクラス

class Wall : public Bbox
{
private:
	
public:
	//あたり判定の行動
	bool Collision_Action(std::vector<Collision_Data>* result, Shape3D* ball) override;

	//コンストラクタ
	Wall(const ML::Vec3& pos, const ML::Vec3& half_Of_Length, const ML::QT& qt, const string& id, const string& mesh_Name);
};