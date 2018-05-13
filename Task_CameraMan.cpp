//-------------------------------------------------------------------
//カメラマン
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_CameraMan.h"

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
		//カメラの初期設定はTask_Gameの方で行う

		this->nowPos = ge->camera[0]->pos;
		this->initPos = ge->camera[0]->pos;
		this->distance = 2750.0f;
		this->angle = ML::Vec3(0, 0, 0);
		this->maxAngle = 30;
		this->timeCnt = 0;


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
		auto in1 = DI::GPad_GetState("P1");

		this->nowPos = ge->Map_center - ML::Vec3(0, 0, this->distance);

		//Rstickの動きでカメラを移動
		if (in1.RStick.axis.x != 0)
		{
			this->angle.x += in1.RStick.axis.x * 3;
		}
		if (in1.RStick.axis.y != 0)
		{
			this->angle.y -= in1.RStick.axis.y * 3;
		}

		//移動範囲設定
		if (this->angle.x < -this->maxAngle)
		{
			this->angle.x = -this->maxAngle;
		}
		else if (this->angle.x > this->maxAngle)
		{
			this->angle.x = this->maxAngle;
		}
		if (this->angle.y < -this->maxAngle)
		{
			this->angle.y = -this->maxAngle;
		}
		else if (this->angle.y > this->maxAngle)
		{
			this->angle.y = this->maxAngle;
		}

		//回転クォータニオン生成
		ML::QT rotationY = ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(this->angle.x));
		ML::QT rotationX = ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(this->angle.y));

		//アフィン変換
		ML::Mat4x4 matR;
		D3DXMatrixAffineTransformation(&matR, 1, &ge->Map_center, &(rotationX*rotationY), NULL);

		this->nowPos = matR.TransformCoord(this->nowPos);

		//カメラ位置の更新
		ge->camera[0]->pos = this->nowPos;

		//ライトがオフされてから3秒後に、ライトをオンする
		if (this->It_Passed_3sec())
		{
			DG::EffectState().param.light[0].enable = true;
			DG::EffectState().param.lightAmbient = ML::Color(1, 0.3f, 0.3f, 0.3f);
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
		DG::EffectState().param.light[0].enable = true;
		DG::EffectState().param.lightAmbient = ML::Color(1, 0, 0, 0);
		this->timeCnt = 0;
	}
	//-----------------------------------------------------------------
	//3秒がたったのかを確認
	bool Object::It_Passed_3sec()
	{
		if (this->timeCnt > 180)
		{
			return true;
		}
		return false;
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