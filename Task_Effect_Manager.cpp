//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Effect_Manager.h"
#include  "easing.h"

namespace  EffectManager
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
		//エフェクトリストをクリア
		this->efList.clear();
		this->play_Effect_List.clear();

		//使用するエフェクトメッシュの生成
		//テレポートイン
		DG::Mesh_CreateFromSOBFile("DestroyItem", "./data/mesh/effect/DestroyItem.SOB");
		//テレポートアウト
		DG::Mesh_CreateFromSOBFile("CreateItem", "./data/mesh/effect/CreateItem.SOB");
		//ゲームクリア
		DG::Mesh_CreateFromSOBFile("DestroyCharactor", "./data/mesh/effect/DestroyPlayer.SOB");
		
		//必要なエフェクト生成
		this->Create_Effect(BEffect::effType::Teleportin);
		this->Create_Effect(BEffect::effType::TeleportOut);
		this->Create_Effect(BEffect::effType::Game_Clear);

		this->render3D_Priority[0] = 1.0f;		

		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		DG::Mesh_Clear();
		if (this->efList.size() != 0)		
		{
			//リストの開放
			for (auto e : this->efList)
			{
				delete e;
			}
		}
		this->efList.clear();
		this->play_Effect_List.clear();
		/*for (auto it = this->efList.begin(); it != this->efList.end(); it++)
		{
			if ((*it) != nullptr)
			{
				delete (*it);
			}
		}*/
		
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
		//念のためプレイリストサイズの上限を決めておく
		if (this->play_Effect_List.size() > 20)
		{
			this->play_Effect_List.pop_front();
		}
		//エフェクトのフレーム毎の変化
		for (auto it = this->play_Effect_List.begin(); it != this->play_Effect_List.end(); it++)
		{
			if ((*it).Get_Type() == BEffect::effType::CLEAR)
			{
				continue;
			}
			(*it).UpDate_Effect();
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
			if ((*it).Get_Type() == BEffect::effType::CLEAR)
			{
				continue;
			}
			(*it).Playing_Effect();
		}
	}

	//--------------------------------------------------------------------------------------
	//追加メソッド
	//-----------------------------------------------------------------------------
	void Object::Add_Effect(const ML::Vec3& pos, const ML::Vec3& angle, const BEffect::effType& handle)
	{	
		Effect ef(BEffect::effType::CLEAR);
		//プレイリストに入るエフェクトをコピーする
		for (auto& el : this->efList)
		{
			if (el->Get_Type() == handle)
			{
				ef = *el;
			}
		}
		//エフェクト情報設定
		ef.Load_Eff(pos, angle);
		//プレイリストにpush_back
		this->play_Effect_List.push_back(ef);
	}
	void Object::Add_Effect(const ML::Vec3& pos, const ML::Vec3& target, const ML::Vec3& angle, const BEffect::effType& handle)
	{
		Effect ef(BEffect::effType::CLEAR);
		//プレイリストに入るエフェクトをコピーする
		for (auto& el : this->efList)
		{
			if (el->Get_Type() == handle)
			{
				ef = *el;
			}
		}
		//エフェクト情報設定
		ef.Load_Eff(pos, target, angle);
		//プレイリストにpush_back
		this->play_Effect_List.push_back(ef);
	}
	//-----------------------------------------------------------------------------------
	//新しいエフェクト生成
	void Object::Create_Effect(const BEffect::effType& handle)
	{
		Effect* NewEf = new Effect(handle);
		//全体リストにpush_back
		this->efList.push_back(NewEf);
	}
	//----------------------------------------------------------------------------------
	//エフェクト寿命減らし及び削除
	void Object::Dec_Effect_Life()
	{
		if (this->play_Effect_List.size() == 0)
		{
			return;
		}
		for (auto it = this->play_Effect_List.begin(); it != this->play_Effect_List.end(); it++)
		{
			if ((*it).Is_Alive())
			{
				(*it).Dec_Eff();
			}
		}
		//ライフが０になったエフェクトはリストから外す		
		this->play_Effect_List.remove_if([](Effect& e) {return e.Eff_Judge(); });
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