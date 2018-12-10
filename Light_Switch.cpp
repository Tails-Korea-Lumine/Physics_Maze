#include "Light_Switch.h"
#include "MyPG.h"
#include "Task_CameraMan.h"

bool Light_Switch::Collision_Action(std::vector<Collision_Data>* result, Shape3D* ball) const
{	
	//あたり判定結果をresultに保存
	if (!this->collision_Base->Hit(result, ball, this->unusable_Triagle))
	{
		//当たらなかった時はfalseを返す
		return false;
	}
	//カメラマンにライトを3秒間オフする命令を送る
	ge->GetTask_One_G<CameraMan::Object>("カメラマン")->Turnoff_the_Light();
	//判定は当たったのでtrueを返す
	return true;
}

Light_Switch::Light_Switch(const ML::Vec3& pos, const ML::Vec3& half_Of_Length, const ML::QT& qt, const string id)
	:Bbox(pos, half_Of_Length, qt, id)
{
	this->chip = Bbox::BoxType::LightSwitch;	
}