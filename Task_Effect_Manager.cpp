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
		//使用するエフェクトメッシュの生成
		//テレポートイン
		DG::Mesh_CreateFromSOBFile("DestroyItem", "./data/mesh/effect/DestroyItem.SOB");
		//テレポートアウト
		DG::Mesh_CreateFromSOBFile("CreateItem", "./data/mesh/effect/CreateItem.SOB");
		//ゲームクリア
		DG::Mesh_CreateFromSOBFile("DestroyCharactor", "./data/mesh/effect/DestroyPlayer.SOB");
		//エフェクトリストをクリア
		this->efList.clear();

		this->render3D_Priority[0] = 1.0f;
		//ダミーデータ
		Effect* dummmy0 = new Effect;
		dummmy0->Set_Dummy();

		this->efList.push_back(dummmy0);

		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		//リストの開放
		DG::Mesh_Clear();
		if (this->efList.size() != 0)
		{
			for (auto i : this->efList)
			{
				i->Finalize();
			}
		}
		this->efList.clear();
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

		if (this->efList.size() > 60)
		{
			this->efList.pop_front();
		}
		//エフェクトのフレーム毎の変化
		for (auto it = this->efList.begin(); it != this->efList.end(); it++)
		{
			if ((*it) == nullptr)
			{
				continue;
			}
			(*it)->UpDate_Effect((*it)->Get_Type());
		}
		this->Dec_Effect_Life();
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
	}

	void  Object::Render3D_L0()
	{
		if (this->efList.size() <= 1)
		{
			return;
		}
		//エフェクトの再生
		for (auto it = this->efList.begin(); it != this->efList.end(); it++)
		{
			if ((*it) == nullptr)
			{
				continue;
			}
			(*it)->Playing_Effect((*it)->Get_Type());
		}
	}

	//--------------------------------------------------------------------------------------
	//追加メソッド
	//-----------------------------------------------------------------------------
	void Object::Add_Effect(ML::Vec3 pos, ML::Vec3 angle, BEffect::effType handle)
	{
		Effect* NewEF = new Effect();
		NewEF->Load_Eff(pos, angle, handle);

		this->efList.push_back(NewEF);
	}
	void Object::Add_Effect(ML::Vec3 pos, ML::Vec3 target, ML::Vec3 angle, BEffect::effType handle)
	{
		Effect* NewEF = new Effect();
		NewEF->Load_Eff(pos, target, angle, handle);

		this->efList.push_back(NewEF);
	}
	void Object::Dec_Effect_Life()
	{
		if (this->efList.size() <= 1)
		{
			return;
		}
		for (auto it = this->efList.begin(); it != this->efList.end(); it++)
		{
			if ((*it)->Is_Alive())
			{
				(*it)->Dec_Eff();
			}
			//ライフが０になったエフェクトはヒ-プから開放
			else if((*it)->Get_Type() != BEffect::effType::CLEAR)
			{
				(*it)->Finalize();				
			}
		}
		//ライフが０になったエフェクトはリストから外す
		//return it->effect_Life == 0
		this->efList.remove_if(Effect::Eff_Judge);
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