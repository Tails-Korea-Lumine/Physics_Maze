#include "Teleportation.h"
#include "MyPG.h"

bool Teleportation::Collision_Action(std::vector<Collision_Data>* result, Shape3D* ball) const
{	
	//あたり判定結果をresultに保存
	if (!this->collision_Base->Hit(result, ball, this->unusable_Triagle))
	{
		//当たらなかった時はfalseを返す
		return false;
	}
	//テレポートアウト場所
	ML::Vec3 exitpos;
	//テレポートができない時もfalseを返す
	if (ge->TM.Find_Exit(this->side_Number, &exitpos))
	{
		ball->Set_Position(exitpos);
		auto eff = ge->eff_Manager.lock();
		//テレポートインのエフェクト
		eff->Add_Effect(ball->Get_Center(), this->Get_Pos(), ML::Vec3(0, 0, 0), BEffect::effType::Teleportin);
		//テレポートインの音を鳴らす
		//DM::Sound_Play(this->res->seTeleportIn, false);
		//テレポートアウトのエフェクト
		eff->Add_Effect(exitpos, exitpos - ge->Map_center, BEffect::effType::TeleportOut);
		//テレポートアウトの音を鳴らす
		//DM::Sound_Play(this->res->seTeleportOut, false);

		return true;
	}
	return false;
}

void Teleportation::Rotate_Box(ML::Mat4x4* mat, const ML::QT& q)
{
	//回転量と位置を更新
	this->collision_Base->Rotation(mat, q);
	//テレポート扉の位置更新
	ge->TM.Update_Door_Position(this->side_Number, this->Get_Pos());
}

Teleportation::Teleportation(const ML::Vec3& pos, const ML::Vec3& half_Of_Length, const ML::QT& qt, const string id, const int& side_Number)
	:Bbox(pos, half_Of_Length, qt, id)
{
	this->chip = Bbox::BoxType::Teleportation;
	this->side_Number = side_Number;
	ge->TM.Update_Door_Position(this->side_Number, pos);
}