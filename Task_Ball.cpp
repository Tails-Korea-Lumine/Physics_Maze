//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Ball.h"
#include  "Task_MapSide.h"
#include  "Task_MapCore.h"
#include  "Task_MapFence.h"
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
		//this->pos = ML::Vec3(1000, 500, 900);//仮の位置後で調整をかける(2018/04/20)
		this->speed = ML::Vec3(0, 0, 0);
		//this->moveVec = ML::Vec3(0, 0, 0);

		float r = 30.0f;
		this->sphere = new Sphere(ML::Vec3(1000, 500, 900), ML::Vec3(r, r, r), ML::QT());
		this->m = 15.0f;
		//this->rot = 0.0f;
		this->collision_Flag.clear();
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
		this->collision_Flag.clear();
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
		//重力加速
		Physics::Gravity_Accelerate(&this->speed,this->m);				
			
		//回転量上昇
		this->Rotate();

		//テレポートフラグを無効に
		this->teleportation_Flag = false;
		
		//デバッグ用
		float sl = this->speed.Length();
		if (sl > 4.0f)
		{
			cout << sl << endl;
		}		
	}
	//-------------------------------------------------------------------
	//あたり判定による方向転換及び移動
	void Object::Move_Ball(const unsigned int& precisioin)
	{
		int vol;
		//終端速度及び物理精密度による速度処理
		auto Clamp_Speed = [&](ML::Vec3& speed)
		{
			//1フレームの終端速度
			const float termination_Speed = 6.0f;
			//終端速度に調整する場合
			if (speed.Length() > termination_Speed)
			{
				speed = speed.Normalize();
				speed *= termination_Speed;
			}
			return speed / (float)precisioin;
		};
		
		//もし,どこもあたり判定をせずに動いた場合
		//処理せずに次のフレームに移る
		if (ge->collision_Result.empty())
		{
			//今回のフレームに衝突しなかったら
			//衝突フラグを無効にする
			this->collision_Flag.clear();
			
			//移動(フレーム終了する直前に行う)
			this->sphere->Offset(Clamp_Speed(this->speed));
			return;
		}
		//前回フレームのあたり判定結果を確認
		
		//結果の数分ループする
		for (auto p : ge->collision_Result)
		{
			//速度と法線ベクトルの角度が似た方向なら加速無し
			float cos;
			MyMath::Vector_Dot(&cos, this->speed, p.normal);
			if (cos >= cosf(ML::ToRadian(85)))
			{
				continue;
			}
			//前の処理で衝突だったら、今回のフレームでの衝突判定でやること
			if (this->Is_Collision(p.collision_Id))
			{
				//今回のフレームに衝突だったら
				//斜め線加速をする
				Physics::Diagonal_Accelerate(&this->speed, p.normal);
			}		
			//前の処理で衝突ではなかったら、今回のフレームでの衝突判定でやること			
			else
			{
				//今回のフレームに衝突だったら
				//反射角で跳ね返す
				Physics::Reflaction_Vector(&this->speed, p.normal);
				//DM::Sound_Volume(this->res->hit_Se_Name, int((this->speed.Length() / 6) * 1000));
				DM::Sound_Play(this->res->hit_Se_Name, false);
			}
		}		

		//今回使ったフラグは全部消して、今回のあたり判定結果のフラグを書き込む
		this->collision_Flag.clear();
		for (auto& p : ge->collision_Result)
		{			
			this->collision_Flag.push_back(p.collision_Id);
		}
		
		//移動(フレーム終了する直前に行う)
		this->sphere->Offset(Clamp_Speed(this->speed));
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

	//あたり判定フラグを確認
	bool Object::Is_Collision(const string& id) const
	{
		for (auto& cf : this->collision_Flag)
		{
			if (cf == id)
			{
				return true;
			}
		}
		return false;
	}
	//--------------------------------------------------------------
	//転がる表現のために回転させる
	void Object::Rotate()
	{
		ML::Vec3 anker;
		float cos;
		//回転軸を計算する
		for (auto& col : ge->collision_Result)
		{
			//内積で速度と90度の法線ベクトルを探す
			MyMath::Vector_Dot(&cos, this->speed, col.normal);

			if (cos >= _CMATH_::cosf(ML::ToRadian(90)))
			{
				//回転軸を作る
				MyMath::Get_Normal_to_Vector_Cross(&anker, this->speed, col.normal);
				//回転行列とクォータニオンの生成
				ML::QT rot = ML::QT(anker, this->speed.Length());
				ML::Mat4x4 matR;
				D3DXMatrixAffineTransformation(&matR, 1.0f, &this->sphere->Get_Center(), &rot, NULL);

				//回転量アップデート
				this->sphere->Rotation(&matR, rot);
			}
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
			DM::Sound_Volume(this->res->hit_Se_Name, 1000);
			return;
		}
		//マップのフレーム回転量で回転させる
		ML::Mat4x4 matR;
		D3DXMatrixAffineTransformation(&matR, 1, &ge->Map_center, &qt, NULL);

		//this->pos = matR.TransformCoord(this->pos);
		this->sphere->Rotation(&matR, qt);

		DM::Sound_Volume(this->res->hit_Se_Name, 0);
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