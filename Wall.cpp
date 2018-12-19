#include "Wall.h"


bool Wall::Collision_Action(std::vector<Collision_Data>* result, Shape3D* ball) 
{	
	//あたり判定結果をresultに保存
	if (!this->collision_Base->Hit(result, ball))
	{
		//当たらなかった時だけゼロベクトルリザルトをpushbackする
		result->push_back(Collision_Data());
	}
	//新しい判定結果にはIDをつけておく
	for (auto& r : *result)
	{
		if (r.collision_Id == "")
		{
			r.collision_Id = this->box_Id;
		}
	}
	//0番のフラグを戻り値で返す
	return result->at(0).collision_Flag;
}

Wall::Wall(const ML::Vec3& pos, const ML::Vec3& half_Of_Length, const ML::QT& qt, const string& id, const string& mesh_Name)
	:Bbox(pos,half_Of_Length,qt,id,mesh_Name)
{
	this->chip = Bbox::BoxType::Wall;
}