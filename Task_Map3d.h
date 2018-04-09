#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//マップ
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include  "Collision.h"

struct mapData
{
	int chip;//マスのマップチップ
	ML::Vec3 pos;//中心点の座標
	ML::Box3D collision_Base;//一個一個の判定範囲
};

namespace Map3d
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("マップ");	//グループ名
	const  string  defName("NoName");	//タスク名
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
		mapData arr[30][30][30];
		int sizeZ, sizeY, sizeX;
		float chipSize;
		ML::Box3D hitBase;//マップ全体の判定範囲
		string chipName[10];
		After_Collision collision_Tri;//ボールのほうに渡す情報

		ML::Vec3 map_Rotation;//マップの回転量
		ML::QT map_QT;
		Collision col;
		

	public:
		//メソッド
		bool Map_Load(string f_);
		bool Map_Check_Hit(const ML::Box3D& hit);
		void Map_Check_Hit(ML::Vec3 pos, float r);//球とマップのあたり判定		
		void Map_Rotate();//Updateで呼ぶ関数
		

		After_Collision is_Collision();
	};
}