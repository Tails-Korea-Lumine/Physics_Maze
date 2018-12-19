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

void Goal::Rendering() const
{
	//行列生成
	ML::Mat4x4 matW;	
	//アフィン変換
	D3DXMatrixAffineTransformation(&matW, 100.0f, NULL, &this->collision_Base->Get_Quaternion(), &this->collision_Base->Get_Center());
	//ワールド行列に上書き
	DG::EffectState().param.matWorld = matW;
	//レンダリング
	DG::Mesh_Draw(this->mesh_Name);
}

Goal::Goal(const ML::Vec3& pos, const ML::Vec3& half_Of_Length, const ML::QT& qt, const string& id, const string& mesh_Name)
	:Bbox(pos, half_Of_Length, qt, id,mesh_Name)
{
	this->chip = Bbox::BoxType::Goal;
}