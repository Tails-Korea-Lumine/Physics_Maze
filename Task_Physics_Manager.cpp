//-------------------------------------------------------------------
//マップマネージャ
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Physics_Manager.h"
#include "easing.h"
#include "MyMath.h"
#include  "Task_CameraMan.h"
#include  "Task_MapCore.h"
#include  "Task_MapSide.h"
#include  "Task_MapFence.h"
#include  "Task_Ball.h"


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
		

		this->esasing_Name_QTxp = "Decrese_StickVolumeXP";
		this->esasing_Name_QTxm = "Decrese_StickVolumeXM";
		this->esasing_Name_QTyp = "Decrese_StickVolumeYP";
		this->esasing_Name_QTym = "Decrese_StickVolumeYM";
		this->esasing_Name_QTzp = "Decrese_StickVolumeZP";
		this->esasing_Name_QTzm = "Decrese_StickVolumeZM";

		//軸の初期値
		this->anckerX = ML::Vec3(1, 0, 0);
		this->anckerY = ML::Vec3(0, 1, 0);
		this->anckerZ = ML::Vec3(0, 0, 1);
	
		//easing set
		easing::Set(this->esasing_Name_QTxp, easing::QUARTIN, 1.7f, 0, 7);
		easing::Set(this->esasing_Name_QTxm, easing::QUARTIN, 1.7f, 0, 7);
		easing::Set(this->esasing_Name_QTyp, easing::QUARTIN, 1.7f, 0, 7);
		easing::Set(this->esasing_Name_QTym, easing::QUARTIN, 1.7f, 0, 7);
		easing::Set(this->esasing_Name_QTzp, easing::QUARTIN, 1.7f, 0, 7);
		easing::Set(this->esasing_Name_QTzm, easing::QUARTIN, 1.7f, 0, 7);
		//easing set end						 
		easing::Set_End(this->esasing_Name_QTxp);
		easing::Set_End(this->esasing_Name_QTxm);
		easing::Set_End(this->esasing_Name_QTyp);
		easing::Set_End(this->esasing_Name_QTym);
		easing::Set_End(this->esasing_Name_QTzp);
		easing::Set_End(this->esasing_Name_QTzm);

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
		this->Managing(PRECISION);

		this->Input_Analog_Action();
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
		auto map = ge->GetTask_Group_GN<Map_Side::Object>("マップ", "Side");
		auto fence = ge->GetTask_Group_GN<MapFence::Object>("マップ", "Fence");
		auto core = ge->GetTask_One_G<Map_Core::Object>("マップ");
		auto ball = ge->GetTask_One_G<Ball::Object>("ボール");
		auto cm = ge->GetTask_One_G<CameraMan::Object>("カメラマン");

		//ボールが存在しない場合かカメラ移動中なら処理しない
		if (ball == nullptr || cm->Is_Moving_Now() == true)
		{
			return;
		}

		//ボールが存在している面の法線ベクトル
		std::vector<ML::Vec3> side_Normals;
		float delicate = 0.0f;
		//微調整機能		
		delicate = in1.B2.on ? (float)precision *2.0f : (float)precision;

		for (unsigned int i = 0; i < precision; i++)
		{			
			//フレーム回転量のまとめ				
			ML::QT frame_QT_All = this->Calculate_Frame_Quatanion(delicate);
			//法線ベクトル保存場所クリア
			side_Normals.clear();
			//各タスクの回転量を更新及び回転
			core->Rotate_Core_and_Barrier(frame_QT_All);
			for (auto m = map->begin(); m != map->end(); m++)
			{
				(*m)->Map_Rotate(frame_QT_All);
				//マップの法線ベクトルを収得
				side_Normals.push_back((*m)->Get_Normal_Side());
			}
			for (auto f = fence->begin(); f != fence->end(); f++)
			{
				(*f)->Map_Rotate(frame_QT_All);
			}
			//位置補正を仕掛ける
			ball->Fix_Position_for_Rotate(frame_QT_All);

			ge->collision_Result.clear();
		
			//ボールとマップのあたり判定及び保存
			//コア
			core->Core_Check_Hit(ball->Get_Collision_Area());
			//面
			for (auto m = map->begin(); m != map->end(); m++)
			{				
				(*m)->Map_Check_Hit(ball->Get_Collision_Area());
			}
			//フェンス
			for (auto f = fence->begin(); f != fence->end(); f++)
			{
				(*f)->Map_Check_Hit(ball->Get_Collision_Area());
			}		
			
			//判定結果に無駄なデータが入っているなら取り除く			
			ge->collision_Result.remove_if([](const Collision_Data& c) {return !c.collision_Flag; });
			
			//ボールを移動させる
			ball->Physics_Actioin(precision);
		}
		
		//カメラ目的地をボールがある面が見えるように設定
		if (in1.B4.down || ball->Is_Teleport_Now())
		{
			//法線ベクトル選択
			ML::Vec3 side_Normal = this->Select_Nearest_Side(side_Normals, (ball->Get_Pos() - ge->Map_center));
			if (!side_Normal.Is_Zero_Vec())
			{
				//ゼロベクトルでなければカメラを移動させる
				cm->Set_Destination(side_Normal);
			}
		}
	}
	//カメラ移動の法線ベクトル選択処理
	ML::Vec3 Object::Select_Nearest_Side(const std::vector<ML::Vec3>& normals, const ML::Vec3& b)
	{
		//全体中心からボールまでのベクトルと法線ベクトルたちを内積させる
		float cos[6];		
		for (size_t i =0; i<normals.size(); i++)
		{
			MyMath::Vector_Dot(&cos[i], normals[i], b);
		}

		//最大値を取る
		size_t max_Num = 0;

		for (size_t i = 1; i < 6; i++)
		{
			if (cos[max_Num] < cos[i])
			{
				max_Num = i;
			}
		}

		return normals.at(max_Num);
	}

	//----------------------------------------------------------------------
	//入力による処理
	void Object::Input_Analog_Action()
	{
		auto in1 = DI::GPad_GetState("P1");
		if (ge->game.lock()->GET_READY())
		{
			//入力されてる所のeasingデータをリセットさせる
			if (in1.LStick.axis.y > 0.1f)
			{
				easing::Re_Start(this->esasing_Name_QTxm);
			}
			else if (in1.LStick.axis.y < -0.1f)
			{
				easing::Re_Start(this->esasing_Name_QTxp);
			}
			if (in1.LStick.axis.x > 0.1f)
			{
				easing::Re_Start(this->esasing_Name_QTym);
			}
			else if (in1.LStick.axis.x < -0.1f)
			{
				easing::Re_Start(this->esasing_Name_QTyp);
			}
			if (in1.Triger.axis.x <-0.1f)
			{
				easing::Re_Start(this->esasing_Name_QTzm);
			}
			else if (in1.Triger.axis.x >0.1f)
			{
				easing::Re_Start(this->esasing_Name_QTzp);
			}
		}
	}
	//----------------------------------------------------------------------
	//クォータニオン計算
	ML::QT Object::Calculate_Frame_Quatanion(const float& precision)
	{
		//easingデータが回転量で更新される
		ML::QT frame_QTxm = ML::QT(this->anckerX, ML::ToRadian(-(easing::GetPos(this->esasing_Name_QTxm)) / precision));

		ML::QT frame_QTxp = ML::QT(this->anckerX, ML::ToRadian((easing::GetPos(this->esasing_Name_QTxp)) / precision));


		ML::QT frame_QTym = ML::QT(this->anckerY, ML::ToRadian(-(easing::GetPos(this->esasing_Name_QTym)) / precision));

		ML::QT frame_QTyp = ML::QT(this->anckerY, ML::ToRadian((easing::GetPos(this->esasing_Name_QTyp)) / precision));


		ML::QT frame_QTzm = ML::QT(this->anckerZ, ML::ToRadian(-(easing::GetPos(this->esasing_Name_QTzm)) / precision));

		ML::QT frame_QTzp = ML::QT(this->anckerZ, ML::ToRadian((easing::GetPos(this->esasing_Name_QTzp)) / precision));

		return frame_QTxp *  frame_QTxm *  frame_QTyp *  frame_QTym * frame_QTzm *  frame_QTzp;
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