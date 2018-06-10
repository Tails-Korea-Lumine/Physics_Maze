//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_UI.h"
#include  "easing.h"
#include  "Task_Game.h"
#include "Task_Result.h"

namespace  UI
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		//名前初期化
		this->uI_Time_Img = "UI_Time";
		this->wipeIn_Img = "WipeIn";
		this->wipeOut_Img = "WipeOut";
		this->number_Img = "Number";
		
		//オフスクリーン生成
		DG::Image_Create(this->uI_Time_Img, "./data/image/UI_Time.png");
		DG::Image_Create(this->wipeIn_Img, "./data/image/wipeIn.png");
		DG::Image_Create(this->wipeOut_Img, "./data/image/wipeOut.png");
		DG::Image_Create(this->number_Img, "./data/image/Number.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		//画像の開放
		DG::Image_Erase(this->uI_Time_Img);
		DG::Image_Erase(this->wipeIn_Img);
		DG::Image_Erase(this->wipeOut_Img);
		DG::Image_Erase(this->number_Img);
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
		this->render2D_Priority[0] = 1.0f;
		this->timeCnt = 0;
		this->wipe = WipeInOut::Wipe_Out;
		this->wipe_Alpha = 1.0f;

		//uiのy座標はeasingで動かすので画面外適当なところに初期化
		this->ui_Time = ML::Vec2(0, -1000.0f);
		//数字表の読み込みチップを初期化
		for (int i = 0; i < 11; i++)
		{
			this->src_Number[i].x = (140 * i);
			this->src_Number[i].y = 0;
			this->src_Number[i].w = 140;
			this->src_Number[i].h = 140;
		}
		//時計の表し方
		easing::Set("Up_to_Down", easing::SINEOUT, -50, 0, 30);		
		easing::Start("Up_to_Down");

		//幕のeasingSet
		easing::Set("wipeOut", easing::SINEOUT, -200, (float)ge->screenWidth, 200);
		easing::Set("wipeIn", easing::SINEOUT, -2200, 0, 130);
		
		//幕はタスク生成の時画面から消えて行って
		//タスク消滅前に画面の上まで移動する
		easing::Start("wipeOut");

		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		this->ui_Time.y = easing::GetPos("Up_to_Down");
		//UIで見せる時計は準備が終わった後に流れる
		if (ge->game.lock()->GET_READY())
		{
			this->timeCnt++;
		}

		//透明度の範囲指定
		if (this->wipe_Alpha < 0.0f)
		{
			this->wipe_Alpha = 0.0f;
		}
		if (this->wipe_Alpha > 1.0f)
		{
			this->wipe_Alpha = 1.0f;
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		if (ge->GetTask_One_G<Game::Object>("ゲーム") != nullptr)	
		{
			this->Draw_Time();
		}		
		this->Draw_Wipe_InOut();
	}
	//----------------------------------------------------------------------------------
	//時間の描画
	void Object::Draw_Time() const
	{
		int min10, min1, sec10, sec1;
		//各位の数値を別々にもらう
		min10 = this->timeCnt / 60 / 60 / 10;
		min1 = this->timeCnt / 60 / 60 % 10;
		sec10 = this->timeCnt / 60 % 60 / 10;
		sec1 = this->timeCnt / 60 % 60 % 10;

		ML::Box2D draw_UI_Time(0, 0, 200, 64);
		ML::Box2D src_UI_Time(0, 0, 430, 140);
		ML::Box2D draw_Min10(200, 0, 64, 64);
		ML::Box2D draw_Min1(250, 0, 64, 64);
		ML::Box2D draw_Sec10(310, 0, 64, 64);
		ML::Box2D draw_Sec1(360, 0, 64, 64);
		ML::Box2D draw_colon(280, 0, 50, 64);

		//easingでのY座標更新
		draw_UI_Time.Offset(this->ui_Time);
		draw_Min10.Offset(this->ui_Time);
		draw_Min1.Offset(this->ui_Time);
		draw_colon.Offset(this->ui_Time);
		draw_Sec10.Offset(this->ui_Time);
		draw_Sec1.Offset(this->ui_Time);

		//描画
		DG::Image_Draw(this->res->uI_Time_Img, draw_UI_Time, src_UI_Time);
		DG::Image_Draw(this->res->number_Img, draw_Min10, this->src_Number[min10]);
		DG::Image_Draw(this->res->number_Img, draw_Min1, this->src_Number[min1]);
		DG::Image_Draw(this->res->number_Img, draw_Sec10, this->src_Number[sec10]);
		DG::Image_Draw(this->res->number_Img, draw_Sec1, this->src_Number[sec1]);
		DG::Image_Draw(this->res->number_Img, draw_colon, this->src_Number[10]);
	}

	//--------------------------------------------------------------------------------------
	//幕の描画
	void Object::Draw_Wipe_InOut()
	{
		ML::Box2D drawOut(-200, -160, 3000, 1500);
		ML::Box2D drawIN(-2200, 0, 3000, 1500);		
		ML::Box2D src(0, 0, 1920, 1080);
		
		switch (this->wipe)
		{			
			//画面明かし
		case WipeInOut::Wipe_Out:			

			this->wipe_Alpha -= 0.01f;
			
			drawOut.x = int(easing::GetPos("wipeOut"));

			DG::Image_Draw(this->res->wipeOut_Img, drawOut, src, ML::Color(this->wipe_Alpha,1,1,1));
			break;
						
			//画面隠し
		case WipeInOut::Wipe_In:
			

			drawIN.x = int(easing::GetPos("wipeIn"));
			this->wipe_Alpha += 0.02f;			

			DG::Image_Draw(this->res->wipeIn_Img, drawIN, src, ML::Color(this->wipe_Alpha, 1, 1, 1));
			break;
		}
	}

	//-----------------------------------------------------------------------
	//画面隠しを始める
	void Object::Start_WipeIn()
	{
		easing::Start("wipeIn");
		this->wipe = WipeInOut::Wipe_In;
		this->wipe_Alpha = 0.1f;
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