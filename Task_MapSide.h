#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//マップ
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include "Collision.h"
#include "Bbox.h"
#include "Task_title.h"



namespace Map_Side
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("マップ");	//グループ名
	const  string  defName("Side");	//タスク名
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
		Bbox* arr[8][8];
		size_t sizeZ, sizeX;
		int mapSize;
		int sideNumber;
		float chipSize;
		int gimicCnt;
		float rendering_Judge;
		string chipName[10];

		//法線ベクトル
		ML::Vec3 Normal_Side;
		//回転量
		ML::QT map_QT;		
		
	
		//メソッド
		bool Map_Load(string f_);
		//レンダリングするかを確認するメソッド
		bool Is_Need_Render();
		bool Is_Need_Render(const unsigned int& z ,const unsigned int& index);
		//連続していて使えない面を探す
		void Check_Unusable_Side();
		//配列ソート及びボールをスタート位置に置く
		//void Array_Sorting();
		//ボールタスクのフラグにIDを組み込める
		//void Insert_Id_To_Ball();
		//クォータニオンを更新する関数
		void UpDate_Quartanion(const ML::QT& qt);
		
	public:
		//球とマップのあたり判定
		bool Map_Check_Hit(Shape3D* ball);
		//マップ回転
		void Map_Rotate(const ML::QT& qt);
		
		//マップの法線ベクトルを返す処理
		void Get_Normal_Side(ML::Vec3*) const;		
	};
}