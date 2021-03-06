#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//物理運動マネージャ
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"


namespace Physics_Manager
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("物理運動");	//グループ名
	const  string  defName("Manager");	//タスク名
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
		string esasing_Name_QTxp;
		string esasing_Name_QTxm;
		string esasing_Name_QTyp;
		string esasing_Name_QTym;
		string esasing_Name_QTzp;
		string esasing_Name_QTzm;
		//回転軸
		ML::Vec3 anckerX;
		ML::Vec3 anckerY;
		ML::Vec3 anckerZ;
			
		//メソッド
		//マップの処理をいっぺんに管理する 引数 : (精密度)
		void Managing(const unsigned int delicate);
		//入力による処理
		void Input_Analog_Action();
		//クォータニオン計算
		ML::QT Calculate_Frame_Quatanion(const float& precision);
		//カメラ移動の法線ベクトル選択処理
		ML::Vec3 Select_Nearest_Side(const std::vector<ML::Vec3>& normals, const ML::Vec3& b);
	public:
		//回転軸再設定
		void Ancker_Calculating();
	};
}