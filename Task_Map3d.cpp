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


		//データのゼロクリア
		ZeroMemory(this->arr, sizeof(this->arr));
		this->sizeX = 0;
		this->sizeY = 0;
		this->sizeZ = 0;
		this->hitBase = ML::Box3D(0, 0, 0, 0, 0, 0);
		this->chipSize = 100.0f;
		for (int i = 0; i < _countof(this->chipName); i++)
		{
			this->chipName[i] = "";
		}

		this->collision_Tri = { false,ML::Vec3(0,0,0) };
		//★データ初期化
		this->Map_Load("./data/StageData/Map00.txt");

		this->render3D_Priority[0] = 1.0f;
		
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
		//あたり判定は毎回マップのほうで行う
		//ボールの情報を修得
		auto ball = ge->GetTask_One_G<Ball::Object>("ボール");
		
		//ボールとマップのあたり判定
		this->Map_Check_Hit(ball->pos, ball->r);
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
					ML::Mat4x4 matW;
					matW = matS * matT;
					DG::EffectState().param.matWorld = matW;
					DG::Mesh_Draw(this->chipName[cn]);
				}
			}
		}
	}

	//-----------------------------------------------------------------------------------
	//追加メソッド
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
		this->hitBase = ML::Box3D(0, 0, 0,
			this->sizeX*(int)this->chipSize,
			this->sizeY*(int)this->chipSize,
			this->sizeZ*(int)this->chipSize);
		//マップ配列データの読みおこみ
		for (int y = 0; y < this->sizeY; y++)
		{
			for (int z = this->sizeZ - 1; z >= 0; z--)
			{
				for (int x = 0; x < this->sizeX; x++)
				{
					fin >> this->arr[z][y][x].chip;
					this->arr[z][y][x].pos = ML::Vec3(x*this->chipSize + this->chipSize / 2, y*this->chipSize + this->chipSize / 2, z*this->chipSize + this->chipSize / 2);
					this->arr[z][y][x].collision_Base = ML::Box3D(-50, -50, -50, 100, 100, 100);
				}
			}
		}
		fin.close();
		return true;
	}

	//あたり判定
	bool Object::Map_Check_Hit(const ML::Box3D& hit)
	{
		struct Box3D_2point
		{
			int fx, fy, fz;//値が小さい側の点
			int bx, by, bz;//値が大きい側の点
		};

		Box3D_2point r =
		{
			hit.x , hit.y , hit.z,
			hit.x + hit.w , hit.y + hit.h , hit.z + hit.d
		};

		Box3D_2point m =
		{
			this->hitBase.x , this->hitBase.y, this->hitBase.z,
			this->hitBase.x + this->hitBase.w , this->hitBase.y + this->hitBase.h, this->hitBase.z + this->hitBase.d
		};

		//キャラクタの矩形をマップの範囲内に丸め込む
		if (r.fx < m.fx) { r.fx = m.fx; }
		if (r.fy < m.fy) { r.fy = m.fy; }
		if (r.fz < m.fz) { r.fz = m.fz; }
		if (r.bx > m.bx) { r.bx = m.bx; }
		if (r.by > m.by) { r.by = m.by; }
		if (r.bz > m.bz) { r.bz = m.bz; }

		//キャラクタがマップの外に完全に出ていたら判定終了
		if (r.bx <= r.fx) { return false; }
		if (r.by <= r.fy) { return false; }
		if (r.bz <= r.fz) { return false; }

		//ループ範囲設定
		int sx, sy, sz, ex, ey, ez;
		sx = r.fx / (int)this->chipSize;
		sy = r.fy / (int)this->chipSize;
		sz = r.fz / (int)this->chipSize;
		ex = (r.bx - 1) / (int)this->chipSize;
		ey = (r.bx - 1) / (int)this->chipSize;
		ez = (r.bz - 1) / (int)this->chipSize;

		//接触判定
		for (int z = sz; z <= ez; z++)
		{
			for (int y = sy; y <= ey; y++)
			{
				for (int x = sx; x <= ex; x++)
				{
					if (this->arr[z][y][x].chip > 0)
					{
						return true;
					}
				}
			}
		}
		return false;//接触しているものがない
	}
	//球とマップのあたり判定
	void Object::Map_Check_Hit(ML::Vec3 pos, float r)
	{
		//仮の範囲
		//マップの読み込みが確立した後に再調整をかける(2018/03/28)
		int sx, sy, sz;
		int ex, ey, ez;

		sx = (pos.x / 100) -1;
		sy = (pos.y / 100) -1;
		sz = (pos.z / 100) -1;
		ex = sx + 2;
		ey = sy + 2;
		ez = sz + 2;

		if (sx < 0 || sy < 0 || sz < 0)
		{
			return;
		}
		if (ex > this->sizeX || ey > this->sizeY || ez > this->sizeZ)
		{
			return;
		}
		
		//判定スタート
		for (int y = sy; y <= ey; y++)
		{
			for (int z = sz; z <= ez; z++)
			{
				for (int x = sx; x <= ex; x++)
				{
					//道は判定しない
					if (this->arr[z][y][x].chip == 0)
					{
						continue;
					}
					//判定するマスを修得する
					ML::Box3D Mass = this->arr[z][y][x].collision_Base.OffsetCopy(this->arr[z][y][x].pos);
					this->collision_Tri = this->col.Hit_Check(Mass, pos, r);
					//判定で当たったら処理を止める
					if (this->is_Collision().collision_Flag)
					{
						return;
					}
				}
			}
		}
	}
	//---------------------------------------------------------------------------
	//法線ベクトルをマップ情報で修得する
	//void Object::Take_Normal_Vector(mapData& m)
	//{
	//	int i = 0;
	//	for (int z = 1; z < this->sizeZ; z++)
	//	{
	//		for (int y = 1; y < this->sizeY; y++)
	//		{
	//			for (int x = 1; x < this->sizeX; x++)
	//			{
	//				//検索しているところが道以外なら
	//				if (this->arr[z][y][x].chip >= 1)
	//				{
	//					//自分の上下左右を調べて壁だったら法線ベクトルを持つ
	//					if (this->arr[z-1][y][x].chip < 1)
	//					{
	//						this->arr[z - 1][y][x].normal[i] = ML::Vec3(0, 0, -1);
	//						i++;
	//					}
	//					if (this->arr[z + 1][y][x].chip < 1)
	//					{
	//						this->arr[z + 1][y][x].normal[i] = ML::Vec3(0, 0, +1);
	//						i++;
	//					}
	//					if (this->arr[z][y-1][x].chip < 1)
	//					{
	//						this->arr[z][y-1][x].normal[i] = ML::Vec3(0, -1, 0);
	//						i++;
	//					}
	//					if (this->arr[z][y + 1][x].chip < 1)
	//					{
	//						this->arr[z][y + 1][x].normal[i] = ML::Vec3(0, +1, 0);
	//						i++;
	//					}
	//					if (this->arr[z][y][x-1].chip < 1)
	//					{
	//						this->arr[z][y][x-1].normal[i] = ML::Vec3(-1, 0, 0);
	//						i++;
	//					}if (this->arr[z][y][x+1].chip < 1)
	//					{
	//						this->arr[z][y][x+1].normal[i] = ML::Vec3(+1, 0, 0);
	//						i++;
	//					}
	//				}
	//				//検索が終わったらiを０に戻す
	//				i = 0;
	//			}
	//		}
	//	}
	//}

	//-----------------------------------------------------------------------
	//あたっているかを返す関数
	After_Collision Object::is_Collision()
	{
		return this->collision_Tri;
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