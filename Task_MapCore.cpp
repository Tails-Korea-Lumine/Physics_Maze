//-------------------------------------------------------------------
//マップのコア
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_MapCore.h"
#include  "Task_Ball.h"
#include "Wall.h"

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
		this->core = new Wall(ge->Map_center, ML::Vec3(50 * this->mapSize, 50 * this->mapSize, 50 * this->mapSize), this->map_QT,"core");

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

		for (size_t i = 0; i < 6; i++)
		{
			this->barrier[i] = new Wall(ge->Map_center + ML::Vec3(0,50*(this->mapSize+3),0), ML::Vec3(50 * (this->mapSize + 2), 50 * (this->mapSize + 2), 50 * (this->mapSize + 2)), this->map_QT, "barrier" + to_string(i));
			ML::Mat4x4 matR;
			matR.Identity();
			//初期位置まで回転
			switch (i)
			{
			case 0:
				continue;
				break;
			case 1:
				//回転行列生成
				D3DXMatrixAffineTransformation(&matR, 1.0f, &ge->Map_center, &ML::QT(this->map_QT * ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(90))), NULL);
				//回転
				this->barrier[i]->Rotate_Box(&matR, ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(90)));
				break;
			case 2:
				//回転行列生成
				D3DXMatrixAffineTransformation(&matR, 1.0f, &ge->Map_center, &ML::QT(this->map_QT * ML::QT(ML::Vec3(0, 0, 1), ML::ToRadian(90))), NULL);
				//回転
				this->barrier[i]->Rotate_Box(&matR, ML::QT(ML::Vec3(0, 0, 1), ML::ToRadian(90)));
				break;
			case 3:
				//回転行列生成
				D3DXMatrixAffineTransformation(&matR, 1.0f, &ge->Map_center, &ML::QT(this->map_QT * ML::QT(ML::Vec3(0, 0, 1), ML::ToRadian(-90))), NULL);
				//回転
				this->barrier[i]->Rotate_Box(&matR, ML::QT(ML::Vec3(0, 0, 1), ML::ToRadian(-90)));
				break;
			case 4:
				//回転行列生成
				D3DXMatrixAffineTransformation(&matR, 1.0f, &ge->Map_center, &ML::QT(this->map_QT * ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(-90))), NULL);
				//回転
				this->barrier[i]->Rotate_Box(&matR, ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(-90)));
				break;
			case 5:
				//回転行列生成
				D3DXMatrixAffineTransformation(&matR, 1.0f, &ge->Map_center, &ML::QT(this->map_QT * ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(180))), NULL);
				//回転
				this->barrier[i]->Rotate_Box(&matR, ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(180)));
				break;
			}
		}

		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		delete core;
		for (size_t i = 0; i < 6; i++)
		{
			delete this->barrier[i];
		}

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
		D3DXMatrixAffineTransformation(&matW, this->mapSize * 100.0f, NULL, &this->map_QT,&this->core->Get_Pos());

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
		this->core->Rotate_Box(&matR, qt);
		for (size_t i = 0; i < 6; i++)
		{
			this->barrier[i]->Rotate_Box(&matR, qt);
		}
	}

	//------------------------------------------------------------------------------------
	//あたり判定

	void Object::Core_Check_Hit(Shape3D* ball)
	{
		std::vector<Collision_Data> col_Poligons;
		//接触三角形を判定前にクリアする	
		col_Poligons.clear();

		//判定関数
		//std::vector<After_Collision> poligonC
		this->core->Collision_Action(&col_Poligons, ball);
		
		for (size_t i = 0; i < 6; i++)
		{
			this->barrier[i]->Collision_Action(&col_Poligons, ball);
		}

		//全体衝突結果に保存する
		for (auto& c : col_Poligons)
		{
			ge->collision_Result.push_back(c);
		}		
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