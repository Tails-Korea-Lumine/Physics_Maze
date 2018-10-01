#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//マップ
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include  "Collision.h"
#include "Bbox.h"
#include "Task_title.h"



namespace MapFence
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("マップ");	//グループ名
	const  string  defName("Fence");	//タスク名
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
	};
	//-------------------------------------------------------------------
	class  Object : public BTask
	{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//生成窓口 引数はtrueでタスクシステムへ自動登録
		static  Object::SP  Create(bool flagGameEnginePushBack_,int sideNum, Difficult_Range di);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize(int sideNum, Difficult_Range di);
		bool  B_Finalize();
		bool  Initialize(int sideNum, Difficult_Range di);	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate();		//「実行」１フレーム毎に行う処理
		void  Render2D_AF();	//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
		void  Render3D_L0();
		//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	
		//追加したい変数・メソッドはここに追加する	
		//変数
		Bbox arr[10][10];
		int sizeX, sizeY;
		int mapSize;
		int fenceNumber;
		float chipSize;
		string chipName[10];		
		//ボールのほうに渡す情報
		std::vector< After_Collision> col_Poligons;
		//回転量
		ML::QT map_QT;		
		
	public:
		//メソッド
		//外部ファイルからの読み込み
		bool Map_Load(string f_);
		//球とマップのあたり判定
		void Map_Check_Hit(const ML::Vec3& pos, const float& r, const ML::Vec3& speed);
		//マップの回転
		void Map_Rotate();
		//あたり判定の結果を返す関数
		void Get_Collision_Poligon(std::vector<After_Collision>* result) const;
		//クォータニオンを更新する関数
		void UpDate_Quartanion(const ML::QT& qt);
		
	};
}