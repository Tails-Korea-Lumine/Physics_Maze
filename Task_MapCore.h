#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//マップのコア
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include "Collision.h"
#include "Bbox.h"
#include "Task_title.h"

namespace Map_Core
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("マップ");	//グループ名
	const  string  defName("Core");	//タスク名
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
		//呼び出すメッシュの名前(バリアーは表示しないので1個だけ
		string meshName;
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
		static  Object::SP  Create(bool flagGameEnginePushBack_, Difficult_Range di);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize(Difficult_Range di);
		bool  B_Finalize();
		bool  Initialize(Difficult_Range di);	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate();		//「実行」１フレーム毎に行う処理
		void  Render2D_AF();	//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
		void  Render3D_L0();
		//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	
		//追加したい変数・メソッドはここに追加する
		//変数
		//真ん中のキューブと外に飛ばされないようにする外壁
		Bbox core, barrier;		
		//マップの大きさ
		int mapSize;
		//ボールに渡す情報	
		std::vector< After_Collision> col_Poligons;		
		//回転量
		ML::QT map_QT;		
		//メソッド
		//クォータニオンを更新する関数
		void UpDate_Quartanion(const ML::QT& qt);
	public:
		//回転
		void Rotate_Core_and_Barrier(const ML::QT& qt);
		//球とマップのあたり判定
		void Core_Check_Hit(std::vector<ML::Vec3>& all_Points, const ML::Vec3& pos, const float& r, const ML::Vec3& speed);
		
	};
}