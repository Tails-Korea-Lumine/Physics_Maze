//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include "Task_MapFence.h"
#include  "Task_Ball.h"

namespace  MapFence
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
	bool  Object::Initialize(int sideNum, Difficult_Range di)
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();


		//★データ初期化		
		this->fenceNumber = sideNum;
		this->mapSize = 8;//基本は8X8難易度によって増加される(2018/04/21)
		this->render3D_Priority[0] = 1.0f;
		//データのゼロクリア
		//ZeroMemory(this->arr, sizeof(this->arr));
		//this->sizeX = 0;
		this->sizeY = 0;
		this->sizeX = 0;
		this->chipSize = 100.0f;
		for (int i = 0; i < _countof(this->chipName); i++)
		{
			this->chipName[i] = "";
		}
		this->col_Poligons.clear();
		//外部ファイルからの読み込み
		switch (di)
		{
		case Difficult_Range::Easy:
			this->Map_Load("./data/fence/Easy/line" + to_string(sideNum) + ".txt");
			break;
		case Difficult_Range::Normal:
			this->Map_Load("./data/fence/Normal/line" + to_string(sideNum) + ".txt");
			break;
		case Difficult_Range::Hard:
			this->Map_Load("./data/fence/Hard/line" + to_string(sideNum) + ".txt");
			break;
		}
		//面ごとに別の初期値を与える
		switch(fenceNumber)
		{
		case 0:
		case 8:
			this->Map_Rotate( ML::QT(0.0f));
			break;
		case 9:
		case 1:
			this->Map_Rotate(ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(90)));
			break;
		case 10:
		case 2:
			this->Map_Rotate(ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(180)));
			break;
		case 11:
		case 3:
			this->Map_Rotate(ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(-90)));
			break;
		case 4:
			this->Map_Rotate(ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(90)));
			break;
		case 5:
			this->Map_Rotate(ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(90)) * ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(90)));
			break;
		case 6:
			this->Map_Rotate(ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(90)) * ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(180)));
			break;
		case 7:
			this->Map_Rotate(ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(90)) * ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(-90)));
			break;
		}
		
		
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
		//this->sizeX = 0;
		this->sizeY = 0;
		this->sizeX = 0;
		this->col_Poligons.clear();

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
		
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		
	}

	void  Object::Render3D_L0()
	{
		
		ML::Mat4x4 matW;
		//matS.Scaling(this->chipSize);
		for (int y = 0; y < this->sizeY; y++)
		{
			for (int x = 0; x < this->sizeX; x++)
			{
				
				//道はレンダリングしない
				if (this->arr[y][x].What_Type_Is_this_Box() == BoxType::Road)
				{
					continue;
				}
					 
				//描画
				D3DXMatrixAffineTransformation(&matW, this->chipSize, NULL, &this->map_QT, &this->arr[y][x].Get_Pos());

				DG::EffectState().param.matWorld = matW;
				DG::Mesh_Draw(this->chipName[(int)this->arr[y][x].What_Type_Is_this_Box()]);
				
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
			this->chipName[c] = "Fence_Chip" + std::to_string(c);
			DG::Mesh_CreateFromSOBFile(this->chipName[c], chipFilePath);
		}
		//マップ配列サイズの読み込み
		fin >> this->sizeX >> this->sizeY;
		//マップ配列データの読みこみ
		for (int y = 0; y < this->sizeY; y++)
		{
			for (int x = 0; x < sizeX; x++)
			{
				//チップ番号(ボックスタイプ)読み込み
				int chip;
				fin >> chip;
				//マップ中央を基準にした初期位置算出
				ML::Vec3 pos = ML::Vec3(x * this->chipSize + this->chipSize / 2, y*this->chipSize + this->chipSize / 2, this->chipSize + this->chipSize / 2);

				if (this->fenceNumber < 8)
				{
					pos += ge->Map_center - ML::Vec3((this->mapSize*this->chipSize / 2), -(this->mapSize*this->chipSize / 2), -((this->mapSize - 2)*this->chipSize / 2));
				}
				else
				{
					pos += ge->Map_center - ML::Vec3(((this->mapSize + 2)*this->chipSize / 2), (this->mapSize*this->chipSize / 2), -((this->mapSize - 2)*this->chipSize / 2));
				}
				//あたり判定用矩形
				ML::Box3D base = ML::Box3D(-this->chipSize / 2, -this->chipSize / 2, -this->chipSize / 2, this->chipSize, this->chipSize, this->chipSize);				

				//配列に登録
				this->arr[y][x] = Bbox(BoxType(chip), pos, base, this->map_QT);
				
			}
		}
		fin.close();
		return true;
	}
	//-----------------------------------------------------------------------
	void Object::Map_Check_Hit(std::vector<ML::Vec3>& all_Points, const ML::Vec3& pos, const float& r, const ML::Vec3& speed)
	{
		//多重衝突まで適用したver0.3(2018/04/16)

		//接触三角形を判定前にクリアする
		this->col_Poligons.clear();

		//判定スタート
		for (int y = 0; y < this->sizeY; y++)
		{
			for (int x = 0; x < this->sizeX; x++)
			{
				//for (int x = 0; x < this->sizeX; x++)
				{
					//道は配列の後ろに積めておいたので発見したらその後は処理せずにbreak
					if (this->arr[y][x].What_Type_Is_this_Box() == BoxType::Road)
					{
						continue;
					}
					//一定距離以内のものだけ判定をする
					ML::Vec3 d = this->arr[y][x].Get_Pos() - pos;
					//dは絶対値の距離					
					//一定距離以上だったら判定せず次に項目に
					if (d.Length() > this->chipSize)
					{
						continue;
					}
													
					//this->collision_Tri = this->col.Hit_Check(Mass, pos, r, this->map_QT); //(ver0.2で使った処理)
					//std::vector<After_Collision> poligon 
					this->arr[y][x].Get_Collision_Poligon(&this->col_Poligons, all_Points, pos, r, speed);
					

					//全体衝突結果に保存する
					for (auto& c : this->col_Poligons)
					{
						ge->collision_Result.push_back(c);
					}
				}
			}
		}
	}

	//------------------------------------------------------------------------
	//回転させる
	void Object::Map_Rotate(const ML::QT& qt)
	{
		//全体の回転値更新
		this->UpDate_Quartanion(qt);

		for (int y = 0; y < this->sizeY; y++)
		{
			for (int x = 0; x < this->sizeX; x++)
			{
				//回転行列生成
				ML::Mat4x4 matR;
				D3DXMatrixAffineTransformation(&matR, this->chipSize / 100.0f, &ge->Map_center, &qt, NULL);
				//ボックスに個別で渡す
				this->arr[y][x].Rotate_Box(&matR, qt);
				
			}
		}
	}

	//-------------------------------------------------------------------------------------------
	//クォータニオンを更新する関数
	void Object::UpDate_Quartanion(const ML::QT& qt)
	{
		this->map_QT *= qt;
	}
	//-------------------------------------------------------------------------------------
	//配列ソート及びボールをスタート位置に置く
	void Object::Array_Sorting()
	{
		//一時的にコピーする場所
		Bbox temp;
		//配列のボックスタイプが道ならば後ろに置く
		for (int y = 0; y < this->sizeY; y++)
		{
			for (int x = 0; x < this->sizeX; x++)
			{
				//道のボックスは後ろに積める
				if (this->arr[y][x].What_Type_Is_this_Box() == BoxType::Road)
				{
					temp = this->arr[y][x];
					//1個先のものに上書きする
					for (int i = x; i < this->sizeX - 1; i++)
					{
						this->arr[y][i] = this->arr[y][i + 1];
					}
					this->arr[y][this->sizeX - 1] = temp;
				}
				else if (this->arr[y][x].What_Type_Is_this_Box() == BoxType::Start)
				{
					//ボールをその位置に置く
					ge->GetTask_One_G<Ball::Object>("ボール")->Teleportation(this->arr[y][x].Get_Pos());
				}
				//道を発見したのに1個先のものが道以外だったら
				if (this->arr[y][x].What_Type_Is_this_Box() == BoxType::Road && this->arr[y][x + 1].What_Type_Is_this_Box() != BoxType::Road)
				{
					//積める処理のやり直し
					y--;
				}
			}
		}
	}
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_, int sideNum, Difficult_Range di)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
			}
			if (!ob->B_Initialize(sideNum,di)) {
				ob->Kill();//イニシャライズに失敗したらKill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize(int sideNum, Difficult_Range di)
	{
		return  this->Initialize(sideNum,di);
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