#include "Light_Switch.h"
#include "MyPG.h"
#include "Task_CameraMan.h"

bool Light_Switch::Collision_Action(std::vector<Collision_Data>* result, Shape3D* ball) 
{	
	//あたり判定結果をresultに保存
	if (!this->collision_Base->Hit(result, ball))
	{
		//当たらなかった時はfalseを返す
		return false;
	}
	//カメラマンにライトを3秒間オフする命令を送る
	ge->GetTask_One_G<CameraMan::Object>("カメラマン")->Turnoff_the_Light();
	//判定は当たったが物理処理はやらないのが望ましいのでfalseを返す
	return false;
}

Light_Switch::Light_Switch(const ML::Vec3& pos, const ML::Vec3& half_Of_Length, const ML::QT& qt, const string& id, const string& mesh_Name)
	:Bbox(pos, half_Of_Length, qt, id,mesh_Name)
{
	this->chip = Bbox::BoxType::LightSwitch;	
}