//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Title.h"
#include  "Task_UI.h"
#include  "Task_Game.h"
#include "Task_Tutorial.h"
#include  "easing.h"

namespace  Title
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->select_Guide_Image = "SGI";
		this->press_Any_Key_Image = "PAKI";
		this->start_Image = "SI";
		this->tutorial_Image = "TI";
		this->Title_Name_Image[0] = "Title_S";
		this->Title_Name_Image[1] = "Title_P";
		this->Title_Name_Image[2] = "Title_I";
		this->Title_Name_Image[3] = "Title_N";
		this->Title_Name_Image[4] = "Title_N";
		this->Title_Name_Image[5] = "Title_I";
		this->Title_Name_Image[6] = "Title_N";
		this->Title_Name_Image[7] = "Title_G";
		this->Title_Name_Image[8] = "Title_C";		
		this->Title_Name_Image[9] = "Title_U";
		this->Title_Name_Image[10] = "Title_B";
		this->Title_Name_Image[11] = "Title_E";
		this->BG_ImageName = "BG";
		this->Difficalt_Image[0] = "Easy";
		this->Difficalt_Image[1] = "Normal";
		this->Difficalt_Image[2] = "Hard";
		this->Column_Image[0] = "Outline";
		this->Column_Image[1] = "Control";
		this->Column_Image[2] = "Obstacle";
		this->bgm_Title = "BGM_Title";
		
		DG::Image_Create(this->select_Guide_Image, "./data/image/moveSelect.png");
		DG::Image_Create(this->press_Any_Key_Image, "./data/image/preaaAny.png");
		DG::Image_Create(this->start_Image, "./data/image/start.png");
		DG::Image_Create(this->tutorial_Image, "./data/image/tutorial.png");
		DG::Image_Create(this->BG_ImageName, "./data/image/background.jpg");
		DG::Image_Create(this->Title_Name_Image[0], "./data/image/S.png");
		DG::Image_Create(this->Title_Name_Image[1], "./data/image/P.png");
		DG::Image_Create(this->Title_Name_Image[2], "./data/image/I.png");
		DG::Image_Create(this->Title_Name_Image[3], "./data/image/N.png");
		DG::Image_Create(this->Title_Name_Image[4], "./data/image/N.png");
		DG::Image_Create(this->Title_Name_Image[5], "./data/image/I.png");
		DG::Image_Create(this->Title_Name_Image[6], "./data/image/N.png");
		DG::Image_Create(this->Title_Name_Image[7], "./data/image/G.png");
		DG::Image_Create(this->Title_Name_Image[8], "./data/image/C.png");
		DG::Image_Create(this->Title_Name_Image[9], "./data/image/U.png");
		DG::Image_Create(this->Title_Name_Image[10], "./data/image/B.png");
		DG::Image_Create(this->Title_Name_Image[11], "./data/image/E.png");
		DG::Image_Create(this->Difficalt_Image[0], "./data/image/Easy.png");
		DG::Image_Create(this->Difficalt_Image[1], "./data/image/Normal.png");
		DG::Image_Create(this->Difficalt_Image[2], "./data/image/Hard.png");
		DG::Image_Create(this->Column_Image[0], "./data/image/Outline.png");
		DG::Image_Create(this->Column_Image[1], "./data/image/Control.png");
		DG::Image_Create(this->Column_Image[2], "./data/image/Obstacle.png");
		//bgm初期化
		DM::Sound_CreateStream(this->bgm_Title, "./data/sound/title.wav");

		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->select_Guide_Image);
		DG::Image_Erase(this->press_Any_Key_Image);
		DG::Image_Erase(this->start_Image);
		DG::Image_Erase(this->tutorial_Image);
		DG::Image_Erase(this->BG_ImageName);
		for (int i = 0; i < 12; i++)
		{
			DG::Image_Erase(this->Title_Name_Image[i]);
		}
		for (int i = 0; i < 3; i++)
		{
			DG::Image_Erase(this->Difficalt_Image[i]);
			DG::Image_Erase(this->Column_Image[i]);
		}

		DM::Sound_Erase(this->bgm_Title);
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
		this->select_now = false;
		this->timeCnt = 0;
		this->next_Task_Index = { 0,0 };
		this->moving_Menu = 0;
		this->moving_Title_Name = 0;
		this->n = nowMenu::Start_Tutorial;

		for (int i = 0; i < 12; i++)
		{
			this->Title_Name[i].x = -140;
			this->Title_Name[i].y = -140;
		}

		//easing function set
		//SPINNING CUBE
				
		for (int j = 0; j < 12; j++)
		{
			easing::Set("Title_x" + to_string(j), easing::QUARTINOUT, -140.0f + (100.0f * j), 60.0f + (100.0f * j), 100.0f);
		}
		
		/*easing::Set("Title_1_x", easing::QUARTINOUT, 1280, 240, 110);
		easing::Set("Title_2_x", easing::QUARTINOUT, 1280, 380, 120);
		easing::Set("Title_3_x", easing::QUARTINOUT, 1280, 520, 130);
		easing::Set("Title_4_x", easing::QUARTINOUT, 1280, 660, 140);*/
		easing::Set("Title_0_y", easing::QUINTOUT, -140, 160, 100);

		
		/*easing::Set("Title_5_x", easing::QUARTINOUT, -140, 640, 180);
		easing::Set("Title_6_x", easing::QUARTINOUT, -140, 780, 170);
		easing::Set("Title_7_x", easing::QUARTINOUT, -140, 920, 160);
		easing::Set("Title_8_x", easing::QUARTINOUT, -140, 1060, 150);*/
		easing::Set("Title_1_y", easing::QUINTOUT, 720, 160, 150);

		//easing start
		for (int i = 0; i < 12; i++)
		{
			easing::Start("Title_x" + to_string(i));
		}
		easing::Start("Title_0_y");
		easing::Start("Title_1_y");


		//カメラの設定
		ge->camera[0] = MyPG::Camera::Create(
			ML::Vec3(1000.0f, 0.0f, 1000.0f),				//	ターゲット位置
			ML::Vec3(1000.0f, 1400.0f, -1000.0f),			//	カメラ位置
			ML::Vec3(0.0f, 1.0f, 0.0f),					//	カメラの上方向ベクトル
			ML::ToRadian(35), 10.0f, 4000.0f,	//	視野角・視野距離
			(float)ge->screenWidth / (float)ge->screenHeight);		//	画面比率		
		DG::EffectState().param.bgColor = ML::Color(1, 0, 0, 0);

		//bgm再生
		DM::Sound_Play(this->res->bgm_Title, true);
		//★タスクの生成
		auto ui = UI::Object::Create(true);

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->KillAll_G("UI");
		DM::Sound_Stop(this->res->bgm_Title);		

		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			//★引き継ぎタスクの生成
			if (this->next_Task_Index.x == -1)
			{
				//ゲームタスクに移る
				auto nextTask = Game::Object::Create(true, Difficult_Range(this->next_Task_Index.y));
			}
			else if (this->next_Task_Index.x == 1)
			{
				//テュートリアルタスクに移る
				auto nextTask = Tutorial::Object::Create(true,Tutorial_Column(this->next_Task_Index.y));
			}
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto in = DI::GPad_GetState("P1");
		easing::UpDate();
		this->UpDate_Title_Name();
		
		if (this->Press_Any_Key() && this->select_now == false)
		{
			auto cursor = Cursor::Object::Create(true);
			this->select_now = true;
		}

		auto cur = ge->GetTask_One_G<Cursor::Object>("カーソル");
		
		//カーソルが生きている間に
		if (cur != nullptr)
		{
			//現在メニューに対しての処理
			this->n = cur->Get_Now_Menu();
			switch(n)						
			{
			case nowMenu::Start_Tutorial:
				this->moving_Menu -= 22;
				if (this->moving_Menu < 0)
				{
					this->moving_Menu = 0;
				}
				break;
			case nowMenu::difficult:
			case nowMenu::TutorialCalum:
				this->moving_Menu += 22;
				if (this->moving_Menu > (int)ge->screenWidth)
				{
					this->moving_Menu = (int)ge->screenWidth;
				}
				break;
			}
			//メニューは関係なくカーソルが生きている間タイトル名は上に上がっている
			this->moving_Title_Name += 6;
		}

				
		this->timeCnt++;
		//カウンタの上限指定		
		
		if (this->moving_Title_Name > 160)
		{
			this->moving_Title_Name = 160;
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{		

		ML::Box2D draw_BG(0, 0, 1280, 720);
		ML::Box2D src_BG(0, 0, 1280, 960);
		DG::Image_Draw(this->res->BG_ImageName, draw_BG, src_BG);
		this->Draw_Title_Name();
		this->Draw_PAK();
		this->Draw_Menu();
		this->Draw_Dif_Col(this->n);
	}

	void  Object::Render3D_L0()
	{
		
	}
	//----------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------
	//追加メソッド
	//スティック以外のボタンを押すのかを判別
	bool Object::Press_Any_Key()
	{
		auto in1 = DI::GPad_GetState("P1");

		if (in1.B1.down || in1.B2.down || in1.B3.down || in1.B4.down ||
			in1.HD.down || in1.HL.down || in1.HR.down || in1.HU.down ||
			in1.L1.down || in1.L2.down || in1.L3.down ||
			in1.R1.down || in1.R2.down || in1.R3.down ||
			in1.SE.down || in1.ST.down)
		{
			return true;
		}

		return false;
	}
	//-------------------------------------------------------------------------------
	//タイトル名を表示
	void Object::Draw_Title_Name()
	{
		ML::Box2D draw(0, 0, 100, 100);
		ML::Box2D src(0, 0, 140, 140);

		draw.Offset(0, -this->moving_Title_Name);
		for (int i = 0; i < 12; i++)
		{
			DG::Image_Draw(this->res->Title_Name_Image[i], draw.OffsetCopy(this->Title_Name[i]), src);
		}
	}
	//-----------------------------------------------------------------------------------
	//タイトル名の位置を更新
	void Object::UpDate_Title_Name()
	{
		//X座標更新
		/*this->Title_Name[0].x = easing::GetPos("Title_0_x");
		this->Title_Name[1].x = easing::GetPos("Title_1_x");
		this->Title_Name[2].x = easing::GetPos("Title_2_x");
		this->Title_Name[3].x = easing::GetPos("Title_3_x");
		this->Title_Name[4].x = easing::GetPos("Title_4_x");
		this->Title_Name[5].x = easing::GetPos("Title_5_x");
		this->Title_Name[6].x = easing::GetPos("Title_6_x");
		this->Title_Name[7].x = easing::GetPos("Title_7_x");
		this->Title_Name[8].x = easing::GetPos("Title_8_x");*/

		for (int i = 0; i < 12; i++)
		{
			this->Title_Name[i].x = easing::GetPos("Title_x" + to_string(i));
		}

		for (int i = 0; i < 12; i++)
		{			
			switch (i%2)
			{
			case 0:
				this->Title_Name[i].y = easing::GetPos("Title_0_y");
				break;
			case 1:
				this->Title_Name[i].y = easing::GetPos("Title_1_y");
				break;
			}
			
		}		
	}
	//----------------------------------------------------------------------------------
	//press any key を表示
	void Object::Draw_PAK()
	{
		if (this->timeCnt < 200 || this->select_now)
		{
			return;
		}
		ML::Box2D draw(250, 500, 750, 100);
		ML::Box2D src(0, 0, 900, 140);

		DG::Image_Draw(this->res->press_Any_Key_Image, draw, src);
	}

	//---------------------------------------------------------------------
	//メニューの表示
	void Object::Draw_Menu()
	{
		if(!this->select_now)
		{
			return;
		}

		ML::Box2D draw_Start(400, 310, 380, 100);
		ML::Box2D src_Start(0, 0, 420, 140);
		ML::Box2D draw_Tutorial(400, 400, 520, 100);
		ML::Box2D src_Tutorial(0, 0, 560, 140);		

		ML::Box2D draw_Guide(350, 660, 550, 50);
		ML::Box2D src_Guide(0, 0, 2200, 140);

		draw_Start.Offset(-this->moving_Menu, 0);
		draw_Tutorial.Offset(-this->moving_Menu, 0);

		DG::Image_Draw(this->res->start_Image, draw_Start, src_Start);
		DG::Image_Draw(this->res->tutorial_Image, draw_Tutorial, src_Tutorial);
		DG::Image_Draw(this->res->select_Guide_Image, draw_Guide, src_Guide);
	}

	//-----------------------------------------------------------------------------
	//難易度とテュートリアル目次の表示
	void Object::Draw_Dif_Col(nowMenu now)
	{
		//表示範囲は画像を作った後に変更する(2018/05/04)
		ML::Box2D draw_Dif_Col0(400 + ge->screen2DWidth, 310, 380, 100);//Easy and OutLine
		ML::Box2D draw_Dif_Col1(400 + ge->screen2DWidth, 400, 380, 100);//Normal and Control
		ML::Box2D draw_Dif_Col2(400 + ge->screen2DWidth, 490, 380, 100);//Hard and Obstacle

		//仮のsrc
		ML::Box2D src_Dif_col0(0, 0, 700, 140);

		draw_Dif_Col0.Offset(-this->moving_Menu, 0);
		draw_Dif_Col1.Offset(-this->moving_Menu, 0);
		draw_Dif_Col2.Offset(-this->moving_Menu, 0);
		switch (now)
		{
		case nowMenu::difficult:
			DG::Image_Draw(this->res->Difficalt_Image[0], draw_Dif_Col0, src_Dif_col0);
			DG::Image_Draw(this->res->Difficalt_Image[1], draw_Dif_Col1, src_Dif_col0);
			DG::Image_Draw(this->res->Difficalt_Image[2], draw_Dif_Col2, src_Dif_col0);
			break;
		case nowMenu::TutorialCalum:
			DG::Image_Draw(this->res->Column_Image[0], draw_Dif_Col0, src_Dif_col0);
			DG::Image_Draw(this->res->Column_Image[1], draw_Dif_Col1, src_Dif_col0);
			DG::Image_Draw(this->res->Column_Image[2], draw_Dif_Col2, src_Dif_col0);
			break;
		}
	}

	//----------------------------------------------------------------------------
	//次のタスクを決める関数
	void Object::I_Select(POINT select)
	{
		//-1ならゲーム本編
		//1ならテュートリアル
		this->next_Task_Index = select;

		this->Kill();
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