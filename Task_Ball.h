#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//ボール
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include "Sphere.h"

namespace Ball
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("ボール");	//グループ名
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
		string meshName;
		string hit_Se_Name;
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

		//現在位置(中心点)
		//ML::Vec3 pos;
		//移動量
		ML::Vec3 speed;
		//判定範囲
		Shape3D* sphere;

		//重さ
		float m;
		
		//接触しているかを判定するフラグ
		std::list<string> collision_Flag;

		//テレポートをしたのかを判別するフラグ
		bool teleportation_Flag;

		//メソッド
		//一個前のフレームで衝突したかを返す
		bool Is_Collision(const string&) const;
		//転がる表現のために回転させる
		void Rotate(const unsigned int& precision);
		//移動
		void Move_Ball(const unsigned int& precision);

	public:
		//その時のボールの情報を返す関数
		//位置
		ML::Vec3 Get_Pos() const;
		//半直径
		float Get_Radious() const;
		//速度
		ML::Vec3 Get_Speed() const;
			

		//テレポートしたかを確認するタスク
		bool Is_Teleport_Now() const;
		//あたり判定による物理処理
		void Physics_Actioin(const unsigned int& precision);
		//位置補正用回転関数 引数 : (マップのフレーム回転量)
		void Fix_Position_for_Rotate(const ML::QT& qt);
		//テレポート(指定した場所に即移動する)引数 : (目的地)
		void Teleportation(const ML::Vec3& objectPos);

		//あたり判定範囲をもらう
		Shape3D* Get_Collision_Area();
	};
}