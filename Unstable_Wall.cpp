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
		this->hit_Flag = false;
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
	//エフェクト生成
	this->Be_Effect(result->at(result->size() - 1).normal);
	return true;
}

void Unstable_Wall::Be_Effect(const ML::Vec3& normal)
{
	auto eff = ge->eff_Manager.lock();
	if (this->life <= 0)
	{
		//壊れて消えるエフェクト生成
		eff->Add_Effect(this->Get_Pos(), normal, BEffect::effType::Crashed);
	}

	if (this->hit_Flag == false)
	{
		//あたったエフェクト生成
		eff->Add_Effect(this->Get_Pos(), normal, BEffect::effType::Breaking);
		this->hit_Flag = true;
	}
	
}

void Unstable_Wall::Rendering() const
{
	//ライフが残っていないならレンダリングしない
	if (this->life <= 0.0f)
	{
		return;
	}
	//行列生成
	ML::Mat4x4 matRT, matS;
	//スケーリング
	matS.Scaling(this->collision_Base->Get_Length()*2.0f);
	//アフィン変換
	D3DXMatrixAffineTransformation(&matRT, 1.0f, NULL, &this->collision_Base->Get_Quaternion(), &this->collision_Base->Get_Center());
	//ワールド行列に上書き
	DG::EffectState().param.matWorld = matS * matRT;
	//レンダリング
	DG::Mesh_Draw(this->mesh_Name);
}

void Unstable_Wall::Decrease_Life(const float& dec)
{
	this->life -= dec;
}

Unstable_Wall::Unstable_Wall(const ML::Vec3& pos, const ML::Vec3& half_Of_Length, const ML::QT& qt, const string& id, const string& mesh_Name)
	:Bbox(pos,half_Of_Length,qt,id,mesh_Name)
{
	this->chip = Bbox::BoxType::Unstable_Wall;
	this->life = 60.0f;
	this->hit_Flag = false;
}