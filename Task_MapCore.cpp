//-------------------------------------------------------------------
//マップのコア
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_MapCore.h"
#include  "Task_Ball.h"

namespace  Map_Core
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->meshName = "MapCore";
		
		DG::Mesh_CreateFromSOBFile(this->meshName, "./data/mesh/Core.sob");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Mesh_Erase(this->meshName);
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize(Difficult_Range di)
	{
		//ID登録のためにボールタスクをもらっておく
		auto ball = ge->GetTask_One_G<Ball::Object>("ボール");
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化	
		this->mapSize = 8;//基本は8X8難易度によって増加される	
		this->map_QT = ML::QT(0.0f);
		
		//コアの初期化
		this->core = Bbox(BoxType::Core, ge->Map_center, ML::Box3D(-50 * this->mapSize, -50 * this->mapSize, -50 * this->mapSize, 100 * this->mapSize, 100 * this->mapSize, 100 * this->mapSize), this->map_QT,"core");
		ball->Set_Id_And_Flag("core");
		//バリアの初期化
		//各面ごとバリアを置いたver0.5
		////各面に価するあたり判定範囲
		//ML::Box3D XZ, XY, YZ;
		//XZ = ML::Box3D(-100 * (this->mapSize -2), -50 , -100 * (this->mapSize -2), 100 * (this->mapSize + 4), 100, 100 * (this->mapSize + 4));
		//XY = ML::Box3D(-100 * (this->mapSize - 2), -100 * (this->mapSize - 2), -50 , 100 * (this->mapSize + 4), 100 * (this->mapSize + 4), 100);
		//YZ = ML::Box3D(-50 , -100 * (this->mapSize - 2), -100 * (this->mapSize - 2), 100, 100 * (this->mapSize + 4), 100 * (this->mapSize + 4));
		////面ごとの初期位置		
		//this->b_ini_pos[0] = ge->Map_center + ML::Vec3(0.0f, 53.0f * (this->mapSize + 3), 0.0f);
		//this->b_ini_pos[1] = ge->Map_center + ML::Vec3(0.0f, 0.0f, 53.0f * (this->mapSize + 3));
		//this->b_ini_pos[2] = ge->Map_center + ML::Vec3(-53.0f * (this->mapSize + 3), 0.0f, 0.0f);
		//this->b_ini_pos[3] = ge->Map_center + ML::Vec3(53.0f * (this->mapSize + 3), 0.0f, 0.0f);
		//this->b_ini_pos[4] = ge->Map_center + ML::Vec3(0.0f, 0.0f, -53.0f * (this->mapSize + 3));
		//this->b_ini_pos[5] = ge->Map_center + ML::Vec3(0.0f, -53.0f * (this->mapSize + 3), 0.0f);
		////Bbox初期化
		//this->barrier[0] = Bbox(BoxType::Barrier, b_ini_pos[0] , XZ, this->map_QT);
		//this->barrier[1] = Bbox(BoxType::Barrier, b_ini_pos[1] , XY, this->map_QT);
		//this->barrier[2] = Bbox(BoxType::Barrier, b_ini_pos[2] , YZ, this->map_QT);
		//this->barrier[3] = Bbox(BoxType::Barrier, b_ini_pos[3] , YZ, this->map_QT);
		//this->barrier[4] = Bbox(BoxType::Barrier, b_ini_pos[4] , XY, this->map_QT);
		//this->barrier[5] = Bbox(BoxType::Barrier, b_ini_pos[5] , XZ, this->map_QT);

		//逆向きのバリア一個ですますver0.6
		this->barrier = Bbox(BoxType::Barrier, ge->Map_center, ML::Box3D(50 * (this->mapSize+2), 50 * (this->mapSize+2), 50 * (this->mapSize+2), -100 * (this->mapSize+2), -100 * (this->mapSize+2), -100 * (this->mapSize+2)), this->map_QT,"barrier");
		ball->Set_Id_And_Flag("barrier");

		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		this->col_Poligons.clear();


		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			//★引き継ぎタスクの生成
			//auto nextTask = Game::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		//auto ball = ge->GetTask_One_G<Ball::Object>("ボール");
		//this->frame_QT = ML::QT(0.0f);
		//auto in1 = DI::GPad_GetState("P1");

		////あたり判定の精密調整を仕掛けた ver0.5
		//
		//{
		//	//スティックが倒された量を更新
		//	if (in1.B1.on)
		//	{
		//		//スティックで入力
		//		if (in1.LStick.axis.x != 0)
		//		{
		//			this->frame_QT = ML::QT(ML::Vec3(0, 0, 1), ML::ToRadian(-in1.LStick.axis.x ));
		//			this->map_QT *= this->frame_QT;
		//		}
		//	}
		//	//押されていない時はY軸回転とX軸回転
		//	else
		//	{

		//		if (in1.LStick.axis.y != 0)
		//		{
		//			this->frame_QT = ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(-in1.LStick.axis.y));
		//			this->map_QT *= this->frame_QT;
		//		}

		//		if (in1.LStick.axis.x != 0)
		//		{
		//			this->frame_QT = ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(-in1.LStick.axis.x));
		//			this->map_QT *= this->frame_QT;
		//		}
		//	}

		//	//ワールド回転量に反映
		//	ge->World_Rotation = this->map_QT;


		//	//回転
		//	this->Rotate_Core_and_Barrier();


		//	//あたり判定は毎回マップのほうで行う
		//	//ボールの情報を修得

		//	if (ball != nullptr)
		//	{
		//		this->Core_Check_Hit(ball->Get_Pos(), ball->Get_Radious(), ball->Get_Speed());
		//	}
		//	//位置補正を仕掛ける
		//	/*for (auto p : this->col_Poligons)
		//	{
		//		if (p.collision_Flag)
		//		{
		//			ball->Fix_Position_for_Rotate(this->frame_QT);
		//			break;
		//		}
		//	}*/
		//}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{

	}

	void  Object::Render3D_L0()
	{
		//レンダリング処理、バリアはレンダリングをしない
		ML::Mat4x4 matW;

		//アフィン変換
		D3DXMatrixAffineTransformation(&matW, this->mapSize * 100.0f, NULL, &this->map_QT,&this->core.Get_Pos());

		DG::EffectState().param.matWorld = matW;

		DG::Mesh_Draw(this->res->meshName);

	}

	//-----------------------------------------------------------------------------------
	//追加メソッド

	//回転
	void Object::Rotate_Core_and_Barrier(const ML::QT& qt)
	{
		//全体の回転値更新
		this->UpDate_Quartanion(qt);

		ML::Mat4x4 matR;
		//matRをアフィン変換行列に作る
		D3DXMatrixAffineTransformation(&matR, 1.0f, &ge->Map_center, &qt,NULL);
		
		//回転開始
		ML::Vec3 temp = matR.TransformCoord(ge->Map_center);
		this->core.Rotate_Box(&matR, qt);
		
		this->barrier.Rotate_Box(&matR, qt);
	}

	//------------------------------------------------------------------------------------
	//あたり判定

	void Object::Core_Check_Hit(std::vector<ML::Vec3>& all_Points, const ML::Vec3& pos, const float& r, const ML::Vec3& speed)
	{
		//接触三角形を判定前にクリアする	
		this->col_Poligons.clear();

		//判定関数
		//std::vector<After_Collision> poligonC
		this->core.Get_Collision_Poligon(&this->col_Poligons, all_Points, pos, r, speed);
		
		this->barrier.Get_Collision_Poligon(&this->col_Poligons, all_Points, pos, r, speed);

		//全体衝突結果に保存する
		for (auto& c : this->col_Poligons)
		{
			ge->collision_Result.push_back(c);
		}
		//Graviation_Pullの削除によって戻り値が不要になった
		//return ball_was_Collision_to_Core;
	}
	//-------------------------------------------------------------------------------------------
	//クォータニオンを更新する関数
	void Object::UpDate_Quartanion(const ML::QT& qt)
	{
		this->map_QT *= qt;
	}
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_, Difficult_Range di)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
			}
			if (!ob->B_Initialize(di)) {
				ob->Kill();//イニシャライズに失敗したらKill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize(Difficult_Range di)
	{
		return  this->Initialize(di);
	}
	//-------------------------------------------------------------------
	Object::~Object() { this->B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object() {	}
	//-------------------------------------------------------------------
	//リソースクラスの生成
	Resource::SP  Resource::Create()
	{
		if (auto sp = instance.lock()) {
			return sp;
		}
		else {
			sp = Resource::SP(new  Resource());
			if (sp) {
				sp->Initialize();
				instance = sp;
			}
			return sp;
		}
	}
	//-------------------------------------------------------------------
	Resource::Resource() {}
	//-------------------------------------------------------------------
	Resource::~Resource() { this->Finalize(); }
}