#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include "Task_cursor.h"

enum Difficult_Range
{
	Easy = -1,
	Normal = 1,
	Hard = 3,
};

enum Tutorial_Column
{
	OutLine = -1,
	Control = 1,
	Obstacle = 3,
};

namespace Title
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("タイトル");	//グループ名
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
		string Title_Name_Image[12];
		string press_Any_Key_Image;
		string Difficalt_Image[3];
		string Column_Image[3];
		string start_Image;
		string tutorial_Image;
		string select_Guide_Image;
		string BG_ImageName;
		string bgm_Title;
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
		//タイトル文字の移動および描画座標
		ML::Vec2 Title_Name[12];
		//メニュー選択中なのかを確認するフラグ
		bool select_now;
		//時間確認用のカウンタ
		int timeCnt;
		//メニューを左右に動かせる移動量
		int moving_Menu;
		//タイトル名を上に動かせる移動量
		int moving_Title_Name;
		//次に移動するタスクを決める変数
		POINT next_Task_Index;
		//現在メニュー
		nowMenu n;
		//グラデーションウェーブ用
		float waveR, waveG, waveB;
		float dgreeR, dgreeG, dgreeB;
		//bgm volume
		long vol;
	
		//メソッド
		//ゲームパッドから入力をもらえるすべてのものを確認
		bool Press_Any_Key() const;
		//PAKを描画しなくてもいいのかを判別する
		bool Is_Need_to_Draw_PAK() const;
		//タイトル名位置更新
		void UpDate_Title_Name();
		//タイトル名描画関数
		void Draw_Title_Name() const;
		//press any keyを描画
		void Draw_PAK() const;
		//メニューを描画
		void Draw_Menu() const;
		//2番目のメニュー(難易度またはテュートリアル目次)を描画
		void Draw_Dif_Col(nowMenu now) const;
		//色をだんだん変更する
		void Color_Wave();

	public:
		//次のタスクを決める関数
		void I_Select(POINT select);
		//BGMヴォリュームをフェイドアウトさせる
		void BGM_Fade_Out();
	};
}