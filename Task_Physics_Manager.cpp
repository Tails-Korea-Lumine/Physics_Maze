//-------------------------------------------------------------------
//マップマネージャ
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Physics_Manager.h"
#include "easing.h"
#include "MyMath.h"

#define PRECISION 3

namespace  Physics_Manager
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
		

		this->frame_QTxp = ML::QT(0.0f);
		this->frame_QTxm = ML::QT(0.0f);
		this->frame_QTyp = ML::QT(0.0f);
		this->frame_QTym = ML::QT(0.0f);
		this->frame_QTzp = ML::QT(0.0f);
		this->frame_QTzm = ML::QT(0.0f);

		//軸の初期値
		this->anckerX = ML::Vec3(1, 0, 0);
		this->anckerY = ML::Vec3(0, 1, 0);
		this->anckerZ = ML::Vec3(0, 0, 1);
	
		//easing set
		easing::Set("Decrese_StickVolumeXP", easing::QUARTIN, 1.7f, 0, 7);
		easing::Set("Decrese_StickVolumeXM", easing::QUARTIN, 1.7f, 0, 7);
		easing::Set("Decrese_StickVolumeYP", easing::QUARTIN, 1.7f, 0, 7);
		easing::Set("Decrese_StickVolumeYM", easing::QUARTIN, 1.7f, 0, 7);
		easing::Set("Decrese_StickVolumeZP", easing::QUARTIN, 1.7f, 0, 7);
		easing::Set("Decrese_StickVolumeZM", easing::QUARTIN, 1.7f, 0, 7);
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
		this->Managing(PRECISION);
		
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
	void Object::Managing(const unsigned int precision)
	{
		
		auto in1 = DI::GPad_GetState("P1");
		//ほかのタスクの情報をもらってくる
		auto map = ge->GetTask_Group_GN<Map3d::Object>("マップ", "Side");
		auto fence = ge->GetTask_Group_GN<MapFence::Object>("マップ", "Fence");
		auto core = ge->GetTask_One_G<Map_Core::Object>("マップ");
		auto ball = ge->GetTask_One_G<Ball::Object>("ボール");
		auto cm = ge->GetTask_One_G<CameraMan::Object>("カメラマン");

		//ボールが存在しない場合処理しない
		if (ball == nullptr)
		{
			return;
		}
		//カメラ移動中なら処理しない
		if (cm->Is_Moving_Now() == true)
		{
			return;
		}

		//ボールが存在している面の法線ベクトル
		ML::Vec3 side_Normal(0, 0, 0);
		float delicate = 0.0f;
		//微調整機能
		if (in1.B2.on)
		{
			delicate = (float)precision * 2.0f;
		}
		else
		{
			delicate = (float)precision;
		}

		//ボールの全ドットを格納する場所
		std::vector<ML::Vec3> all_Points;

		//std::vector<After_Collision> Result;
		for (unsigned int i = 0; i < precision; i++)
		{			
			//回転をeasingで減速運動させるver0.3
			
			if (ge->game.lock()->GET_READY())
			{
				//入力されてる所のeasingデータをリセットさせる
				if (in1.LStick.axis.y > 0)
				{					
					easing::Re_Start("Decrese_StickVolumeXM");
				}
				else if (in1.LStick.axis.y < 0)
				{
					easing::Re_Start("Decrese_StickVolumeXP");
				}
				if (in1.LStick.axis.x > 0)
				{
					easing::Re_Start("Decrese_StickVolumeYM");
				}
				else if (in1.LStick.axis.x < 0)
				{
					easing::Re_Start("Decrese_StickVolumeYP");
				}
				if (in1.Triger.axis.x <0)
				{
					easing::Re_Start("Decrese_StickVolumeZM");
				}
				else if (in1.Triger.axis.x >0)
				{
					easing::Re_Start("Decrese_StickVolumeZP");
				}
			}
			//easingデータが回転量で更新される
			this->frame_QTxp = ML::QT(this->anckerX, ML::ToRadian(-(easing::GetPos("Decrese_StickVolumeXM"))  / delicate));
						
			this->frame_QTxm = ML::QT(this->anckerX, ML::ToRadian((easing::GetPos("Decrese_StickVolumeXP")) / delicate));
			
			
			this->frame_QTym = ML::QT(this->anckerY, ML::ToRadian(-(easing::GetPos("Decrese_StickVolumeYM")) / delicate));

			this->frame_QTyp = ML::QT(this->anckerY, ML::ToRadian((easing::GetPos("Decrese_StickVolumeYP")) / delicate));
						
			
			this->frame_QTzm = ML::QT(this->anckerZ, ML::ToRadian(-(easing::GetPos("Decrese_StickVolumeZM")) / delicate));

			this->frame_QTzp = ML::QT(this->anckerZ, ML::ToRadian((easing::GetPos("Decrese_StickVolumeZP")) / delicate));

			//フレーム回転量のまとめ				
			ML::QT frame_QT_All = this->frame_QTxp * this->frame_QTxm * this->frame_QTyp * this->frame_QTym *this->frame_QTzm * this->frame_QTzp;

			//各タスクの回転量を更新及び回転
			core->Rotate_Core_and_Barrier(frame_QT_All);
			for (auto m = map->begin(); m != map->end(); m++)
			{
				(*m)->Map_Rotate(frame_QT_All);
			}
			for (auto f = fence->begin(); f != fence->end(); f++)
			{
				(*f)->Map_Rotate(frame_QT_All);
			}
			
			//ボールの外角ドットを取り出しておく
			all_Points.clear();
			ball->Get_Poionts_to_Sphere(&all_Points);

			//あたり判定は毎回マップのほうで行う	
			ge->collision_Result.clear();					
			
			
			//ボールとマップのあたり判定及び保存
			//コア
			core->Core_Check_Hit(all_Points, ball->Get_Pos(), ball->Get_Radious(), ball->Get_Speed());
			//面
			for (auto m = map->begin(); m != map->end(); m++)
			{
				//ボールが存在しているマップの法線ベクトルを収得
				if ((*m)->Map_Check_Hit(all_Points, ball->Get_Pos(), ball->Get_Radious(), ball->Get_Speed()))
				{
					(*m)->Get_Normal_Side(&side_Normal);
				}
			}
			//フェンス
			for (auto f = fence->begin(); f != fence->end(); f++)
			{
				(*f)->Map_Check_Hit(all_Points, ball->Get_Pos(), ball->Get_Radious(), ball->Get_Speed());
			}		
			
			//判定結果に無駄なデータが入っているなら取り除く
			ge->collision_Result.erase(remove_if(ge->collision_Result.begin(), ge->collision_Result.end(), [](const After_Collision& c) {return !c.collision_Flag; })
				,ge->collision_Result.end());

			//ボールを移動させる
			ball->Move_Ball(precision);
			//位置補正を仕掛ける
			if (ge->collision_Result.size() != 0)
			{
				ball->Fix_Position_for_Rotate(frame_QT_All);
			}			
		}

		
		//カメラ目的地をボールがある面が見えるように設定
		if (side_Normal.Is_Zero_Vec() == false && (in1.B4.down || ball->Is_Teleport_Now()))
		{
			cm->Set_Destination(side_Normal);
		}
	}
	//------------------------------------------------------------------------
	//回転軸再計算
	void Object::Ancker_Calculating()
	{
		//カメラからマップ中心のベクトルが新しいZ回転軸
		ML::Vec3 NEW_AnckerZ = ge->Map_center - ge->camera[0]->pos;

		//新、旧Z回転軸の法線ベクトルでまだ分からない回転軸をもう1個計算
		ML::Vec3 NEW_AnckerY, NEW_AnckerX;
		//カメラの上方向ベクトルが新しいY回転軸
		NEW_AnckerY = ge->camera[0]->up;
		//X回転軸は新しい二つの軸の法線
		MyMath::Get_Normal_to_Vector_Cross(&NEW_AnckerX, NEW_AnckerY, NEW_AnckerZ);

		this->anckerX = NEW_AnckerX.Normalize();
		this->anckerY = NEW_AnckerY.Normalize();
		this->anckerZ = NEW_AnckerZ.Normalize();
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