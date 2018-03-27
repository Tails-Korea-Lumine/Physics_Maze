#include "MyPG.h"
#include "MyGameMain.h"

//ゲーム情報
TaskFlag			actTask, nextTask; 

//-----------------------------------------------------------------------------
//初期化処理
//機能概要：プログラム起動時に１回実行される（素材などの準備を行う）
//-----------------------------------------------------------------------------
void  MyGameMain_Initalize()
{
	actTask  = Task_Non;	//初期状態は実行中のタスクを無効にしておく
	nextTask = Task_Title;	//次のタスクをタイトルにすることで最初にタイトルが実行される
}
//-----------------------------------------------------------------------------
//解放処理
//機能概要：プログラム終了時に１回実行される（素材などの解放を行う）
//-----------------------------------------------------------------------------
void  MyGameMain_Finalize()
{
	DI::GPad_Erase("P1");	//明示的に解放したい場合は
}


//-----------------------------------------------------------------------------
//外部ファイルより
namespace  Title {//タイトルタスク
	extern  void  Initialize();
	extern  TaskFlag  UpDate();
	extern  void  Render2D();
	extern  void  Render3D();
	extern  void  Finalize();
}
namespace  Game {//ゲーム本編タスク
	extern  void  Initialize();
	extern  TaskFlag  UpDate();
	extern  void  Render2D();
	extern  void  Render3D();
	extern  void  Finalize();
}
//以下必要に応じて追加
//-----------------------------------------------------------------------------
//更新処理
//機能概要：ゲームの１フレームに当たる処理
//-----------------------------------------------------------------------------
void  MyGameMain_UpDate()
{
	//次に実行するタスクが、今のタスクと一致しない
	if (actTask != nextTask) {
		//今実行中のタスクの終了処理を呼びだす
		switch (actTask) {
		case Task_Title:	Title::Finalize();	break;
		case Task_Game:		Game::Finalize();	break;
			//以下必要に応じて追加
		}
		//次のタスクに移る
		actTask = nextTask;
		//次のタスクの初期化処理を呼びだす
		switch (actTask) {
		case Task_Title:	Title::Initialize();	break;
		case Task_Game:		Game::Initialize();		break;
			//以下必要に応じて追加
		}
	}
	//現在のタスクの実行・更新処理を呼びだす
	switch (actTask) {
	case Task_Title:	nextTask = Title::UpDate();		break;
	case Task_Game:		nextTask = Game::UpDate();		break;
		//以下必要に応じて追加
	}
}
//-----------------------------------------------------------------------------
//描画処理
//機能概要：ゲームの１フレームに当たる表示処理
//-----------------------------------------------------------------------------
void  MyGameMain_Render2D_BF()
{
}
void  MyGameMain_Render2D_AF()
{
	switch (actTask) {
	case Task_Title:	Title::Render2D();	break;
	case Task_Game:		Game::Render2D();	break;
		//以下必要に応じて追加
	}
}
//-----------------------------------------------------------------------------
//描画処理
//機能概要：ゲームの１フレームに当たる表示処理3D
//-----------------------------------------------------------------------------
void  MyGameMain_Render3D_L0()
{
	switch (actTask) {
	case Task_Title:	Title::Render3D();	break;
	case Task_Game:		Game::Render3D();	break;
		//以下必要に応じて追加
	}
}
void  MyGameMain_Render3D_L1()
{
}
void  MyGameMain_Render3D_L2()
{
}
void  MyGameMain_Render3D_L3()
{
}