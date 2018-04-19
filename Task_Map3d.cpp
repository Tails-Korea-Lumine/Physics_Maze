//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Map3d.h"
#include  "Task_Ball.h"

namespace  Map3d
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		
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
		this->Map_Load("./data/StageData/Map00.txt");

		this->render3D_Priority[0] = 1.0f;
		//面ごとに別の初期値を与える
		//this->map_QT = ML::QT(0.0f);
		
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		for (int i = 0; i < _countof(this->chipName); i++)
		{
			if (this->chipName[i] != "")
			{
				DG::Mesh_Erase(this->chipName[i]);
			}
		}
		this->sizeX = 0;
		this->sizeY = 0;
		this->sizeZ = 0;

		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			//★引き継ぎタスクの生成
			
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
				ML::QT temp_qt(ML::Vec3(1,0,0),ML::ToRadian(-in1.LStick.axis.y));
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
		this->Map_Rotate();


		//あたり判定は毎回マップのほうで行う
		//ボールの情報を修得
		auto ball = ge->GetTask_One_G<Ball::Object>("ボール");
		
		//ボールとマップのあたり判定
		this->Map_Check_Hit(ball->pos, ball->r,ball->speed);
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		
	}

	void  Object::Render3D_L0()
	{
		

		ML::Mat4x4 matS;
		matS.Scaling(this->chipSize / 100.0f);
		for (int y = 0; y < this->sizeY; y++)
		{
			for (int z = 0; z < this->sizeZ; z++)
			{
				for (int x = 0; x < this->sizeX; x++)
				{
					int cn = this->arr[z][y][x].chip;
					if (cn == 0)
					{
						continue;
					}
					 
					ML::Mat4x4 matT;
					matT.Translation(this->arr[z][y][x].pos);

					ML::Mat4x4 matR;
					matR.RotationQuaternion(this->map_QT);
					//D3DXMatrixAffineTransformation(&matR, this->chipSize / 100.0f, NULL, &qtW, NULL);
					////matR.Inverse();

					//if (this->map_Rotation.Length() != 0)
					//{
					//	ML::Mat4x4 matMove;
					//	D3DXMatrixAffineTransformation(&matMove, this->chipSize / 100.0f, NULL, &qtM, NULL);
					//	matR *= matMove;
					//}
					//matR.Inverse();

					DG::EffectState().param.matWorld =  matR * matT;					
					DG::Mesh_Draw(this->chipName[cn]);
				}
			}
		}
	}

	//-----------------------------------------------------------------------------------
	//追加メソッド	
	//あたっているかを返す関数
	After_Collision Object::is_Collision()
	{
		return this->collision_Tri;		
	}

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