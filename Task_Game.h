#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//ゲーム本編
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include "Task_title.h"

namespace Game
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("ゲーム");	//グループ名
	const  string  defName("本編");	//タスク名
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
		string BG_mesh;
		string bgmName;
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
		//タスク消滅までのカウンタとフラグ	
		int countdown;
		bool countdownFlag;

		//背景を回転させる角度
		float angleY;

		Difficult_Range nowdi;

		//遊んだプレー時間Resultタスクに渡すもの
		int timeCnt;
		//bgm volume
		long vol;

	public:
		//カウントダウンフラグを返す関数
		bool Is_Count_Down() const;
		//クリアフラグを立てる関数
		void Game_Clear();
		//クリアしたのかを返す関数
		bool IS_Cleared() const;
		//操作可能かを返す関数
		bool GET_READY() const;
		//画面切り替えが終わったのかを確認
		bool Count_Down_Over() const;
		//ゲームクリア演出を始める
		bool Start_Clear_Promotion();
		//カメラ演出が終わったのかを確認
		bool Is_Camera_Promotion_Over();
		//BGMヴォリュームをフェイドアウトさせる
		void BGM_Fade_Out();

	};
}