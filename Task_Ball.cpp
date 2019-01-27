//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Ball.h"
#include  "Task_MapSide.h"
#include  "Task_MapCore.h"
#include  "Task_MapFence.h"
#include "Task_CameraMan.h"
#include "Physics.h"
#include <iostream>

namespace  Ball
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->meshName = "Item";
		DG::Mesh_CreateFromSOBFile(this->meshName, "./data/mesh/Item.SOB");
		this->hit_Se_Name = "Hit_the_Ball";
		DM::Sound_CreateSE(this->hit_Se_Name, "./data/sound/Ball_Hit.wav");
	
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{		
		DG::Mesh_Erase(this->meshName);	
		DM::Sound_Erase(this->hit_Se_Name);
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化		
		this->speed = ML::Vec3(0, 0, 0);

		float r = 30.0f;
		this->sphere = new Sphere(ML::Vec3(1000, 500, 900), ML::Vec3(r, r, r), ML::QT());//初期生成位置は仮の位置
		this->m = 35.0f;
		this->teleportation_Flag = false;

	
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->eff_Manager.lock()->Add_Effect(this->Get_Pos(), ML::Vec3(0, 0, 0), BEffect::effType::Game_Clear);
		delete this->sphere;

		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			//★引き継ぎタスクの生成
			
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto cm = ge->GetTask_One_G<CameraMan::Object>("カメラマン");
		if (cm != nullptr && cm->Is_Moving_Now() == false)
		{
			//重力加速
			Physics::Gravity_Accelerate(&this->speed, this->m);
		}		

		//テレポートフラグを無効に
		this->teleportation_Flag = false;		
	}
	//-------------------------------------------------------------------
	//移動
	void Object::Move_Ball(const unsigned int& precisioin)
	{	
		//移動
		this->sphere->Offset(this->speed * ge->g_Time.Delta_Time() / (float)precisioin);
		//回転量上昇
		this->Rotate(precisioin);
	}

	//あたり判定による物理処理
	void Object::Physics_Actioin(const unsigned int& precision)
	{
		const float sin = sinf(ML::ToRadian(89));
		//結果の数分ループする
		for (auto p : ge->collision_Result)
		{
			//速度と法線ベクトルの角度が鋭角なら加速無し
			float cos;
			MyMath::Vector_Dot(&cos, this->speed, p.normal);
			if (cos >= 0.0f)
			{
				continue;
			}
			//反射角で跳ね返す
			Physics::Reflaction_Vector(&this->speed, p.normal);

			//衝突の音を出す処理
			float s;
			MyMath::Vector_Cross(&s, this->speed, p.normal);
			
			//反射角が一定角度以下または、1.3m/s以下は音を出さない
			if ((s <= sin) && speed.Length() >= 130.0f)
			{
				DM::Sound_Play(this->res->hit_Se_Name, false);
			}
		}		
		//移動
		this->Move_Ball(precision);
	}
	
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{		
	}

	void  Object::Render3D_L0()
	{
		ML::Mat4x4 mat;

		D3DXMatrixAffineTransformation(&mat, this->sphere->Get_Length().x, NULL, &this->sphere->Get_Quaternion(), &this->sphere->Get_Center());

		DG::EffectState().param.matWorld = mat;

		DG::Mesh_Draw(this->res->meshName);
	}

	//--------------------------------------------------------------------
	//追加メソッド
	
	//転がる表現のために回転させる
	void Object::Rotate(const unsigned int& precision)
	{
		ML::Vec3 anker;
		float rot;
		//回転軸を計算する
		for (auto& col : ge->collision_Result)
		{
			//回転軸を作る
			MyMath::Get_Normal_to_Vector_Cross(&anker, this->speed, col.normal);
			anker = anker.Normalize();
			//軸がゼロベクトルなら処理しない
			if (anker.Is_Zero_Vec())
			{
				continue;
			}
			//速度に対する回転量計算
			MyMath::Get_Rotation_By_Speed(&rot, this->sphere->Get_Length().x, this->speed.Length() * ge->g_Time.Delta_Time()/ precision);
			//回転行列とクォータニオンの生成
			ML::QT qt = ML::QT(anker, -rot);
			ML::Mat4x4 matR;
			D3DXMatrixAffineTransformation(&matR, 1.0f, &this->sphere->Get_Center(), &qt, NULL);

			//回転量アップデート
			this->sphere->Rotation(&matR, qt);			
		}
	}

	//---------------------------------------------------------------------
	//その時のボールの情報を返す関数
	//位置
	ML::Vec3 Object::Get_Pos() const
	{
		return this->sphere->Get_Center();
	}
	//半直径
	float Object::Get_Radious() const
	{
		return this->sphere->Get_Length().x;
	}
	//速度
	ML::Vec3 Object::Get_Speed() const
	{
		return this->speed;
	}

	//--------------------------------------------------------------------
	//位置補正用回転関数
	void Object::Fix_Position_for_Rotate(const ML::QT& qt)
	{
		if (ge->collision_Result.empty())
		{			
			return;
		}
		//マップのフレーム回転量で回転させる
		ML::Mat4x4 matR;
		D3DXMatrixAffineTransformation(&matR, 1, &ge->Map_center, &qt, NULL);

		//this->pos = matR.TransformCoord(this->pos);
		this->sphere->Rotation(&matR, qt);		
	}

	//----------------------------------------------------------------------------
	//テレポート(指定した場所に即移動する)
	void Object::Teleportation(const ML::Vec3& objectPos)
	{
		this->teleportation_Flag = true;
		this->sphere->Set_Position(objectPos);
	}

	//------------------------------------------------------------------------------
	//コアとあたってない時コアの方に引っ張られる
	/*void Object::Graviation_Pull(const ML::Vec3& force)
	{
		this->speed += force;
	}*/
	//-----------------------------------------------------------------------------
	//テレポートしたかを確認
	bool Object::Is_Teleport_Now() const
	{
		return this->teleportation_Flag;
	}
	//あたり判定範囲をもらう
	Shape3D* Object::Get_Collision_Area()
	{
		return this->sphere;
	}
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//イニシャライズに失敗したらKill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize()
	{
		return  this->Initialize();
	}
	//-------------------------------------------------------------------
	Object::~Object() { this->B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object() {	}
	//-------------------------------------------------------------------
	//リソースクラスの生成
	Resource::SP  Resource::Create()
	{
		if (auto sp = instance.lock()) {
			return sp;
		}
		else {
			sp = Resource::SP(new  Resource());
			if (sp) {
				sp->Initialize();
				instance = sp;
			}
			return sp;
		}
	}
	//-------------------------------------------------------------------
	Resource::Resource() {}
	//-------------------------------------------------------------------
	Resource::~Resource() { this->Finalize(); }
}