//-------------------------------------------------------------------
//タイトル画面
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
		//仮のメッシュ
		DG::Mesh_CreateFromSOBFile(this->meshName, "./data/mesh/box3.sob");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{		
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化	
		this->mapSize = 8;//基本は8X8難易度によって増加される(2018/04/21)		
		this->map_QT = ML::QT(0.0f);
		
		//コアの初期化
		this->core = Bbox(BoxType::Core, ge->Map_center, ML::Box3D(-50 * this->mapSize, -50 * this->mapSize, -50 * this->mapSize, 100 * this->mapSize, 100 * this->mapSize, 100 * this->mapSize), this->map_QT);
		//バリアの初期化
		ML::Box3D XZ, XY, YZ;
		XZ = ML::Box3D(-100 * (this->mapSize -2), -50 , -100 * (this->mapSize -2), 100 * (this->mapSize + 4), 100, 100 * (this->mapSize + 4));
		XY = ML::Box3D(-100 * (this->mapSize - 2), -100 * (this->mapSize - 2), -50 , 100 * (this->mapSize + 4), 100 * (this->mapSize + 4), 100);
		YZ = ML::Box3D(-50 , -100 * (this->mapSize - 2), -100 * (this->mapSize - 2), 100, 100 * (this->mapSize + 4), 100 * (this->mapSize + 4));

		this->b_ini_pos[0] = ge->Map_center + ML::Vec3(0, 50 * (this->mapSize + 4), 0);
		this->b_ini_pos[1] = ge->Map_center + ML::Vec3(0, 0, 50 * (this->mapSize + 4));
		this->b_ini_pos[2] = ge->Map_center + ML::Vec3(-50 * (this->mapSize + 4), 0, 0);
		this->b_ini_pos[3] = ge->Map_center + ML::Vec3(50 * (this->mapSize + 4), 0, 0);
		this->b_ini_pos[4] = ge->Map_center + ML::Vec3(0, 0, -50 * (this->mapSize + 4));
		this->b_ini_pos[5] = ge->Map_center + ML::Vec3(0, -50 * (this->mapSize + 4), 0);

		this->barrier[0] = Bbox(BoxType::Barrier, b_ini_pos[0] , XZ, this->map_QT);
		this->barrier[1] = Bbox(BoxType::Barrier, b_ini_pos[1] , XY, this->map_QT);
		this->barrier[2] = Bbox(BoxType::Barrier, b_ini_pos[2] , YZ, this->map_QT);
		this->barrier[3] = Bbox(BoxType::Barrier, b_ini_pos[3] , YZ, this->map_QT);
		this->barrier[4] = Bbox(BoxType::Barrier, b_ini_pos[4] , XY, this->map_QT);
		this->barrier[5] = Bbox(BoxType::Barrier, b_ini_pos[5] , XZ, this->map_QT);

		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放


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
		auto in1 = DI::GPad_GetState("P1");
		//スティックが倒された量を更新
		if (in1.B1.on)
		{
			//スティックで入力
			if (in1.LStick.axis.x != 0)
			{
				ML::QT temp_qt(ML::Vec3(0, 0, 1), ML::ToRadian(-in1.LStick.axis.x));
				this->map_QT *= temp_qt;
			}
		}
		//押されていない時はY軸回転とX軸回転
		else
		{

			if (in1.LStick.axis.y != 0)
			{
				ML::QT temp_qt(ML::Vec3(1, 0, 0), ML::ToRadian(-in1.LStick.axis.y));
				this->map_QT *= temp_qt;
			}

			if (in1.LStick.axis.x != 0)
			{
				ML::QT temp_qt(ML::Vec3(0, 1, 0), ML::ToRadian(-in1.LStick.axis.x));
				this->map_QT *= temp_qt;
			}
		}

		//ワールド回転量に反映
		ge->World_Rotation = this->map_QT;


		//回転
		this->Rotate_Core_and_Barrier();


		//あたり判定は毎回マップのほうで行う
		//ボールの情報を修得
		auto ball = ge->GetTask_One_G<Ball::Object>("ボール");

		this->Core_Check_Hit(ball->pos, ball->r, ball->speed);

	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{

	}

	void  Object::Render3D_L0()
	{
		//レンダリング処理、バリアはレンダリングをしない
		ML::Mat4x4 matS, matR, matT;

		matS.Scaling(this->mapSize);
		matR.RotationQuaternion(this->map_QT);
		/*for (int i = 0; i < 6; i++)
		{
			matT.Translation(this->barrier[i].Get_Pos());

			DG::EffectState().param.matWorld = matS * matR * matT;

			DG::Mesh_Draw(this->res->meshName);
		}*/
		matT.Translation(this->core.Get_Pos());

		DG::EffectState().param.matWorld = matS * matR * matT;

		DG::Mesh_Draw(this->res->meshName);

	}

	//-----------------------------------------------------------------------------------
	//追加メソッド

	//回転
	void Object::Rotate_Core_and_Barrier()
	{
		ML::Mat4x4 matR;
		//matRをアフィン変換行列に作る
		D3DXMatrixAffineTransformation(&matR, 1.0f, &ge->Map_center, &this->map_QT,NULL);
		
		//回転開始
		ML::Vec3 temp = matR.TransformCoord(ge->Map_center);
		this->core.Rotate_Box(temp, this->map_QT);
		for (int b = 0; b < 6; b++)
		{
			ML::Vec3 btemp = matR.TransformCoord(this->b_ini_pos[b]);
			this->barrier[b].Rotate_Box(btemp, this->map_QT);
		}
	}

	//------------------------------------------------------------------------------------
	//あたり判定

	void Object::Core_Check_Hit(ML::Vec3 pos, float r, ML::Vec3 speed)
	{
		//接触三角形を判定前にクリアする	
		this->col_Poligons.clear();

		//判定関数
		After_Collision poligon0 = this->core.Get_Collision_Poligon(pos, r, speed);


		//判定の結果を保存
		this->col_Poligons.push_back(poligon0);
		for (int b = 0; b < 6; b++)
		{
			this->col_Poligons.push_back(this->barrier[b].Get_Collision_Poligon(pos,r,speed));
		}
	}

	//-----------------------------------------------------------------------------------
	//ほかのプログラムにあたり判定が終わったポリゴンを渡す関数
	std::vector<After_Collision> Object::Get_Collision_Poligon()
	{
		return this->col_Poligons;
	}
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//イニシャライズに失敗したらKill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize()
	{
		return  this->Initialize();
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