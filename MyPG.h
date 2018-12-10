#pragma once
#include "GameEngine_Ver3_7.h"
#include <fstream>
#include <sstream>
//#include "Collision.h"
#include "MyMath.h"
#include "Dm2008_Ver1_3.h"
#include "Teleportation_Manager.h"
#include "Task_Effect_Manager.h"
#include "Task_Game.h"


//ゲーム内全域で使用する構造体などを定義する
//-----------------------------------------------------------------------------------------------

//三角形の基本情報
//頂点と法線ベクトル
struct Triangle
{
	ML::Vec3 a, b, c;
	ML::Vec3 normal;
};

//あたり判定の後に返す構造体
struct Collision_Data
{
	bool collision_Flag;
	string collision_Id;
	ML::Vec3 normal;

	Collision_Data()
	{
		this->collision_Flag = false;
		this->collision_Id = "";
		this->normal = ML::Vec3(0, 0, 0);
	}

	//比較演算子オーバーロード
	bool operator==(const Collision_Data& ac)
	{
		float check;
		MyMath::Vector_Dot(&check, this->normal, ac.normal);
		return check >= _CMATH_::cosf(ML::ToRadian(359));
	}

	bool operator!=(const Collision_Data& ac)
	{
		return *this == ac ? false : true;
	}
};



//-----------------------------------------------------------------------------------------------
namespace  MyPG
{
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	//----------------------------------------------
	//カメラ基本形
	class Camera
	{
		Camera( ){ }
		Camera(
				const ML::Vec3&		tg_,	//	被写体の位置
				const ML::Vec3&		pos_,	//	カメラの位置
				const ML::Vec3&		up_,	//	カメラの上方向を示すベクトル（大体Ｙ＋固定）
				float				fov_,	//	視野角
				float				np_,	//	前クリップ平面（これより前は映らない）
				float				fp_,	//	後クリップ平面（これより後ろは映らない）
				float				asp_);	//	アスペクト比（画面の比率に合わせる　横÷縦）			
	public:
	//	ビュー情報（方向関連）
		ML::Vec3 target;			//	被写体の位置
		ML::Vec3 pos;			//	カメラの位置
		ML::Vec3 up;				//	カメラの上方向を示すベクトル（大体Ｙ＋固定）
	//	射影情報（視野範囲関連）
		float fov;					//	視野角
		float nearPlane;			//	前クリップ平面（これより前は映らない）
		float forePlane;			//	後クリップ平面（これより後ろは映らない）
		float aspect;				//	アスペクト比（画面の比率に合わせる　横÷縦）
	//	行列情報
		ML::Mat4x4  matView, matProj;
		~Camera( );
		typedef shared_ptr<Camera> SP;
	//	カメラを生成する
		static SP Create(
				const ML::Vec3&		tg_,	//	被写体の位置
				const ML::Vec3&		pos_,	//	カメラの位置
				const ML::Vec3&		up_,	//	カメラの上方向を示すベクトル（大体Ｙ＋固定）
				float				fov_,	//	視野角
				float				np_,	//	前クリップ平面（これより前は映らない）
				float				fp_,	//	後クリップ平面（これより後ろは映らない）
				float				asp_);	//	アスペクト比（画面の比率に合わせる　横÷縦）	
	//	カメラの設定
		void UpDate( );
	};
	//----------------------------------------------
	class MyGameEngine : public GameEngine
	{
	public:
		MyGameEngine(const int& sizeX, const int& sizeY );
	//ゲームエンジンに追加したものの初期化と開放
		bool Initialize(HWND wnd_, IDXGIAdapter* adp);
		~MyGameEngine( );
	//ゲームエンジンに追加したもののステップ処理
		void UpDate( );

	//3DPG1対応により追加
		//2D描画環境のデフォルトパラメータ設定
		void Set2DRenderState(DWORD l_);
		//3D描画環境のデフォルトパラメータ設定
		void Set3DRenderState(DWORD l_);



	//ゲームエンジンに追加したいものは下に加える
	//----------------------------------------------
		MyPG::Camera::SP		camera[4];		//	カメラ
		D3D10_VIEWPORT			vp3D_L0;
		D3D10_VIEWPORT			vp3D_L1;
		D3D10_VIEWPORT			vp3D_L2;
		D3D10_VIEWPORT			vp3D_L3;
		D3D10_VIEWPORT			vp3D_BF;
		D3D10_VIEWPORT			vp3D_AF;		
		
		bool gameClearFlag;
		bool getReadyFlag;

		//ワールド回転量
		//ML::QT World_Rotation; 
		//マップ全体の中心
		ML::Vec3 Map_center;
		//あたり判定の結果
		std::list<Collision_Data> collision_Result;
		//テレポーテーションマネージャー
		Teleportation_Manager TM;
		//エフェクトマネージャー
		weak_ptr <EffectManager::Object> eff_Manager;
		//本編を常駐させる
		weak_ptr<Game::Object> game;
	//----------------------------------------------
	};
}
extern MyPG::MyGameEngine* ge;


