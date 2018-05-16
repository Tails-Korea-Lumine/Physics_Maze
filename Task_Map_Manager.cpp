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
		easing::Init();

		this->frame_QTxp = ML::QT(0.0f);
		this->frame_QTxm = ML::QT(0.0f);
		this->frame_QTyp = ML::QT(0.0f);
		this->frame_QTym = ML::QT(0.0f);
		this->frame_QTzp = ML::QT(0.0f);
		this->frame_QTzm = ML::QT(0.0f);
	
		//easing set
		easing::Set("Decrese_StickVolumeXP", easing::QUARTIN, 1.7f, 0, 4);
		easing::Set("Decrese_StickVolumeXM", easing::QUARTIN, 1.7f, 0, 4);
		easing::Set("Decrese_StickVolumeYP", easing::QUARTIN, 1.7f, 0, 4);
		easing::Set("Decrese_StickVolumeYM", easing::QUARTIN, 1.7f, 0, 4);
		easing::Set("Decrese_StickVolumeZP", easing::QUARTIN, 1.7f, 0, 4);
		easing::Set("Decrese_StickVolumeZM", easing::QUARTIN, 1.7f, 0, 4);
		//easing set end						 
		easing::Set_End("Decrese_StickVolumeXP");
		easing::Set_End("Decrese_StickVolumeXM");
		easing::Set_End("Decrese_StickVolumeYP");
		easing::Set_End("Decrese_StickVolumeYM");
		easing::Set_End("Decrese_StickVolumeZP");
		easing::Set_End("Decrese_StickVolumeZM");

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


		//std::vector<After_Collision> Result;
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
			
			//回転をeasingで減速運動させるver0.3
			
			//入力されてる所のeasingデータをリセットさせる
			if (in1.LStick.axis.y > 0)
			{
				easing::Reset("Decrese_StickVolumeXM");
				easing::Start("Decrese_StickVolumeXM");
			}
			else if (in1.LStick.axis.y < 0)
			{
				easing::Reset("Decrese_StickVolumeXP");
				easing::Start("Decrese_StickVolumeXP");
			}
			if (in1.LStick.axis.x > 0)
			{
				easing::Reset("Decrese_StickVolumeYM");
				easing::Start("Decrese_StickVolumeYM");
			}
			else if (in1.LStick.axis.x < 0)
			{
				easing::Reset("Decrese_StickVolumeYP");
				easing::Start("Decrese_StickVolumeYP");
			}
			if (in1.R2.on)
			{
				easing::Reset("Decrese_StickVolumeZM");
				easing::Start("Decrese_StickVolumeZM");
			}
			else if (in1.L2.on)
			{
				easing::Reset("Decrese_StickVolumeZP");
				easing::Start("Decrese_StickVolumeZP");
			}

			//easingデータが回転量で更新される
			this->frame_QTxp = ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(-easing::GetPos("Decrese_StickVolumeXM")  / float(delicate)));
						
			this->frame_QTxm = ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(easing::GetPos("Decrese_StickVolumeXP") / float(delicate)));
			
			
			this->frame_QTym = ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(-easing::GetPos("Decrese_StickVolumeYM") / float(delicate)));

			this->frame_QTyp = ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(easing::GetPos("Decrese_StickVolumeYP") / float(delicate)));
						
			
			this->frame_QTzm = ML::QT(ML::Vec3(0, 0, 1), ML::ToRadian(-easing::GetPos("Decrese_StickVolumeZM") / float(delicate)));

			this->frame_QTzp = ML::QT(ML::Vec3(0, 0, 1), ML::ToRadian(easing::GetPos("Decrese_StickVolumeZP") / float(delicate)));
				
			

			//各タスクの回転量を更新
			core->UpDate_Quartanion(this->frame_QTxp * this->frame_QTxm * this->frame_QTyp * this->frame_QTym *this->frame_QTzm * this->frame_QTzp);
			for (auto m = map->begin(); m != map->end(); m++)
			{
				(*m)->UpDate_Quartanion(this->frame_QTxp * this->frame_QTxm * this->frame_QTyp* this->frame_QTym *this->frame_QTzm * this->frame_QTzp);
			}
			for (auto f = fence->begin(); f != fence->end(); f++)
			{
				(*f)->UpDate_Quartanion(this->frame_QTxp * this->frame_QTxm * this->frame_QTyp* this->frame_QTym *this->frame_QTzm * this->frame_QTzp);
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
			ge->collision_Result.clear();
			//ボールとマップのあたり判定
			if (ball != nullptr)
			{
				//コアとボールが接触していない時、引力をかける(バリアの外に出られなくようにする仕組み)
				if (!core->Core_Check_Hit(ball->Get_Pos(), ball->Get_Radious(), ball->Get_Speed()))
				{
					ML::Vec3 force = ge->Map_center - ball->Get_Pos();
					ball->Graviation_Pull(force.Normalize());
				}
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
			core->Get_Collision_Poligon(&ge->collision_Result);
			for (auto i = map->begin(); i != map->end(); i++)
			{
				(*i)->Get_Collision_Poligon(&ge->collision_Result);
			}
			for (auto i = fence->begin(); i != fence->end(); i++)
			{
				(*i)->Get_Collision_Poligon(&ge->collision_Result);
			}
			//ge->collision_Result = Result;

			//位置補正を仕掛ける
			for (auto p : ge->collision_Result)
			{
				if (p.collision_Flag)
				{
					ball->Fix_Position_for_Rotate(this->frame_QTxp * this->frame_QTxm * this->frame_QTyp* this->frame_QTym *this->frame_QTzm * this->frame_QTzp);
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