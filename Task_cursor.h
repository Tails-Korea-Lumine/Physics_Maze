#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//カーソル
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

enum nowMenu
{
	clear = -10,
	Start_Tutorial = 0,
	difficult = -1,
	TutorialCalum = 1,
};

namespace Cursor
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("カーソル");	//グループ名
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
		string imageName;
		string seMoveName;
		string seSelectName;
		string seCancelName;
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
		ML::Vec2 pos;//描画する座標
		ML::Vec2 destination;//目的地
		POINT select_pos;
		nowMenu now;
		//タスク消滅までのカウンタとフラグ
		int countdown;
		bool countdownFlag;
		//各メニューの範囲
		//0番がmin、1番がmax
		int first_Menu_Range[2];
		int last_Menu_Range[2];

	public:
		//メソッド
		int Move_Cursor(POINT& select);
		//現在メニューを返す関数
		nowMenu Get_Now_Menu();
		//カウントダウンフラグを返す関数
		bool Is_Count_Down() const;
		//画面切り替えが終わったのかを確認
		bool Count_Down_Over() const;
		//選択処理
		void Click();
	};
}