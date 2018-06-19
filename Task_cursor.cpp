//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_cursor.h"
#include "Task_Title.h"
#include "Task_UI.h"

#define OUT_OF_SCREEN -32

namespace  Cursor
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imageName = "Cursor";
		DG::Image_Create(this->imageName, "./data/image/cursor.png");

		this->seMoveName = "seCursorMove";
		DM::Sound_CreateSE(this->seMoveName, "./data/sound/Cursor.wav");
		this->seSelectName = "seCursorSelect";
		DM::Sound_CreateSE(this->seSelectName, "./data/sound/CursorSelct.wav");
		this->seCancelName = "seCursorCancel";
		DM::Sound_CreateSE(this->seCancelName, "./data/sound/CursorCencel.wav");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
		DM::Sound_Erase(this->seMoveName);
		DM::Sound_Erase(this->seCancelName);
		DM::Sound_Erase(this->seSelectName);
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
		this->pos = ML::Vec2(float(ge->screenWidth/3), OUT_OF_SCREEN);
		this->select_pos.x = 0;
		this->select_pos.y = -1;
		this->countdown = 0;
		this->countdownFlag = false;
		this->now = Start_Tutorial;

		this->first_Menu_Range[0] = -1;
		this->first_Menu_Range[1] = 1;

		this->last_Menu_Range[0] = -1;
		this->last_Menu_Range[1] = 3;

		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放

		auto title = ge->GetTask_One_G <Title::Object>("タイトル");
		if (title == nullptr)
		{
			return false;
		}
		title->I_Select(this->select_pos);


		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{			
		auto title = ge->GetTask_One_G <Title::Object>("タイトル");
		//カウントダウンフラグが立っていない場合のみ選択と取り消し処理を行う
		if (!this->Is_Count_Down())
		{
			//カーソルの目的地を設定
			this->destination.y = (float)this->Move_Cursor(this->select_pos);
			//カーソルの選択処理
			this->Click();
		}
		//フラグが立ったらカウントダウン開始
		else
		{
			this->countdown++;
			//カーソルの目的地は画面外にする
			this->destination.y = OUT_OF_SCREEN;
			//BGMをフェイトアウトさせる
			title->BGM_Fade_Out();
		}
		//カウントダウンが終わったらタスク消滅
		if (this->Count_Down_Over())
		{
			this->Kill();
		}
		//25％ずつ移動
		this->pos.y += (this->destination.y - this->pos.y) * 0.25f;
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//表示範囲
		ML::Box2D draw(-16, -16, 32, 32);
		//画像全体サイズ
		POINT size_Cursor = DG::Image_Size(this->res->imageName);
		ML::Box2D src(0, 0, size_Cursor.x, size_Cursor.y);

		draw.Offset(this->pos);
		DG::Image_Draw(this->res->imageName, draw, src);
	}

	void  Object::Render3D_L0()
	{
		
	}

	//------------------------------------------------------------------------------------
	//追加メソッド
	int Object::Move_Cursor(POINT& select)
	{
		auto in1 = DI::GPad_GetState("P1");

		//メニュー移動単位
		int cursor_Moving = 2;
		//移動
		if (in1.LStick.U.down || in1.HU.down)
		{
			//SE再生
			DM::Sound_Play(this->res->seMoveName, false);
			select.y -= cursor_Moving;
		}
		if (in1.LStick.D.down || in1.HD.down)
		{
			//SE再生
			DM::Sound_Play(this->res->seMoveName, false);
			select.y += cursor_Moving;
		}
		//現在メニュ－に応じる移動範囲設定
		if (this->now == Start_Tutorial)
		{
			if (select.y < this->first_Menu_Range[0])
			{
				select.y = this->first_Menu_Range[0];
			}
			else if (select.y > this->first_Menu_Range[1])
			{
				select.y = this->first_Menu_Range[1];
			}
		}
		else
		{
			if (select.y < this->last_Menu_Range[0])
			{
				select.y = this->last_Menu_Range[0];
			}
			else if (select.y > this->last_Menu_Range[1])
			{
				select.y = this->last_Menu_Range[1];
			}
		}
		
		//目的地になるところを計算して返す
		return (ge->screenHeight/2)-50 + (select.y * 50);
	}

	//---------------------------------------------------------------------------------
	//現在メニューを返す関数
	nowMenu Object::Get_Now_Menu()
	{
		return this->now;
	}

	//---------------------------------------------------------------------------------
	//カウントダウンフラグを返す関数
	bool Object::Is_Count_Down() const
	{
		return this->countdownFlag;
	}
	//--------------------------------------------------------------------------
	//画面切り替えが終わったのかを確認
	bool Object::Count_Down_Over() const
	{
		return (this->countdown > 130);
	}

	//------------------------------------------------------------------------------
	//選択処理
	void Object::Click()
	{
		auto in1 = DI::GPad_GetState("P1");

		if (in1.B2.down || in1.ST.down)
		{
			DM::Sound_Play(this->res->seSelectName, false);
			//今のメニューに対して処理が違う
			if (this->now == Start_Tutorial)
			{
				//第1メニューだったら洗濯を保存して次のメニューに移動する
				this->now = nowMenu(this->select_pos.y);
				this->select_pos.x = this->select_pos.y;
			}
			else
			{
				//最後のメニューだったら
				//UIタスクに画面隠しを頼む
				ge->GetTask_One_G<UI::Object>("UI")->Start_WipeIn();
				this->countdownFlag = true;
			}
		}
		//カーソルの取り消し処理
		if (in1.B1.down)
		{
			this->now = Start_Tutorial;
			this->select_pos.x = 0;
			DM::Sound_Play(this->res->seCancelName, false);
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