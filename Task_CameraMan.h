#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//カメラマン
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

namespace CameraMan
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("カメラマン");	//グループ名
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

		//カメラアングル
		ML::Vec3 angle;
		//アングルの最大値
		float maxAngle;
		//現在カメラの位置
		ML::Vec3 nowPos;
		//カメラの初期位置
		ML::Vec3 initPos;
		//カメラとマップ中心の距離
		float distance;
		//ライトオンまでの時間を計るカウンタ
		int timeCnt;

	public:
		//メソッド
		//ライトをオフする処理
		void Turnoff_the_Light();
		//ライトをオンする処理
		void Turnon_the_Light();
		//3秒がたったのかを判定
		bool It_Passed_3sec() const;
		//ゲームスタート前の演出
		void ProMotion();
		//カメラの移動操作
		void Camera_Move();
	};
}