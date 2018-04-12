//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Game.h"
#include  "Task_title.h"
#include  "Task_Map3d.h"
#include  "Task_Ball.h"
#include  "Task_Player.h"

namespace  Game
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

		//カメラの設定
		ge->camera[0] = MyPG::Camera::Create(
			ML::Vec3(1000.0f, 0.0f, 1000.0f),				//	ターゲット位置
			ML::Vec3(1000.0f, 1400.0f, -1000.0f),			//	カメラ位置
			ML::Vec3(0.0f, 1.0f, 0.0f),					//	カメラの上方向ベクトル
			ML::ToRadian(35), 10.0f, 4000.0f,	//	視野角・視野距離
			(float)ge->screenWidth / (float)ge->screenHeight);		//	画面比率		
		DG::EffectState().param.bgColor = ML::Color(1, 1, 1, 1);
		//ライティング有効化
		DG::EffectState().param.lightsEnable = true;
		//環境光の強さを設定する
		DG::EffectState().param.lightAmbient = ML::Color(1, 0.3f, 0.3f, 0.3f);
		//平行光源の設定
		DG::EffectState().param.light[0].enable = true;
		DG::EffectState().param.light[0].kind = DG_::Light::Directional;//光源の種類
		DG::EffectState().param.light[0].direction = ML::Vec3(1, -1, 1).Normalize();//照射方向
		DG::EffectState().param.light[0].color = ML::Color(1, 1, 1, 1);//色と強さ

		//デバッグ用の文字生成
		DG::Font_Create("FontA", "HGSｺﾞｼｯｸM", 12, 16);

		//ワールド回転量初期化
		ge->World_Rotation = ML::QT();

		//★タスクの生成
		//仮のマップ生成	
		auto map = Map3d::Object::Create(true);
		auto ball = Ball::Object::Create(true);
		auto player = Player::Object::Create(true);

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		auto map = ge->GetTask_One_G<Map3d::Object>("マップ");
		auto ball = ge->GetTask_One_G<Ball::Object>("ボール");
		map->Kill();
		ball->Kill();

		DG::Font_Erase("FontA");

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

		if (in.ST.down)
		{
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		auto ball = ge->GetTask_One_G<Ball::Object>("ボール");

		char buf[1024];
		sprintf(buf, "pos : %4.3f , %4.3f , %4.3f \n"
			"speed : %4.3f , %4.3f , %4.3f \n"
			"WorldR : %4.3f , %4.3f , %4.3f",
			 ball->pos.x, ball->pos.y, ball->pos.z, ball->speed.x, ball->speed.y, ball->speed.z,
			ge->World_Rotation.x, ge->World_Rotation.y, ge->World_Rotation.z);

		ML::Box2D moji(100, 0, 600, 600);
		DG::Font_Draw("FontA", moji, buf, ML::Color(1, 1, 0, 1));
	}

	void  Object::Render3D_L0()
	{
		
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