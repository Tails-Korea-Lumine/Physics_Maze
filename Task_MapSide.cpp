//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_MapSide.h"
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
	bool  Object::Initialize(int sideNum)
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();


		//★データ初期化
		this->sideNumber = sideNum;
		this->mapSize = 8;//基本は8X8難易度によって増加される(2018/04/21)
		this->render3D_Priority[0] = 1.0f;
		//面ごとに別の初期値を与える
		switch(sideNumber)
		{
		case 0:
			this->map_QT = ML::QT(0.0f);
			break;
		case 1:
			this->map_QT = ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(90));
			break;
		case 2:
			this->map_QT = ML::QT(ML::Vec3(0, 0, 1), ML::ToRadian(90));
			break;
		case 3:
			this->map_QT = ML::QT(ML::Vec3(0, 0, 1), ML::ToRadian(-90));
			break;
		case 4:
			this->map_QT = ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(-90));
			break;
		case 5:
			this->map_QT = ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(180));
			break;
		}
		//データのゼロクリア
		ZeroMemory(this->arr, sizeof(this->arr));
		this->sizeX = 0;
		this->sizeY = 0;
		this->sizeZ = 0;
		this->chipSize = 100.0f;
		for (int i = 0; i < _countof(this->chipName); i++)
		{
			this->chipName[i] = "";
		}
		this->col_Poligons.clear();
		this->Map_Load("./data/map/map1.txt");		

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
		this->frame_QT = ML::QT(0.0f);
		auto in1 = DI::GPad_GetState("P1");
		//スティックが倒された量を更新
		if (in1.B1.on)
		{
			//スティックで入力
			if (in1.LStick.axis.x != 0)
			{									
				this->frame_QT = ML::QT(ML::Vec3(0, 0, 1), ML::ToRadian(-in1.LStick.axis.x));
				this->map_QT *= this->frame_QT;
			}
		}
		//押されていない時はY軸回転とX軸回転
		else
		{		

			if (in1.LStick.axis.y != 0)
			{
				this->frame_QT = ML::QT(ML::Vec3(1,0,0),ML::ToRadian(-in1.LStick.axis.y));
				this->map_QT *= this->frame_QT;
			}
			
			if (in1.LStick.axis.x != 0)
			{				
				this->frame_QT = ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(-in1.LStick.axis.x));
				this->map_QT *= this->frame_QT;
			}			
		}

		//ワールド回転量に反映
		//ge->World_Rotation = this->map_QT;		
		
		
		//回転
		
		this->Map_Rotate();
		
		


		//あたり判定は毎回マップのほうで行う
		//ボールの情報を修得
		auto ball = ge->GetTask_One_G<Ball::Object>("ボール");
		
		//ボールとマップのあたり判定
		if (ball != nullptr)
		{
			this->Map_Check_Hit(ball->pos, ball->r, ball->speed);
		}
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
					//道はレンダリングしない
					if (this->arr[z][y][x].What_Type_Is_this_Box() == BoxType::Road)
					{
						continue;
					}
					 
					ML::Mat4x4 matT;
					matT.Translation(this->arr[z][y][x].Get_Pos());

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
					DG::Mesh_Draw(this->chipName[(int)this->arr[z][y][x].What_Type_Is_this_Box()]);
				}
			}
		}
	}

	//-----------------------------------------------------------------------------------
	//追加メソッド	
	//あたっているかを返す関数	

	std::vector<After_Collision> Object::Get_Collision_Poligon()
	{
		return this->col_Poligons;
	}
	//---------------------------------------------------------------------------------------
	//外部ファイルからのマップロード
	bool Object::Map_Load(string f_)
	{
		//外部ファイルから読み込み
		ifstream fin(f_);
		if (!fin)
		{
			return false;
		}
		int chipNum;
		fin >> chipNum;
		//チップファイル名読み込みと、メッシュのロード
		for (int c = 1; c <= chipNum; c++)
		{
			string chipFileName, chipFilePath;
			fin >> chipFileName;
			chipFilePath = "./data/mesh/" + chipFileName;
			this->chipName[c] = "MapChip" + std::to_string(c);
			DG::Mesh_CreateFromSOBFile(this->chipName[c], chipFilePath);
		}
		//マップ配列サイズの読み込み
		fin >> this->sizeX >> this->sizeY >> this->sizeZ;
		//マップ配列データの読みこみ
		for (int y = 0; y < this->sizeY; y++)
		{
			for (int z = this->sizeZ - 1; z >= 0; z--)
			{
				for (int x = 0; x < this->sizeX; x++)
				{
					int chip;
					fin >> chip;
					ML::Vec3 pos = ML::Vec3(x*this->chipSize + this->chipSize / 2, y*this->chipSize + this->chipSize / 2, z*this->chipSize + this->chipSize / 2);
					ML::Box3D base = ML::Box3D(-this->chipSize / 2, -this->chipSize / 2, -this->chipSize / 2, this->chipSize, this->chipSize, this->chipSize);				


					this->arr[z][y][x] = Bbox(BoxType(chip), pos, base, this->map_QT);
				}
			}
		}
		fin.close();
		return true;
	}
	//-----------------------------------------------------------------------
	void Object::Map_Check_Hit(ML::Vec3 pos, float r, ML::Vec3 speed)
	{
		//多重衝突まで適用したver0.3(2018/04/16)

		//接触三角形を判定前にクリアする
		this->col_Poligons.clear();

		//判定スタート
		for (int y = 0; y <= this->sizeY; y++)
		{
			for (int z = 0; z <= this->sizeZ; z++)
			{
				for (int x = 0; x <= this->sizeX; x++)
				{
					//道は判定しない
					if (this->arr[z][y][x].What_Type_Is_this_Box() == BoxType::Road)
					{
						continue;
					}
					//一定距離以内のものだけ判定をする
					ML::Vec3 d = this->arr[z][y][x].Get_Pos() - pos;
					if (d.Length() < 0)
					{
						d *= -1;
					}
					if (d.Length() > 200)
					{
						continue;
					}				
					//this->collision_Tri = this->col.Hit_Check(Mass, pos, r, this->map_QT); //(ver0.2で使った処理)
					After_Collision poligon = this->arr[z][y][x].Get_Collision_Poligon(pos, r, speed);

					this->col_Poligons.push_back(poligon);

					//ver0.2で使った処理
					//判定で当たったら処理を止める
					/*if (this->is_Collision().collision_Flag)
					{
					return;
					}*/
				}
			}
		}
	}

	//------------------------------------------------------------------------
	//回転させる
	void Object::Map_Rotate()
	{
			

		for (int y = 0; y < this->sizeY; y++)
		{
			for (int z = 0; z < this->sizeZ; z++)
			{
				for (int x = 0; x < this->sizeX; x++)
				{

					ML::Vec3 pos = ML::Vec3(x*this->chipSize + this->chipSize / 2, y*this->chipSize + this->chipSize / 2, z*this->chipSize + this->chipSize / 2);
					pos += ge->Map_center - ML::Vec3((this->mapSize*this->chipSize / 2), -400, (this->mapSize*this->chipSize / 2));
					//D3DXMatrixAffineTransformation(&matR, this->chipSize / 100.0f, &ML::Vec3(1050,50,1050), &qtW, NULL);
					//matR.Inverse();

					/*if (this->map_Rotation.Length() != 0)
					{
					ML::Mat4x4 matMove;
					D3DXMatrixAffineTransformation(&matMove, this->chipSize / 100.0f, &ge->Map_center, &qtM, NULL);
					matR *= matMove;
					}*/
					ML::Mat4x4 matR;
					D3DXMatrixAffineTransformation(&matR, this->chipSize / 100.0f, &ge->Map_center, &this->map_QT, NULL);
					//ML::Vec3 temp = matR.TransformCoord(this->arr[z][y][x].Get_Pos());
					pos = matR.TransformCoord(pos);

					//matR.Inverse();
					this->arr[z][y][x].Rotate_Box(pos, this->map_QT);
				}
			}
		}
	}

	
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_, int sideNum)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
			}
			if (!ob->B_Initialize(sideNum)) {
				ob->Kill();//イニシャライズに失敗したらKill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize(int sideNum)
	{
		return  this->Initialize(sideNum);
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