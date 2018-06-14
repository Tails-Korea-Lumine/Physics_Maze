//-------------------------------------------------------------------
//カメラマン
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_CameraMan.h"
#include  "Task_Physics_Manager.h"
#include  "Task_Game.h"
#include  "easing.h"

namespace  CameraMan
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
			ML::Vec3(1050.0f, 50.0f, 1050.0f),				//	ターゲット位置
			ML::Vec3(2050.0f, 1050.0f, -1500.0f),//	カメラ位置
			ML::Vec3(0.0f, 1.0f, 0.0f),					//	カメラの上方向ベクトル
			ML::ToRadian(35), 10.0f, 4000.0f,	//	視野角・視野距離
			(float)ge->screenWidth / (float)ge->screenHeight);		//	画面比率		
		DG::EffectState().param.bgColor = ML::Color(1, 0.0f, 0.0f, 0.0f);
		//ライティング有効化
		DG::EffectState().param.lightsEnable = true;
		//環境光の強さを設定する
		DG::EffectState().param.lightAmbient = ML::Color(1, 0.3f, 0.3f, 0.3f);
		//平行光源の設定
		DG::EffectState().param.light[0].enable = true;
		DG::EffectState().param.light[0].kind = DG_::Light::Directional;//光源の種類
		DG::EffectState().param.light[0].direction = ML::Vec3(1, -1, 1).Normalize();//照射方向
		DG::EffectState().param.light[0].color = ML::Color(1, 1, 1, 1);//色と強さ		

		easing::Set("Moving_Camera", easing::EXPOIN, 0, 0.1f, 20);

		this->nowPos = ge->camera[0]->pos;
		this->initPos = ge->camera[0]->pos;
		this->distance = 2750.0f;
		this->angle = ML::Vec3(0, 0, 0);
		this->maxAngle = 30;
		this->timeCnt = 0;
		this->destination = this->nowPos;
		this->moving_Flag = false;


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
			//auto nextTask = Game::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		//auto game = ge->GetTask_One_G<Game::Object>("ゲーム");

		if (!ge->game.lock()->GET_READY())
		{
			this->ProMotion();
		}
		else
		{
			this->Camera_Move();
		}

		//ライトがオフされてから3秒後に、ライトをオンする
		if (this->It_Passed_3sec())
		{
			this->Turnon_the_Light();
		}
		this->timeCnt++;
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{		
	}

	void  Object::Render3D_L0()
	{
		
	}

	//---------------------------------------------------------------------
	//追加メソッド
	//ライトオフ
	void Object::Turnoff_the_Light()
	{
		DG::EffectState().param.light[0].enable = false;
		DG::EffectState().param.lightAmbient = ML::Color(1, 0.005f, 0.005f, 0.005f);
		this->timeCnt = 0;
	}
	//----------------------------------------------------------------
	//ライトオン
	void Object::Turnon_the_Light()
	{
		DG::EffectState().param.light[0].enable = true;
		DG::EffectState().param.lightAmbient = ML::Color(1, 0.3f, 0.3f, 0.3f);
	}
	//-----------------------------------------------------------------
	//3秒がたったのかを確認
	bool Object::It_Passed_3sec() const
	{
		if (this->timeCnt > 180)
		{
			return true;
		}
		return false;
	}

	//------------------------------------------------------------------------
	//ゲームスタート演出
	void Object::ProMotion()
	{
		if (this->timeCnt < 180)
		{
			ge->camera[0]->pos.z += 22;
		}
		else if (this->timeCnt == 180)
		{
			ge->camera[0]->pos = ML::Vec3(50, -950, 1940);
		}
		else if (this->timeCnt < 360)
		{
			ge->camera[0]->pos.z -= 22;
		}
		else if (this->timeCnt == 360)
		{
			ge->camera[0]->pos = ML::Vec3(1050, 50, -1100);
		}
		else
		{
			ge->camera[0]->pos.z -= 13;
		}

		if (ge->camera[0]->pos.z < ge->Map_center.z - this->distance)
		{
			ge->camera[0]->pos.z = ge->Map_center.z - this->distance;
		}
	}

	//--------------------------------------------------------------------------
	//カメラの移動操作
	void Object::Camera_Move()
	{
		//目的地と現在位置が異なるときに移動開始
		if (this->nowPos != this->destination)
		{	
			easing::Start("Moving_Camera");
			//移動中フラグを立てる
			this->moving_Flag = true;
			//目的地と現在位置が異なる場合
			//相対距離の25%ずつ移動する
			ML::Vec3 d = this->destination - this->nowPos;
			this->nowPos += d * easing::GetPos("Moving_Camera");

			//一定距離以内なら代入させる
			if (d.Length() <= 20)
			{				
				this->moving_Flag = false;
				this->nowPos = this->destination;
				//軸再計算
				ge->GetTask_One_G<Physics_Manager::Object>("物理運動")->Ancker_Calculating();
				//easing reset
				easing::Reset("Moving_Camera");
			}
			//カメラ位置の更新
			ge->camera[0]->pos = this->nowPos;
			ge->camera[0]->UpDate();
		}
	}
	//---------------------------------------------------------------------------
	//目的地設定
	void Object::Set_Destination(const ML::Vec3& side_Normal_On_Ball)
	{
		this->destination = ge->Map_center + (side_Normal_On_Ball.Normalize() * this->distance);
	}
	//----------------------------------------------------------------------------
	//移動中なのかを確認する
	bool Object::Is_Moving_Now()
	{
		return this->moving_Flag;
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