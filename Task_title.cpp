//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Title.h"
#include  "Task_UI.h"
#include  "Task_Game.h"
#include "Task_Tutorial.h"
#include  "easing.h"

#define MOVING_TITLE_NAME_MAX 110
#define MOVING_TITLE_NAME_SPEED 6

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
		this->waveB = 1.0f;
		this->waveG = 1.0f;
		this->waveR = 1.0f;
		this->dgreeB = 0.0f;
		this->dgreeG = 45.0f;
		this->dgreeR = 90.0f;
		this->vol = 1000;

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
			easing::Set("Title_x" + to_string(j), easing::QUARTINOUT, -140.0f + (100.0f * j), 180.0f + (130.0f * j), 100);
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
				//60フレームで全部移動できるような移動量
				this->moving_Menu -= (int)ge->screenWidth / 60;
				if (this->moving_Menu < 0)
				{
					this->moving_Menu = 0;
				}
				break;
			case nowMenu::difficult:
			case nowMenu::TutorialCalum:
				//60フレームで全部移動できるような移動量
				this->moving_Menu += (int)ge->screenWidth / 60;
				if (this->moving_Menu > (int)ge->screenWidth)
				{
					this->moving_Menu = (int)ge->screenWidth;
				}
				break;
			}
			//メニューは関係なくカーソルが生きている間タイトル名は上に上がっている
			this->moving_Title_Name += MOVING_TITLE_NAME_SPEED;
		}
		//カラーウェーブ
		this->Color_Wave();
		//時間上昇
		this->timeCnt++;

		//カウンタの上限指定		
		if (this->moving_Title_Name > MOVING_TITLE_NAME_MAX)
		{
			this->moving_Title_Name = MOVING_TITLE_NAME_MAX;
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{		
		//イメージサイズをもらう
		POINT size_BG = DG::Image_Size(this->res->BG_ImageName);
		//背景を描画する領域
		ML::Box2D draw_BG(0, 0, ge->screenWidth, ge->screenHeight);
		//イメージから取る範囲
		ML::Box2D src_BG(0, 0, size_BG.x, size_BG.y);
		//背景描画
		DG::Image_Draw(this->res->BG_ImageName, draw_BG, src_BG,ML::Color(1,this->waveR,this->waveG,this->waveB));

		//2Dの描画関数を呼ぶ
		this->Draw_Title_Name();		
		this->Draw_Menu();
		this->Draw_Dif_Col(this->n);

		if (this->Is_Need_to_Draw_PAK())
		{
			this->Draw_PAK();
		}
	}

	void  Object::Render3D_L0()
	{
		
	}
	//----------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------
	//追加メソッド
	bool Object::Press_Any_Key() const
	{
		auto in1 = DI::GPad_GetState("P1");
		//ゲームパッドから入力をもらえるすべてのものを確認
		if (in1.B1.down || in1.B2.down || in1.B3.down || in1.B4.down ||
			in1.HD.down || in1.HL.down || in1.HR.down || in1.HU.down ||
			in1.L1.down || in1.L2.down || in1.L3.down ||
			in1.R1.down || in1.R2.down || in1.R3.down ||
			in1.SE.down || in1.ST.down || 
			in1.LStick.volume !=0 || in1.RStick.volume !=0 || in1.Triger.volume !=0)
		{
			return true;
		}

		return false;
	}
	//-------------------------------------------------------------------------------
	//タイトル名を表示
	void Object::Draw_Title_Name() const
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
		
		/*this->Title_Name[0].x = easing::GetPos("Title_0_x");
		this->Title_Name[1].x = easing::GetPos("Title_1_x");
		this->Title_Name[2].x = easing::GetPos("Title_2_x");
		this->Title_Name[3].x = easing::GetPos("Title_3_x");
		this->Title_Name[4].x = easing::GetPos("Title_4_x");
		this->Title_Name[5].x = easing::GetPos("Title_5_x");
		this->Title_Name[6].x = easing::GetPos("Title_6_x");
		this->Title_Name[7].x = easing::GetPos("Title_7_x");
		this->Title_Name[8].x = easing::GetPos("Title_8_x");*/

		//X座標更新
		for (int i = 0; i < 12; i++)
		{
			this->Title_Name[i].x = easing::GetPos("Title_x" + to_string(i));
		}
		//Y座標更新
		for (int i = 0; i < 12; i++)
		{						
			this->Title_Name[i].y = easing::GetPos("Title_" + to_string(i%2) + "_y");
		}		
	}
	//----------------------------------------------------------------------------------
	//press any key を表示
	void Object::Draw_PAK() const
	{		
		ML::Box2D draw((int)(ge->screenWidth/2)-375, (int)ge->screenHeight/2, 750, 100);
		//画像全体サイズ
		POINT size_Pak = DG::Image_Size(this->res->press_Any_Key_Image);
		ML::Box2D src(0, 0, size_Pak.x, size_Pak.y);

		DG::Image_Draw(this->res->press_Any_Key_Image, draw, src);
	}

	//---------------------------------------------------------------------
	//メニューの表示
	void Object::Draw_Menu() const
	{
		//現在、メニュー選択中でないなら処理しない
		if(!this->select_now)
		{
			return;
		}

		//start関連
		//表示範囲
		ML::Box2D draw_Start((ge->screenWidth/2)-200, (ge->screenHeight/2)-150, 380, 100);
		//startイメージ全体サイズ
		POINT size_Start = DG::Image_Size(this->res->start_Image);
		//画像から取り出す範囲
		ML::Box2D src_Start(0, 0, size_Start.x, size_Start.y);

		//tutorial関連
		//表示範囲
		ML::Box2D draw_Tutorial((ge->screenWidth / 2) - 200, (ge->screenHeight / 2)-50, 520, 100);
		//tutorialイメージ全体サイズ
		POINT size_Tutorial = DG::Image_Size(this->res->tutorial_Image);
		//画像から取り出す範囲
		ML::Box2D src_Tutorial(0, 0, size_Tutorial.x, size_Tutorial.y);

		//ボタンガイド関連
		//表示範囲
		ML::Box2D draw_Guide((ge->screenWidth/2)-300, (ge->screenHeight-100), 600, 50);
		//ボタンガイドイメージの全体サイズ
		POINT size_Guide = DG::Image_Size(this->res->select_Guide_Image);
		//画像から取り出す範囲
		ML::Box2D src_Guide(0, 0, size_Guide.x, size_Guide.y);

		//変数に合わせて表示範囲を更新
		draw_Start.Offset(-this->moving_Menu, 0);
		draw_Tutorial.Offset(-this->moving_Menu, 0);

		//描画開始
		DG::Image_Draw(this->res->start_Image, draw_Start, src_Start);
		DG::Image_Draw(this->res->tutorial_Image, draw_Tutorial, src_Tutorial);
		DG::Image_Draw(this->res->select_Guide_Image, draw_Guide, src_Guide);
	}

	//-----------------------------------------------------------------------------
	//難易度とテュートリアル目次の表示
	void Object::Draw_Dif_Col(nowMenu now) const
	{
		//表示範囲
		ML::Box2D draw_Dif_Col0((ge->screenWidth / 2) - 200 + ge->screenWidth, (ge->screenHeight / 2) - 150, 380, 100);//Easy or OutLine
		ML::Box2D draw_Dif_Col1((ge->screenWidth / 2) - 200 + ge->screenWidth, (ge->screenHeight / 2) - 50, 380, 100);//Normal or Control
		ML::Box2D draw_Dif_Col2((ge->screenWidth / 2) - 200 + ge->screenWidth, (ge->screenHeight / 2) + 50, 380, 100);//Hard or Obstacle

		//scrサイズは統一されている
		ML::Box2D src_Dif_col0(0, 0, 700, 140);

		//変数に合わせて表示範囲を更新
		draw_Dif_Col0.Offset(-this->moving_Menu, 0);
		draw_Dif_Col1.Offset(-this->moving_Menu, 0);
		draw_Dif_Col2.Offset(-this->moving_Menu, 0);

		//以前選択したものに合わせて描画処理を別にする
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
	//カーソルの方で呼び出せる関数
	void Object::I_Select(POINT select)
	{
		//-1ならゲーム本編
		//1ならテュートリアル
		this->next_Task_Index = select;

		this->Kill();
	}

	//-----------------------------------------------------------------------------
	//PAKを描画しなくてもいいのかを判別する
	//(trueの時に描画する)
	bool Object::Is_Need_to_Draw_PAK() const
	{
		//200フレーム以降まで何も押してないなら描画をする
		if (this->timeCnt < 200 || this->select_now)
		{
			return false;
		}
		return true;
	}
	//--------------------------------------------------------------------------------------------------
	//カラーウェーブ
	void Object::Color_Wave()
	{
		float waveCntR = 0.06f;
		float waveCntG = 0.14f;
		float waveCntB = 0.1f;
		//ウェーブ
		//サイン関数に渡す角度上昇
		this->dgreeR += waveCntR;
		this->dgreeG += waveCntG;
		this->dgreeB += waveCntB;
		//0.0f~1.0fに固定するためにずらす
		this->waveR = (sinf(ML::ToRadian(this->dgreeR)) / 4) + 0.5f;
		this->waveG = (sinf(ML::ToRadian(this->dgreeG)) / 4) + 0.5f;
		this->waveB = (sinf(ML::ToRadian(this->dgreeB)) / 4) + 0.5f;
	}
	//--------------------------------------------------------------------------------
	//BGM fade out
	void Object::BGM_Fade_Out()
	{
		this->vol -= 5;
		DM::Sound_Volume(this->res->bgm_Title, this->vol);
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