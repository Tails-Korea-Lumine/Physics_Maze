//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Game.h"
#include  "Task_title.h"
#include  "Task_MapSide.h"
#include  "Task_MapCore.h"
#include  "Task_Ball.h"
#include  "Task_Player.h"
#include  "Task_MapFence.h"
#include  "Task_Map_Manager.h"
#include  "Task_Result.h"
#include  "Task_UI.h"

namespace  Game
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->bgmName = "stageBgm";
		DM::Sound_CreateStream(this->bgmName, "./data/sound/stage.wav");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DM::Sound_Erase(this->bgmName);
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

		ge->gameClearFlag = false;

		//カメラの設定
		ge->camera[0] = MyPG::Camera::Create(
			ML::Vec3(1000.0f, 0.0f, 1000.0f),				//	ターゲット位置
			ML::Vec3(1000.0f, 1400.0f, -1000.0f),			//	カメラ位置
			ML::Vec3(0.0f, 1.0f, 0.0f),					//	カメラの上方向ベクトル
			ML::ToRadian(35), 10.0f, 4000.0f,	//	視野角・視野距離
			(float)ge->screenWidth / (float)ge->screenHeight);		//	画面比率		
		DG::EffectState().param.bgColor = ML::Color(0.5f, 0.0f, 0.4f, 0.0f);
		//ライティング有効化
		DG::EffectState().param.lightsEnable = true;
		//環境光の強さを設定する
		DG::EffectState().param.lightAmbient = ML::Color(1, 0.3f, 0.3f, 0.3f);
		//平行光源の設定
		DG::EffectState().param.light[0].enable = true;
		DG::EffectState().param.light[0].kind = DG_::Light::Directional;//光源の種類
		DG::EffectState().param.light[0].direction = ML::Vec3(1, -1, 1).Normalize();//照射方向
		DG::EffectState().param.light[0].color = ML::Color(1, 0.89f, 0.89f, 0.89f);//色と強さ

		//デバッグ用の文字生成
		//DG::Font_Create("FontA", "HGSｺﾞｼｯｸM", 12, 16);

		//bgm再生
		DM::Sound_Play(this->res->bgmName, true);

		//ワールド回転量初期化
		ge->World_Rotation = ML::QT();

		//★タスクの生成
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
		auto manager = Map_Manager::Object::Create(true);
		
		//UIの生成
		auto ui = UI::Object::Create(true);

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

		//DG::Font_Erase("FontA");

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
		//auto in = DI::GPad_GetState("P1");

		if (this->IS_Cleared())
		{
			ge->GetTask_One_G<UI::Object>("UI")->Start_WipeIn();
			this->countdownFlag = true;
		}

		//カウントダウン
		if (this->Is_Count_Down())
		{
			this->countdown++;
		}
		//1秒後にタスク消滅
		if (this->countdown > 60)
		{
			this->Kill();
		}
		//時間を更新
		this->timeCnt++;
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
		DG::Font_Draw("FontA", moji, buf, ML::Color(1, 1, 0, 1));*/
	}

	void  Object::Render3D_L0()
	{
		
	}

	//--------------------------------------------------------------------------------------
	//追加メソッド
	//カウントダウンフラグを返す関数
	bool Object::Is_Count_Down()
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
	bool Object::IS_Cleared()
	{
		return ge->gameClearFlag;
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