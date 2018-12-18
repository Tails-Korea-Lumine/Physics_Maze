#include "Unstable_Wall.h"
#include "MyPG.h"
#include "Task_Ball.h"

bool Unstable_Wall::Collision_Action(std::vector<Collision_Data>* result, Shape3D* ball) 
{
	//ライフが残っているときのみ処理をする
	if (this->life <= 0)
	{
		return false;
	}
	//あたり判定結果をresultに保存
	if (!this->collision_Base->Hit(result, ball))
	{
		//当たらなかった時だけゼロベクトルリザルトをpushbackして終了する
		result->push_back(Collision_Data());
		return false;
	}	
	//新しい判定結果にはIDをつけておく
	for (auto& r : *result)
	{
		if (r.collision_Id == "")
		{
			r.collision_Id = this->box_Id;
		}
	}
	//ボール速度のスカラー量分ライフを消す	
	this->Decrease_Life(ge->GetTask_One_G<Ball::Object>("ボール")->Get_Speed().Length());
}

void Unstable_Wall::Decrease_Life(const float& dec)
{
	this->life -= dec;
}

Unstable_Wall::Unstable_Wall(const ML::Vec3& pos, const ML::Vec3& half_Of_Length, const ML::QT& qt, const string id)
	:Bbox(pos,half_Of_Length,qt,id)
{
	this->chip = Bbox::BoxType::Unstable_Wall;
	this->life = 18.0f;
}