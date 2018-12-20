//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Effect_Manager.h"
#include  "easing.h"
#include "Game_Clear.h"
#include "Teleport_In.h"
#include "Teleport_Out.h"
#include "Breaking.h"
#include "Crashed.h"

namespace  EffectManager
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		//使用するエフェクトメッシュの生成
		//テレポートイン
		this-> mesh_Telein = "Teleport_In";
		DG::Mesh_CreateFromSOBFile(mesh_Telein, "./data/mesh/effect/DestroyItem.SOB");
		//テレポートアウト
		this-> mesh_Teleout = "Teleport_Out";
		DG::Mesh_CreateFromSOBFile(mesh_Teleout, "./data/mesh/effect/CreateItem.SOB");
		//ゲームクリア
		this-> mesh_Gameclear = "Game_Clear";
		DG::Mesh_CreateFromSOBFile(mesh_Gameclear, "./data/mesh/effect/DestroyPlayer.SOB");
		//ブレイキング
		this->mesh_Breaking = "Breaking";
		DG::Mesh_CreateFromSOBFile(this->mesh_Breaking, "./data/mesh/effect/Breaking.SOB");
		//クラッシュ
		this->mesh_Crashed = "Crashed";
		DG::Mesh_CreateFromSOBFile(this->mesh_Crashed, "./data/mesh/effect/Crashed.SOB");

		//サウンドエフェクト
		//テレポートイン
		this-> seTeleportIn = "SETeleIn";
		DM::Sound_CreateSE(seTeleportIn, "./data/sound/TeleportIn.wav");
		//テレポートアウト
		this-> seTeleportOut = "SETeleOut";
		DM::Sound_CreateSE(seTeleportOut, "./data/sound/TeleportOut.wav");
		//ブレイキング
		this->se_Breaking = "SE_Breaking";
		DM::Sound_CreateSE(this->se_Breaking, "./data/sound/Breaking_Box.wav");
		//クラッシュ
		this->se_Crashed = "SE_Crashed";
		DM::Sound_CreateSE(this->se_Crashed, "./data/sound/Crashed.wav");

		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Mesh_Erase(this->mesh_Telein);
		DG::Mesh_Erase(this->mesh_Gameclear);
		DG::Mesh_Erase(this->mesh_Teleout);
		DG::Mesh_Erase(this->mesh_Breaking);
		DG::Mesh_Erase(this->mesh_Crashed);
		DM::Sound_Erase(seTeleportIn);
		DM::Sound_Erase(seTeleportOut);
		DM::Sound_Erase(this->se_Breaking);
		DM::Sound_Erase(this->se_Crashed);
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
		//エフェクトリストをクリア
		this->play_Effect_List.clear();

		this->render3D_Priority[0] = 1.0f;		

		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放		
		if (this->play_Effect_List.size() != 0)		
		{
			//リストの開放
			for (auto e : this->play_Effect_List)
			{
				delete e;
			}
		}
		this->play_Effect_List.clear();
		
		
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
		//easing function update
		easing::UpDate();
		
		//エフェクトのフレーム毎の変化
		for (auto it = this->play_Effect_List.begin(); it != this->play_Effect_List.end(); it++)
		{			
			(*it)->Effect_Update();
		}
		//寿命減少
		this->Dec_Effect_Life();
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
	}

	void  Object::Render3D_L0()
	{
		//エフェクトの再生
		for (auto it = this->play_Effect_List.begin(); it != this->play_Effect_List.end(); it++)
		{			
			(*it)->Effect_Draw();
		}
	}

	//--------------------------------------------------------------------------------------
	//追加メソッド
	//-----------------------------------------------------------------------------
	void Object::Add_Effect(const ML::Vec3& pos, const ML::Vec3& angle, const BEffect::effType& handle)
	{	
		BEffect* ef;
		//ハンドルに合うエフェクトを生成してリストに登録
		switch (handle)
		{
		case BEffect::effType::Game_Clear:
			ef = new Game_Clear(pos, angle, this->res->mesh_Gameclear, "");
			break;
		case BEffect::effType::TeleportOut:
			ef = new Teleport_Out(pos, angle, this->res->mesh_Teleout, this->res->seTeleportOut);
			break;
		case BEffect::effType::Breaking:
			ef = new Breaking(pos, angle, this->res->mesh_Breaking, this->res->se_Breaking);
			break;
		case BEffect::effType::Crashed:
			ef = new Crashed(pos, angle, this->res->mesh_Crashed, this->res->se_Crashed);
			break;
		//条件に合ってないエフェクトハンドルは処理しない
		default:
			return;
		}
		//プレイリストにpush_back
		this->play_Effect_List.push_back(ef);
	}
	void Object::Add_Effect(const ML::Vec3& pos, const ML::Vec3& target, const ML::Vec3& angle, const BEffect::effType& handle)
	{
		BEffect* ef;
		//ハンドルに合うエフェクトを生成してリストに登録
		switch (handle)
		{		
		case BEffect::effType::Teleportin:
			ef = new Teleport_In(pos, target, angle, this->res->mesh_Telein, this->res->seTeleportIn);
			break;
			//条件に合ってないエフェクトハンドルは処理しない
		default:
			return;
		}
		//プレイリストにpush_back
		this->play_Effect_List.push_back(ef);
	}
	//-----------------------------------------------------------------------------------	
	//エフェクト寿命減らし及び削除
	void Object::Dec_Effect_Life()
	{
		if (this->play_Effect_List.empty())
		{
			return;
		}
		for (auto it = this->play_Effect_List.begin(); it != this->play_Effect_List.end(); it++)
		{
			//寿命減少
			(*it)->Dec_Eff();
			//ライフが０になったエフェクトは開放処理
			if ((*it)->Is_Alive() == false)
			{
				delete (*it);
				(*it) = nullptr;
			}
		}
		//解放されたものはリストから外す
		this->play_Effect_List.remove_if([](BEffect* e) {return e == nullptr; });
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