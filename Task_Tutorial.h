#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//テュートリアル
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include "Task_title.h"

namespace Tutorial
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("テュートリアル");	//グループ名
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
		//選択可能な目次が3個
		string imageName[3];
		string Bg_Img;
		string bgmName;
		string guide_Img[2];
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
		static  Object::SP  Create(bool flagGameEnginePushBack_, Tutorial_Column tc);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize(Tutorial_Column tc);
		bool  B_Finalize();
		bool  Initialize(Tutorial_Column tc);	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate();		//「実行」１フレーム毎に行う処理
		void  Render2D_AF();	//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
		void  Render3D_L0();
		//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	
		//追加したい変数・メソッドはここに追加する

		int countDown;
		bool countDown_Flag;

		int timeCnt;
		bool page_Change_Flag;

		int posx[3];//チュートリアルページ切り替え用
		int posy;//テュートリアルページの表示位置-528=<posy=<720 変更可能性あり

		//bgm volume
		long vol;
		
		Tutorial_Column column;

		//BGMヴォリュームをフェイドアウトさせる
		void BGM_Fade_Out();

	public:
		void Page_Chage(bool page_Move_Right);
		bool Can_I_Change_the_Page() const;
		//自分消滅までのカウントダウン開始を確認
		bool Is_Count_Down() const;
		//カウントダウンが終わったのかを確認
		bool Count_Down_Over() const;
	};
}