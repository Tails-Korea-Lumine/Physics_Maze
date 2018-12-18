#include "Goal.h"
#include "MyPG.h"

bool Goal::Collision_Action(std::vector<Collision_Data>* result, Shape3D* ball) 
{	
	//あたり判定結果をresultに保存
	if (!this->collision_Base->Hit(result, ball))
	{
		//当たらなかった時はfalseを返す
		return false;
	}

	//ゲームタスクにクリア処理をさせる
	ge->game.lock()->Game_Clear();
	return true;
}

Goal::Goal(const ML::Vec3& pos, const ML::Vec3& half_Of_Length, const ML::QT& qt, const string id)
	:Bbox(pos, half_Of_Length, qt, id)
{
	this->chip = Bbox::BoxType::Goal;
}