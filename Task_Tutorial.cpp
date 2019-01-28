//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Tutorial.h"
#include  "Task_Title.h"
#include "Task_UI.h"
#include "Task_Effect_Manager.h"
#include "Task_Physics_Manager.h"
#include "Task_MapCore.h"
#include "Task_MapFence.h"
#include "Task_MapSide.h"
#include "Task_Ball.h"
#include "Task_CameraMan.h"
#include "easing.h"

namespace  Tutorial
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imageName[0] = "Tutorial_Outline_Img";
		this->imageName[1] = "Tutorial_Control_Img";
		this->imageName[2] = "Tutorial_Obstacle_Img";
		this->Bg_Img = "Tutorial_BG";
		DG::Image_Create(this->imageName[0], "./data/image/Tutorial_Outline.png");//概要のテュートリアル
		DG::Image_Create(this->imageName[1], "./data/image/Tutorial_Control.png");//操作のテュートリアル
		DG::Image_Create(this->imageName[2], "./data/image/Tutorial_Obstacle.png");//障害物のテュートリアル
		DG::Image_Create(this->Bg_Img, "./data/image/background.jpg");

		this->guide_Img[0] = "page_Guide";
		this->guide_Img[1] = "press_S";

		DG::Image_Create(this->guide_Img[0], "./data/image/pageGuide.png");
		DG::Image_Create(this->guide_Img[1], "./data/image/pressS.png");

		this->bgmName = "tutorialBGM";
		DM::Sound_CreateStream(this->bgmName, "./data/sound/tutorial.wav");
	
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		for (int i = 0; i < 3; i++)
		{
			DG::Image_Erase(this->imageName[i]);
		}
		for (int i = 0; i < 2; i++)
		{
			DG::Image_Erase(this->guide_Img[i]);
		}
		DG::Image_Erase(this->Bg_Img);
		DM::Sound_Erase(this->bgmName);
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize(Tutorial_Column tc)
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化
		this->posy = 0;
		this->column = tc;

		this->countDown = 0;
		this->countDown_Flag = false;

		this->timeCnt = 990;//0~80ではない数値で初期化
		this->page_Change_Flag = false;
		this->page_Change_Speed = (int)ge->screenWidth / 80;

		this->vol = 1000;
		//マップの中心地
		ge->Map_center = ML::Vec3(1050, 50, 1050);
		//判定の結果をゼロクリア
		ge->collision_Result.clear();
		//外部からテュートリアル内容を読み込む処理
		switch(this->column)
		{
		case Tutorial_Column::OutLine:
			//未実装
			break;
		case Tutorial_Column::Control:
			//未実装
			break;
		case Tutorial_Column::Obstacle:
			//未実装
			break;
		}

		DM::Sound_Play(this->res->bgmName, true);
			
		DG::EffectState().param.bgColor = ML::Color(1, 0, 0, 0);
		//★タスクの生成
		//ui
		auto ui = UI::Object::Create(true);
		//カメラ
		auto camera = CameraMan::Object::Create(true);
		//ボール
		auto ball = Ball::Object::Create(true);
		//中心キューブ
		auto core = Map_Core::Object::Create(true,Difficult_Range::TUTORIAL);
		//面
		for (int i = 0; i < 6; i++)
		{
			auto side = Map_Side::Object::Create(true, i, Difficult_Range::TUTORIAL);
		}
		//角
		for (int f = 0; f < 12; f++)
		{
			auto fence = MapFence::Object::Create(true, f, Difficult_Range::TUTORIAL);
		}
		//マネージャ
		auto ma = Physics_Manager::Object::Create(true);
		//エフェクト
		auto effect = EffectManager::Object::Create(true);

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

		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			//★引き継ぎタスクの生成
			auto nextTask = Title::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		//カウントダウン
		if (this->Is_Count_Down())
		{
			this->countDown++;
		}
		//カウントダウンが終わったら自分を消滅
		if (this->Count_Down_Over())
		{
			this->Kill();
		}
		//時間上昇
		this->timeCnt += ge->g_Time.Delta_Time();
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{

	}

	void  Object::Render3D_L0()
	{
		
	}

	//-----------------------------------------------------------------------------------
	//ページ切り替え
	void Object::Page_Chage(bool page_Move_Right)
	{		
		if (this->Can_I_Change_the_Page())
		{
			return;
		}

		//右のページを見る
		if (page_Move_Right)
		{
			if (this->posx[2] <= 0)
			{
				return;
			}
			for (int i = 0; i < 3; i++)
			{
				this->posx[i] -= this->page_Change_Speed;
			}
		}
		//左のページを見る
		else
		{
			if (this->posx[0] >= 0)
			{
				return;
			}
			for (int i = 0; i < 3; i++)
			{
				this->posx[i] += this->page_Change_Speed;
			}
		}
		
	}

	//-----------------------------------------------------------------------------
	//自分消滅までのカウントダウン開始を確認
	bool Object::Is_Count_Down() const
	{
		return this->countDown_Flag;
	}

	//-------------------------------------------------------------------------------
	//カウントダウンが終わったのかを確認
	bool Object::Count_Down_Over() const
	{
		return (this->countDown > 60);
	}
	//----------------------------------------------------------------------------------
	//ページ切り替えが可能かを判断
	bool Object::Can_I_Change_the_Page() const
	{
		return (this->timeCnt > 80);
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
	Object::SP  Object::Create(bool  flagGameEnginePushBack_, Tutorial_Column tc)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
			}
			if (!ob->B_Initialize(tc)) {
				ob->Kill();//イニシャライズに失敗したらKill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize(Tutorial_Column tc)
	{
		return  this->Initialize(tc);
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