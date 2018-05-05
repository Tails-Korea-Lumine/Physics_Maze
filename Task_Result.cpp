//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Result.h"
#include  "Task_Title.h"
#include  "Task_UI.h"

namespace  Result
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->thisTime = "thisTime";
		this->yourScore = "yourScore";
		this->result = "result";
		this->Number_Image = "resultNumber";
		this->presS = "pressS";

		this->bgmName = "ResultBgmName";

		DG::Image_Create(this->thisTime, "./data/image/thisTime.png");
		DG::Image_Create(this->yourScore, "./data/image/yourScore.png");
		DG::Image_Create(this->result, "./data/image/result.png");
		DG::Image_Create(this->Number_Image, "./data/image/Number.png");
		DG::Image_Create(this->presS, "./data/image/pressS.png");

		DM::Sound_CreateStream(this->bgmName, "./data/sound/ending.wav");

		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->thisTime);
		DG::Image_Erase(this->yourScore);
		DG::Image_Erase(this->result);
		DG::Image_Erase(this->Number_Image);

		DM::Sound_Erase(this->bgmName);
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize(int playTime)
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化
		this->countdown = 0;
		this->countdownFlag = false;
		//bgm再生
		DM::Sound_Play(this->res->bgmName, true);

		//点数計算式 : 1000-プレー時間(秒単位)
		this->score = 1000-(playTime/60);
		this->timeCnt = 0;
		for (int i = 0; i < 10; i++)
		{
			this->src_Number[i].x = (140 * i);
			this->src_Number[i].y = 0;
			this->src_Number[i].w = 140;
			this->src_Number[i].h = 140;
		}

		//カメラの設定
		
		DG::EffectState().param.bgColor = ML::Color(1, 0, 0, 0);
		//★タスクの生成

		UI::Object::Create(true);

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->KillAll_G("UI");

		DM::Sound_Stop(this->res->bgmName);

		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			//★引き継ぎタスクの生成
			auto NextTask = Title::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto in = DI::GPad_GetState("P1");
		//カウントダウンフラグを立てる
		if (in.ST.down)
		{
			this->countdownFlag = true;
			ge->GetTask_One_G<UI::Object>("UI")->Start_WipeIn();
			this->timeCnt = 300;
		}
		//フラグが立っている間にカウントダウンする
		if (this->Is_Count_Down())
		{
			this->countdown++;
		}
		//1秒後にタスク消滅
		if (this->countdown > 60)
		{
			this->Kill();
		}
		

		this->timeCnt++;
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//result
		ML::Box2D draw_Result(30, 30, 400, 100);
		ML::Box2D src_Result(0, 0, 500, 140);
		DG::Image_Draw(this->res->result, draw_Result, src_Result);
		//this time
		if (this->timeCnt > 60)
		{
			ML::Box2D draw_ThisTime(500, 70, 700, 140);
			ML::Box2D src_ThisTime(0, 0, 700, 140);
			DG::Image_Draw(this->res->thisTime, draw_ThisTime, src_ThisTime);
		}
		//your score
		if (this->timeCnt > 120)
		{
			ML::Box2D draw_YourScore(450, 300, 770, 140);
			ML::Box2D src_YourScore(0, 0, 770, 140);
			DG::Image_Draw(this->res->yourScore, draw_YourScore, src_YourScore);
		}
		//スコアー表示
		if (this->timeCnt > 180)
		{
			this->Draw_Score();
		}
		//press S to return title
		if (this->timeCnt > 240)
		{
			ML::Box2D draw_pressS(200, 650, 700, 70);
			ML::Box2D src_pressS(0, 0, 1400, 140);
			DG::Image_Draw(this->res->presS, draw_pressS, src_pressS);
		}

	}

	void  Object::Render3D_L0()
	{
		
	}

	//------------------------------------------------------------------------------------
	//追加メソッド
	void Object::Draw_Score()
	{
		int score1000 = 0;
		int score100 = 0;
		int score10 = 0;
		int score1 = 0;
		//点数がーなら０点で表示する
		if (this->score > 0)
		{
			score1000 = this->score / 1000;
			score100 = (this->score / 100) % 10;
			score10 = (this->score / 10) % 10;
			score1 = this->score % 10;
		}


		ML::Box2D draw1000(400, 450, 140, 140);
		ML::Box2D draw100(520, 450, 140, 140);
		ML::Box2D draw10(640, 450, 140, 140);
		ML::Box2D draw1(760, 450, 140, 140);

		//描画
		DG::Image_Draw(this->res->Number_Image, draw1000, this->src_Number[score1000]);
		DG::Image_Draw(this->res->Number_Image, draw100, this->src_Number[score100]);
		DG::Image_Draw(this->res->Number_Image, draw10, this->src_Number[score10]);
		DG::Image_Draw(this->res->Number_Image, draw1, this->src_Number[score1]);
	}

	//----------------------------------------------------------------------------------
	//カウンタダウンフラグを返す関数
	bool Object::Is_Count_Down()
	{
		return this->countdownFlag;
	}
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_, int playTime)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
			}
			if (!ob->B_Initialize(playTime)) {
				ob->Kill();//イニシャライズに失敗したらKill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize(int playTime)
	{
		return  this->Initialize(playTime);
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