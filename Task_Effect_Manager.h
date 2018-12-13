#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//エフェクトマネージャー
//-------------------------------------------------------------------
#include "BEffect.h"

namespace EffectManager
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("エフェクト");	//グループ名
	const  string  defName("マネージャー");	//タスク名
	//-------------------------------------------------------------------
	class  Resource
	{
		bool  Initialize();
		bool  Finalize();
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
		//共有する変数はここに追加する
		string mesh_Telein;
		string mesh_Teleout;
		string mesh_Gameclear;
		string seTeleportIn;
		string seTeleportOut;
	};
	//-------------------------------------------------------------------
	class  Object : public  BTask
	{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//生成窓口 引数はtrueでタスクシステムへ自動登録
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate();		//「実行」１フレーム毎に行う処理
		void  Render2D_AF();	//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
		void  Render3D_L0();
		//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	
		//追加したい変数・メソッドはここに追加する
		//変数		
		//動いているエフェクトリスト
		std::list<BEffect*> play_Effect_List;

		//メソッド
		//全体のライフを1フレームに1ずつ減少させる
		void Dec_Effect_Life();		
	public:
		//外部からのエフェクト追加命令
		//移動しないエフェクト
		//引数 : (生成位置、精製角度、エフェクトの種類)
		void Add_Effect(const ML::Vec3& pos, const ML::Vec3& angle, const BEffect::effType& handle);
		//移動をするエフェクト
		//引数 : (生成位置、目的地、精製角度、エフェクトの種類)
		void Add_Effect(const ML::Vec3& pos, const ML::Vec3& target, const ML::Vec3& angle, const BEffect::effType& handle);			

	};
}