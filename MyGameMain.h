#pragma once
#include "MyPG.h"

enum TaskFlag
{
	Task_Non,		//タスクが未選択状態
	Task_Title,		//タイトル
	Task_Game,		//ゲーム本編

	//以下必要に応じて追加
};

extern  void  MyGameMain_Finalize( );
extern  void  MyGameMain_Initalize( );
extern  void  MyGameMain_UpDate( );
extern  void  MyGameMain_Render2D_BF();
extern  void  MyGameMain_Render2D_AF();
extern  void  MyGameMain_Render3D_L0();
extern  void  MyGameMain_Render3D_L1();
extern  void  MyGameMain_Render3D_L2();
extern  void  MyGameMain_Render3D_L3();
