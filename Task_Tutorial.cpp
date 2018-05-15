//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Tutorial.h"
#include  "Task_Title.h"
#include  "easing.h"

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

		this->timeCnt = 990;//0~80ではない数値で初期化
		this->page_Change_Flag = false;

		switch(this->column)
		{
		case Tutorial_Column::OutLine:
			for (int i = 0; i < 3; i++)
			{
				this->posx[i] = ge->screenWidth * i;
				//easing::Set("Change_Page" + to_string(i), easing::CIRCIN, ge->screenWidth*i, ge->screenWidth*(i+1), 80);
			}
			break;
		case Tutorial_Column::Control:
			for (int i = 0; i < 3; i++)
			{
				this->posx[i] = ge->screenWidth * (i-1);
				//easing::Set("Change_Page" + to_string(i), easing::CIRCIN, ge->screenWidth*(i-1), ge->screenWidth*(i), 80);
			}
			break;
		case Tutorial_Column::Obstacle:
			for (int i = 0; i < 3; i++)
			{
				this->posx[i] = ge->screenWidth * (i-2);
				//easing::Set("Change_Page" + to_string(i), easing::CIRCIN, ge->screenWidth*(i-2), ge->screenWidth*(i - 1), 80);
			}
			break;
		}

		DM::Sound_Play(this->res->bgmName, true);
			
		DG::EffectState().param.bgColor = ML::Color(1, 0, 0, 0);
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
			auto nextTask = Title::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto in = DI::GPad_GetState("P1");

		if (in.LStick.D.on || in.HD.on)
		{
			this->posy -= 2;
		}
		if (in.LStick.U.on || in.HU.on)
		{
			this->posy += 2;
		}		

		if (in.LStick.L.down || in.HL.down)
		{
			if (this->Can_I_Change_the_Page())
			{
				this->page_Change_Flag = false;
				this->timeCnt = 0;
				this->posy = 0;
			}
		}
		else if (in.LStick.R.down || in.HR.down)
		{
			if (this->Can_I_Change_the_Page())
			{
				this->page_Change_Flag = true;
				this->timeCnt = 0;
				this->posy = 0;
			}
		}

		this->Page_Chage(this->page_Change_Flag);

		//posyの範囲設定
		if (this->posy < -528)
		{
			this->posy = -528;
		}
		if (this->posy > 0)
		{
			this->posy = 0;
		}		

		if (in.ST.down)
		{
			if (in.ST.down)
			{
				//自身に消滅要請
				this->Kill();
			}
		}
		this->timeCnt++;
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D drawBG(0, 0, 1280, 720);
		ML::Box2D srcBG(0, 0, 1280, 960);

		DG::Image_Draw(this->res->Bg_Img, drawBG, srcBG);

		ML::Box2D draw[3] = 
		{
			{0, 0, 1280, 1248},
			{ 0, 0, 1280, 1248 },
			{ 0, 0, 1280, 1248 },		
		};
		ML::Box2D src(0, 0,1280, 1248);
		for (int i = 0; i < 3; i++)
		{
			draw[i].Offset(this->posx[i], this->posy);
			DG::Image_Draw(this->res->imageName[i], draw[i], src);
		}
		
		/*DG::Image_Draw(this->res->imageName[1], draw, src);
		DG::Image_Draw(this->res->imageName[2], draw, src);*/
		
		
	}

	void  Object::Render3D_L0()
	{
		
	}

	//-----------------------------------------------------------------------------------
	//ページ切り替え
	void Object::Page_Chage(bool page_Move_Right)
	{
		if (this->timeCnt > 80 )
		{
			return;
		}

		//右のページを見る
		if (page_Move_Right)
		{
			if (this->posx[2] == 0)
			{
				return;
			}
			for (int i = 0; i < 3; i++)
			{
				this->posx[i] -= 16;
			}
		}
		//左のページを見る
		else
		{
			if (this->posx[0] == 0)
			{
				return;
			}
			for (int i = 0; i < 3; i++)
			{
				this->posx[i] += 16;
			}
		}
		
	}

	//----------------------------------------------------------------------------------
	//ページ切り替えが可能かを判断
	bool Object::Can_I_Change_the_Page()
	{
		return (this->timeCnt > 80);
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