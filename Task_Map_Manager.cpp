//-------------------------------------------------------------------
//マップマネージャ
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Map_Manager.h"
#include  "Task_MapCore.h"
#include  "Task_MapSide.h"
#include  "Task_MapFence.h"
#include  "Task_Ball.h"
#include "easing.h"

namespace  Map_Manager
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{		
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{		
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
		easing::Set("StickVolume", easing::QUINTIN, 0,1.22f, 300);
		easing::Start("StickVolume");
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放


		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			//★引き継ぎタスクの生成
			//auto nextTask = Game::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		
		easing::Reset("StickVolume");
		//マップに関することは全部マネージャ側でやる ver0.6
		this->Managing_All_Map(15);
		
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		
	}

	void  Object::Render3D_L0()
	{
		
		
	}

	//----------------------------------------------------------------------------------
	//追加メソッド
	//マップの処理をいっぺんに管理する
	void Object::Managing_All_Map(const unsigned int delicate)
	{
		auto in1 = DI::GPad_GetState("P1");
		//ほかのタスクの情報をもらってくる
		auto map = ge->GetTask_Group_GN<Map3d::Object>("マップ", "Side");
		auto fence = ge->GetTask_Group_GN<MapFence::Object>("マップ", "Fence");
		auto core = ge->GetTask_One_G<Map_Core::Object>("マップ");
		auto ball = ge->GetTask_One_G<Ball::Object>("ボール");

		ML::QT frame_QTx = ML::QT(0.0f);
		ML::QT frame_QTy = ML::QT(0.0f);
		ML::QT frame_QTz = ML::QT(0.0f);
		std::vector<After_Collision> Result;
		for (unsigned int i = 0; i < delicate; i++)
		{
			//z軸回転がボタンを押しながらやるver0.1
			//if (in1.B1.on)
			//{
			//	//スティックで入力
			//	if (in1.LStick.axis.x != 0)
			//	{
			//		frame_QTx = ML::QT(ML::Vec3(0, 0, 1), ML::ToRadian(-in1.LStick.axis.x / float(delicate)));
			//		//this->map_QT *= frame_QT;
			//	}
			//}
			////押されていない時はY軸回転とX軸回転
			//else
			//{

			//	if (in1.LStick.axis.y != 0)
			//	{
			//		frame_QTx = ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(-in1.LStick.axis.y / float(delicate)));
			//		//this->map_QT *= frame_QT;
			//	}

			//	if (in1.LStick.axis.x != 0)
			//	{
			//		frame_QTy = ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(-in1.LStick.axis.x / float(delicate)));
			//		//this->map_QT *= frame_QT;
			//	}
			//}

			//スティックだけで回転させるver0.2
			//スティックが倒された量を更新
			//各軸の回転量にそれぞれ値が入る
			easing::UpDate();
			if(in1.LStick.axis.y !=0)
			{
				frame_QTx = ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(-in1.LStick.axis.y * easing::GetPos("StickVolume") / float(delicate)));				
			}			
			if (in1.RStick.axis.y != 0)
			{
				frame_QTx = ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(-in1.RStick.axis.y * easing::GetPos("StickVolume") / float(delicate)));
			}
			if (in1.LStick.axis.x != 0)
			{
				frame_QTy = ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(-in1.LStick.axis.x  * easing::GetPos("StickVolume") / float(delicate)));
			}			
			if (in1.RStick.axis.x != 0)
			{
				frame_QTz = ML::QT(ML::Vec3(0, 0, 1), ML::ToRadian(-in1.RStick.axis.x  * easing::GetPos("StickVolume") / float(delicate)));
			}		
			
			

			//各タスクの回転量を更新
			core->UpDate_Quartanion(frame_QTx * frame_QTy * frame_QTz);
			for (auto m = map->begin(); m != map->end(); m++)
			{
				(*m)->UpDate_Quartanion(frame_QTx * frame_QTy * frame_QTz);
			}
			for (auto f = fence->begin(); f != fence->end(); f++)
			{
				(*f)->UpDate_Quartanion(frame_QTx * frame_QTy * frame_QTz);
			}
			

			//ワールド回転量に反映
			//ge->World_Rotation = this->map_QT;		


			//回転
			core->Rotate_Core_and_Barrier();
			for (auto m = map->begin(); m != map->end(); m++)
			{
				(*m)->Map_Rotate();
			}
			for (auto f = fence->begin(); f != fence->end(); f++)
			{
				(*f)->Map_Rotate();
			}




			//あたり判定は毎回マップのほうで行う	

			//ボールとマップのあたり判定
			if (ball != nullptr)
			{
				core->Core_Check_Hit(ball->Get_Pos(), ball->Get_Radious(), ball->Get_Speed());
				for (auto m = map->begin(); m != map->end(); m++)
				{
					(*m)->Map_Check_Hit(ball->Get_Pos(), ball->Get_Radious(), ball->Get_Speed());
				}
				for (auto f = fence->begin(); f != fence->end(); f++)
				{
					(*f)->Map_Check_Hit(ball->Get_Pos(), ball->Get_Radious(), ball->Get_Speed());
				}

			}

			//判定の結果を保存
			Result = core->Get_Collision_Poligon();
			for (auto i = map->begin(); i != map->end(); i++)
			{
				for (auto it : (*i)->Get_Collision_Poligon())
				{
					Result.push_back(it);
				}
			}
			for (auto i = fence->begin(); i != fence->end(); i++)
			{
				for (auto it : (*i)->Get_Collision_Poligon())
				{
					Result.push_back(it);
				}
			}
			ge->collision_Result = Result;

			//位置補正を仕掛ける
			for (auto p : Result)
			{
				if (p.collision_Flag)
				{
					ball->Fix_Position_for_Rotate(frame_QTx * frame_QTy * frame_QTz);
					break;
				}
			}
		}
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