//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Game.h"
#include  "Task_title.h"
#include  "Task_MapSide.h"
#include  "Task_MapCore.h"
#include  "Task_Ball.h"
#include  "Task_MapFence.h"
#include  "Task_Physics_Manager.h"
#include  "Task_Result.h"
#include  "Task_UI.h"
#include  "Task_CameraMan.h"
#include "easing.h"

namespace  Game
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->bgmName = "stageBgm";
		this->BG_mesh = "under the sea";
		DM::Sound_CreateStream(this->bgmName, "./data/sound/stage.wav");
		DG::Mesh_CreateFromSOBFile(this->BG_mesh, "./data/mesh/under_the_sea.SOB");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DM::Sound_Erase(this->bgmName);
		DG::Mesh_Erase(this->BG_mesh);
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize(Difficult_Range di)
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化
		this->countdown = 0;
		this->countdownFlag = false;
		this->timeCnt = 0;
		this->nowdi = di;
		this->render2D_Priority[1] = 1.0f;
		ge->gameClearFlag = false;
		ge->getReadyFlag = true;
		this->vol = 1000;

		//★タスクの生成
		//UIの生成
		auto ui = UI::Object::Create(true);

		//カメラマンを生成
		auto cameraman = CameraMan::Object::Create(true);
		
		//デバッグ用の文字生成
		//DG::Font_Create("FontA", "HGSｺﾞｼｯｸM", 12, 16);

		//bgm再生
		DM::Sound_Play(this->res->bgmName, true);

		
		//背景の角度を初期化
		this->angleY = 0.0f;

		//マップの中心地
		ge->Map_center = ML::Vec3(1050, 50, 1050);
		//判定の結果をゼロクリア
		ge->collision_Result.clear();

		//ボールの生成
		auto ball = Ball::Object::Create(true);
		//中心の元なるキューブ
		auto core = Map_Core::Object::Create(true , di);
		//マップ生成
		for (int i = 0; i < 6; i++)
		{
			auto map = Map3d::Object::Create(true, i,di);
		}
		//フェンス生成
		for (int f = 0; f < 12; f++)
		{
			auto fence = MapFence::Object::Create(true, f,di);
		}		

		//マップマネージャ生成
		auto manager = Physics_Manager::Object::Create(true);

		//エフェクトマネージャー生成
		ge->eff_Manager = EffectManager::Object::Create(true);
		

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->KillAll_G("マップ");
		ge->KillAll_G("ボール");
		ge->KillAll_G("UI");
		ge->KillAll_G("カメラマン");
		ge->KillAll_G("エフェクト");
		ge->KillAll_G("物理運動");
	//	DG::Font_Erase("FontA");

		DM::Sound_Stop(this->res->bgmName);

		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			//★引き継ぎタスクの生成
			
			auto nextTask = Result::Object::Create(true,this->timeCnt,this->nowdi);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto in = DI::GPad_GetState("P1");

		if (this->Start_Clear_Promotion())
		{
			ge->KillAll_G("ボール");
			ge->GetTask_One_G<UI::Object>("UI")->Start_WipeIn();
			this->countdownFlag = true;
		}

		//強制終了
		if ((in.ST.on && in.SE.on) && ge->getReadyFlag == false)
		{
			this->Game_Clear();

			this->timeCnt = 0xffff;
		}

		//カウントダウン
		if (this->Is_Count_Down())
		{
			this->countdown++;
			this->BGM_Fade_Out();
		}
		//1秒後にタスク消滅
		if (this->Count_Down_Over())
		{
			this->Kill();
		}		
		ge->TM.Increse_Counter();

		//始まる前の演出が終わったら準備中フラグを無効にする
		if (!this->GET_READY())
		{
			if (this->Is_Camera_Promotion_Over())
			{
				ge->getReadyFlag = false;
				this->timeCnt = 0;
			}
		}
		//時間を更新
		this->timeCnt++;
		//背景角度更新
		this->angleY += 0.04f;
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		/*auto ball = ge->GetTask_One_G<Ball::Object>("ボール");
		if (ball == nullptr)
		{
			return;
		}
		char buf[1024];
		sprintf(buf, "pos : %4.3f , %4.3f , %4.3f \n"
			"speed : %4.3f , %4.3f , %4.3f \n"
			"WorldR : %4.3f , %4.3f , %4.3f",
			 ball->Get_Pos().x, ball->Get_Pos().y, ball->Get_Pos().z, ball->Get_Speed().x, ball->Get_Speed().y, ball->Get_Speed().z,
			ge->World_Rotation.x, ge->World_Rotation.y, ge->World_Rotation.z);

		ML::Box2D moji(100, 0, 600, 600);
		DG::Font_Draw("FontA", moji, buf, ML::Color(1, 1, 0, 1));

		ML::Box2D draw(0, 0, ge->screenWidth, ge->screenHeight);
		ML::Box2D src(0, 0, 960, 539);*/

		//DG::Image_Draw(this->res->BG_Image, draw, src);
	}

	void  Object::Render3D_L0()
	{
		//海の中をレンダリング
		ML::Mat4x4 matS, matR, matT;
		
		matS.Scaling(200.0f);
		matR.RotationY(ML::ToRadian(this->angleY));
		matT.Translation(ge->camera[0]->pos);		

		DG::EffectState().param.matWorld = matS * matR * matT;
		DG::EffectState().param.lightsEnable = false;
		if (DG::EffectState().param.light[0].enable)
		{
			DG::Mesh_Draw(this->res->BG_mesh);
		}

		DG::EffectState().param.lightsEnable = true;
	}

	//--------------------------------------------------------------------------------------
	//追加メソッド
	//カウントダウンフラグを返す関数
	bool Object::Is_Count_Down() const
	{
		return this->countdownFlag;
	}

	//------------------------------------------------------------------------------------
	//クリアフラグを立てる
	void Object::Game_Clear()
	{
		ge->gameClearFlag = true;
	}

	//---------------------------------------------------------------------------------------
	//クリアしたのかを確認
	bool Object::IS_Cleared() const
	{
		return ge->gameClearFlag;
	}

	//--------------------------------------------------------------------------------------
	//操作可能かを確認
	bool Object::GET_READY() const
	{
		return !ge->getReadyFlag;
	}
	//--------------------------------------------------------------------------
	//画面切り替えが終わったのかを確認
	bool Object::Count_Down_Over() const
	{
		return (this->countdown > 130);
	}
	//---------------------------------------------------------------------------
	//ゲームクリア演出を始める
	bool Object::Start_Clear_Promotion()
	{
		return (this->IS_Cleared() && this->countdown == 0);
	}

	//------------------------------------------------------------------------------
	//カメラ演出が終わったのかを確認
	bool Object::Is_Camera_Promotion_Over()
	{
		return this->timeCnt > 420;
	}
	//--------------------------------------------------------------------------------
	//BGM fade out
	void Object::BGM_Fade_Out()
	{
		this->vol -= 5;
		DM::Sound_Volume(this->res->bgmName, this->vol);
	}
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_, Difficult_Range di)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
			}
			if (!ob->B_Initialize(di)) {
				ob->Kill();//イニシャライズに失敗したらKill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize(Difficult_Range di)
	{
		return  this->Initialize(di);
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