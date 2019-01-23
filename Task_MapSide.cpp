//-------------------------------------------------------------------
//キューブ外壁のマップ
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_MapSide.h"
#include  "Task_Ball.h"
#include  "Task_Game.h"
#include  "Task_CameraMan.h"
#include "Teleportation.h"
#include "Goal.h"
#include "Wall.h"
#include "Light_Switch.h"
#include "Unstable_Wall.h"


namespace  Map_Side
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
		this->render3D_Priority[0] = 0.3f;
		this->gimicCnt = 0;		
		this->sideNumber = sideNum;
		this->mapSize = 8;//基本は8X8
		this->rendering_Judge = _CMATH_::cosf(ML::ToRadian(70));
		//this->render3D_Priority[0] = 1.0f;
		//データのゼロクリア
		//ZeroMemory(this->arr, sizeof(this->arr));
		this->sizeX = 0;
		this->sizeZ = 0;
		this->chipSize = 100.0f;

		for (size_t z = 0; z < 8; z++)
		{
			for (size_t x = 0; x < 8; x++)
			{
				this->arr[z][x] = nullptr;
			}
		}		
		//外部ファイルからの読み込み
		switch (di)
		{
		case Difficult_Range::Easy:
			this->Map_Load("./data/map/Easy/map" + to_string(sideNum) + ".txt");
			break;
		case Difficult_Range::Normal:
			this->Map_Load("./data/map/Normal/map" + to_string(sideNum) + ".txt");
			break;
		case Difficult_Range::Hard:
			this->Map_Load("./data/map/Hard/map" + to_string(sideNum) + ".txt");
			break;
		}
		//面ごとに別の初期値を与える
		switch (sideNumber)
		{
		case 0:
			this->Map_Rotate(ML::QT(0.0f));
			this->Normal_Side = ML::Vec3(0, 1, 0);
			break;
		case 1:
			this->Map_Rotate(ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(90)));
			this->Normal_Side = ML::Vec3(0, 0, 1);
			break;
		case 2:
			this->Map_Rotate(ML::QT(ML::Vec3(0, 0, 1), ML::ToRadian(90)));
			this->Normal_Side = ML::Vec3(-1, 0, 0);
			break;
		case 3:
			this->Map_Rotate(ML::QT(ML::Vec3(0, 0, 1), ML::ToRadian(-90)));
			this->Normal_Side = ML::Vec3(1, 0, 0);
			break;
		case 4:
			this->Map_Rotate(ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(-90)));
			this->Normal_Side = ML::Vec3(0, 0, -1);
			break;
		case 5:
			this->Map_Rotate(ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(180)));
			this->Normal_Side = ML::Vec3(0, -1, 0);
			break;
		}
		
		this->Check_Unusable_Side();
		//this->Array_Sorting();
		//this->Insert_Id_To_Ball();
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放		
		for (size_t z = 0; z < this->sizeZ; z++)
		{
			for (size_t x = 0; x < this->sizeX; x++)
			{
				if (this->arr[z][x] != nullptr)
				{
					delete this->arr[z][x];
				}
			}
		}
		this->sizeX = 0;
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
	
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		
	}

	void  Object::Render3D_L0()
	{
		//レンダリングするかを先に確認
		if (this->Is_Need_Render() == false)
		{
			//必要ないときはこのまま処理終了
			return;
		}			
		
		size_t z, x;
		for (z = 0; z < this->sizeZ; z++)
		{
			for (x = 0; x < this->sizeX; x++)
			{
				if (this->arr[z][x] == nullptr || this->Is_Need_Render(z, x) == false)
				{
					continue;
				}
				//各マスのレンダリング処理
				const float rb = 37.45f;
				const float gb = 52.98f;
				const float bb = 11.34;

				float r = abs( cosf(rb * (this->sideNumber + 1)));
				float g = abs( cosf(gb * (this->sideNumber + 1)));
				float b = abs( cosf(bb * (this->sideNumber + 1)));
				ML::Color color = ML::Color(1, r, g, b);
				if (this->arr[z][x]->What_Type_Is_this_Box() == Bbox::BoxType::Wall || this->arr[z][x]->What_Type_Is_this_Box() == Bbox::BoxType::Unstable_Wall)
				{
					DG::EffectState().param.objectColor = color;
				}
				this->arr[z][x]->Rendering();
				DG::EffectState().param.objectColor = ML::Color(1, 1, 1, 1);
			}
		}
		
	}

	//-----------------------------------------------------------------------------------
	//追加メソッド		
	//外部ファイルからのマップロード
	bool Object::Map_Load(string f_)
	{
		//生成したメッシュ名を保存しておく場所
		string chipName[10];
		ML::Mat4x4 matR;
		//外部ファイルから読み込み
		ifstream fin(f_);
		if (!fin)
		{
			return false;
		}
		//レンダリングするメッシュの個数
		int chipNum;
		fin >> chipNum;
		//チップファイル名読み込みと、メッシュのロード
		for (int c = 1; c <= chipNum; c++)
		{
			string chipFileName, chipFilePath;
			fin >> chipFileName;
			chipFilePath = "./data/mesh/" + chipFileName;
			chipName[c] = "Map" + to_string(this->sideNumber) + "Chip" + std::to_string(c);
			DG::Mesh_CreateFromSOBFile(chipName[c], chipFilePath);
		}
		//マップ配列サイズの読み込み
		fin >> this->sizeX >> this->sizeZ;
		//マップ配列データの読みこみ
		
		for (size_t z = 0; z < this->sizeZ; z++)
		{
			for (size_t x = 0; x < this->sizeX; x++)
			{
				//チップ番号(ボックスタイプ)読み込み
				int chip;
				fin >> chip;
				//マップ中心点を基準にした初期位置算出
				ML::Vec3 pos = ML::Vec3(x*this->chipSize + this->chipSize / 2, this->chipSize + this->chipSize / 2, (this->sizeZ - 1 - z)*this->chipSize + this->chipSize / 2);
				pos += ge->Map_center - ML::Vec3((this->mapSize*this->chipSize / 2), -((this->mapSize - 2)*this->chipSize / 2), (this->mapSize*this->chipSize / 2));
				//あたり判定用矩形
				ML::Vec3 base = ML::Vec3(this->chipSize/2.0f, this->chipSize/2.0f, this->chipSize/2.0f);
				//ボックスのID生成
				string id = to_string(this->sideNumber) + to_string(z) + to_string(x);
				
				//生成すること以外に何か処理を加える必要があるもの
				switch (Bbox::BoxType(chip))
				{
				//テレポート
				case Bbox::BoxType::Teleportation:
					//配列に登録
					this->arr[z][x] = new Teleportation(pos, base, this->map_QT, id, chipName[chip], this->sideNumber);
					break;
				//スイッチはあたり判定範囲を小さく	
				case Bbox::BoxType::LightSwitch:
					base /= 10.0f;
					//配列に登録
					this->arr[z][x] = new Light_Switch(pos, base, this->map_QT, id, chipName[chip]);
					break;
				//ボールをスタート位置に置く
				case Bbox::BoxType::Start:					
					D3DXMatrixAffineTransformation(&matR, 1.0f, &ge->Map_center, &ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(-90)), NULL);
					pos = matR.TransformCoord(pos);
					ge->GetTask_One_G<Ball::Object>("ボール")->Teleportation(pos);
					//配列登録はしない
					break;
				//基本の壁障害物
				case Bbox::BoxType::Wall:
					//配列に登録
					this->arr[z][x] = new Wall(pos, base, this->map_QT, id, "MapCore");
					break;
				//ゴール位置
				case Bbox::BoxType::Goal:
					base /= 10.0f;					
					//配列に登録
					this->arr[z][x] = new Goal(pos, base, this->map_QT, id, chipName[chip]);
					break;
				//壊れる壁
				case Bbox::BoxType::Unstable_Wall:
					this->arr[z][x] = new Unstable_Wall(pos, base, this->map_QT, id, chipName[chip]);
					break;
				default:
					continue;
					break;
				}
								
				
			}
			
		}
		fin.close();
		return true;
	}
	//-----------------------------------------------------------------------
	bool Object::Map_Check_Hit(Shape3D* ball)
	{		
		//ボールがこの面にあるかを確認するためのフラグ	
		bool ball_on_This_Side = false;
		std::vector<Collision_Data> col_Poligons;

		//判定スタート		
		for (size_t z = 0; z < this->sizeZ; z++)
		{
			for (size_t x = 0; x < this->sizeX; x++)
			{
				if (this->arr[z][x] == nullptr)
				{
					continue;
				}
				//接触三角形を判定前にクリアする
				col_Poligons.clear();				
				//一定距離以内のものだけ判定をする
				ML::Vec3 d = this->arr[z][x]->Get_Pos() - ball->Get_Center();
				//dは絶対値の距離(distance)				
				//一定距離以上だったら判定せず次に項目に
				if (d.Length() > this->chipSize)
				{
					continue;
				}
				ball_on_This_Side = true;

				//判定開始及びギミック処理
				if (!this->arr[z][x]->Collision_Action(&col_Poligons, ball))
				{
					//あたってないマスなら次のマスに移行
					continue;
				}
				//判定の結果は保存する
				for (auto& c : col_Poligons)
				{
					ge->collision_Result.push_back(c);
				}
			}
			
		}
		//戻り値はボールがこの面にあるかを確認したフラグ
		return ball_on_This_Side;
	}

	//------------------------------------------------------------------------
	//回転させる
	void Object::Map_Rotate(const ML::QT& qt)
	{
		//マップ全体の回転値更新
		this->UpDate_Quartanion(qt);

		for (size_t z = 0; z < this->sizeZ; z++)
		{
			for (size_t x = 0; x < this->sizeX; x++)
			{
				if (this->arr[z][x] == nullptr)
				{
					continue;
				}
				//回転行列生成
				ML::Mat4x4 matR;
				D3DXMatrixAffineTransformation(&matR, 1.0f, &ge->Map_center, &qt, NULL);				

				//ボックスに個別で渡す
				this->arr[z][x]->Rotate_Box(&matR, qt);
			}
		}
		
	}

	//-------------------------------------------------------------------------------------------
	//クォータニオンを更新する関数
	void Object::UpDate_Quartanion(const ML::QT& qt)
	{
		this->map_QT *= qt;

		//法線ベクトルも回転量に応じて同じく回転を行う
		ML::Mat4x4 matR;
		D3DXMatrixAffineTransformation(&matR, 1.0f, NULL, &qt, NULL);

		this->Normal_Side = matR.TransformNormal(this->Normal_Side);
	}

	//------------------------------------------------------------------------------------------
	//レンダリングするかを確認する
	bool Object::Is_Need_Render()
	{
		//カメラとの内積値を保存する場所
		float c = 0.0f;
		//視線ベクトル
		ML::Vec3 sv = ge->camera[0]->target - ge->camera[0]->pos;
		MyMath::Vector_Dot(&c, sv, this->Normal_Side);

		//内積値が-90 < cos < 90の間はレンダリングをしない
		return c <= this->rendering_Judge;
	}
	//---------------------------------------------------------------------------------------------
	//レンダリングするかしないかを確認するためのメソッド(マス別)
	bool Object::Is_Need_Render(const unsigned int& z ,const unsigned int& x)
	{
		ML::Vec3 d_Cmc = (ge->Map_center - ge->camera[0]->pos);
		const float judge = d_Cmc.Length() + (this->chipSize * this->mapSize / 3.0f);

		//判定する距離よりマスとカメラとの距離が遠いならレンダリングしない
		ML::Vec3 d_Cf0 = this->arr[z][x]->Get_Pos() - ge->camera[0]->pos;

		//図った距離で返す
		return d_Cf0.Length() < judge;

	}
	//-----------------------------------------------------------------------------------------
	//マップの法線ベクトルを返すメソッド
	ML::Vec3 Object::Get_Normal_Side()const
	{
		return this->Normal_Side;
	}
	//連続していて使えない面を探す
	void Object::Check_Unusable_Side()
	{
		//最小値、最大値を超えないようにする
		auto Inside_Range = [this](const size_t& cx, const size_t& cz)
		{return cx >= 0 && cx < this->sizeX && cz >= 0 && cz < this->sizeZ; };

		//引数たちが両方壁なのか判断する
		auto Judge = [](const Bbox* b1, const Bbox* b2)
		{
			if (b1 != nullptr && b2 != nullptr)
			{
				auto b1_Type = b1->What_Type_Is_this_Box();
				auto b2_Type = b2->What_Type_Is_this_Box();
				return (b1_Type == Bbox::BoxType::Wall) && (b2_Type == Bbox::BoxType::Wall);
			}
			else
			{
				return false;
			}
		};
		//各ボックスに連続するボックスがあるかを確認
		for (size_t z = 0; z < this->sizeZ; z++)
		{
			for (size_t x = 0; x < this->sizeX; x++)
			{
				//判別開始
				//奥
				if (Inside_Range(x, z-1) && Judge(this->arr[z - 1][x], this->arr[z][x]))
				{
					//無効ポリゴンを表示しておく
					this->arr[z][x]->Marking_On_Unusable_Side(Box_Side::Zplus);
				}
				//手前
				if (Inside_Range(x, z + 1) && Judge(this->arr[z + 1][x], this->arr[z][x]))
				{
					//無効ポリゴンを表示しておく
					this->arr[z][x]->Marking_On_Unusable_Side(Box_Side::Zminus);
				}
				//左
				if (Inside_Range(x - 1, z) && Judge(this->arr[z][x - 1], this->arr[z][x]))
				{
					//無効ポリゴンを表示しておく
					this->arr[z][x]->Marking_On_Unusable_Side(Box_Side::Xminus);
				}
				//右
				if (Inside_Range(x + 1, z) && Judge(this->arr[z][x+1], this->arr[z][x]))
				{
					//無効ポリゴンを表示しておく
					this->arr[z][x]->Marking_On_Unusable_Side(Box_Side::Xplus);
				}
			}
		}
	}

	//-------------------------------------------------------------------------------------
	//配列ソート及びボールをスタート位置に置く
	//void Object::Array_Sorting()
	//{
	//	//2次元配列なので、sizeZ分ソート処理を回す
	//	for (size_t z = 0; z < this->sizeZ; z++)
	//	{
	//		//stl remove_ifで削除(無効データに上書きする)ところをもらいながら
	//		//データを前に積める
	//		auto remove_Point = remove_if(&this->arr[z][0], &this->arr[z][this->sizeX], [](const Bbox& b) {return b.What_Type_Is_this_Box() == BoxType::Road; });

	//		//無効データに上書きする
	//		for (; remove_Point != &this->arr[z][this->sizeX]; remove_Point++)
	//		{
	//			*remove_Point = Bbox();
	//		}
	//	}
	//}
	//ボールタスクのフラグにIDを組み込める
	//void Object::Insert_Id_To_Ball()
	//{
	//	//ID登録のためにボールタスクをもらっておく
	//	auto ball = ge->GetTask_One_G<Ball::Object>("ボール");

	//	for (size_t z = 0; z < this->sizeZ; z++)
	//	{
	//		for (size_t x = 0; x < this->sizeX; x++)
	//		{
	//			auto now_Type = this->arr[z][x].What_Type_Is_this_Box();
	//			//道は配列の後ろに積んでおいたので見つかったらbreak
	//			if (now_Type == BoxType::Road || now_Type == BoxType::Clear)
	//			{
	//				break;
	//			}
	//			//それ以外はフラグ登録
	//			ball->Set_Id_And_Flag(this->arr[z][x].Get_Id());
	//		}
	//	}
	//}
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