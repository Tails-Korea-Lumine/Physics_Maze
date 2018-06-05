#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//UserInterface
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

enum WipeInOut
{
	Wipe_In = 0,
	Wipe_Out = 1,
};

namespace  UI
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("UI");	//グループ名
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
		//共有する変数はここに追加する\
		//段々黒い幕が画面を隠す
		string wipeIn_Img;
		//段々黒い幕が画面を離れる
		string wipeOut_Img;
		//時間表示
		string uI_Time_Img;
		//数字表
		string number_Img;

		
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
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇	
		//追加したい変数・メソッドはここに追加する	
		//変数

		//絵で表示する文字の位置
		//時間
		ML::Vec2 ui_Time;

		//数字表の情報
		//時間表示用：まで含めて11個
		ML::Box2D src_Number[11];

		//時間用カウンタ
		int timeCnt;
		//幕に使うフラグ
		WipeInOut wipe;
		//透明度
		float wipe_Alpha;

		
	public:
		//メソッド
		//幕の描画
		void Draw_Wipe_InOut();
		//時間の描画
		void Draw_Time() const;
		//画面明かしを始める
		void Start_WipeIn();		
	};
}