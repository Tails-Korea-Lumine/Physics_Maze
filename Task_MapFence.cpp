//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include "Task_MapFence.h"
#include  "Task_Ball.h"
#include "Wall.h"
#include "Unstable_Wall.h"

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
		this->size = 0;
		this->chipSize = 100.0f;		
		for (size_t i = 0; i < 10; i++)
		{
			this->arr[i] = nullptr;
		}
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
		//全体初期値の配列
		ML::QT initial_Value_Qt[]=
		{
			ML::QT(0.0f) ,
			ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(90)),
			ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(180)),
			ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(-90)),
			ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(90)),
			ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(90)) * ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(90)),
			ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(90)) * ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(180)),
			ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(90)) * ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(-90)),
			ML::QT(0.0f),
			ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(90)),
			ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(180)),
			ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(-90))
		};

		//面ごとに別の初期値を与える	
		this->Map_Rotate(initial_Value_Qt[this->fenceNumber]);
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
		for (size_t i = 0; i < this->size; i++)
		{
			if (this->arr[i] != nullptr)
			{
				delete arr[i];
			}
		}
		this->size = 0;

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
		for (size_t i = 0; i < this->size; i++)
		{
			//登録してないもの、距離がどう過ぎるのは無視する
			if (this->arr[i] == nullptr || this->Is_Need_Render(i) == false)
			{
				continue;
			}
			
			//色変更
			DG::EffectState().param.objectColor = ML::Color(1, 0.28f, 0.28f, 0.28f);
			//描画
			this->arr[i]->Rendering();
			//色を元道理に
			DG::EffectState().param.objectColor = ML::Color(1, 1, 1, 1);
		}
	}

	//-----------------------------------------------------------------------------------
	//追加メソッド		
	//外部ファイルからのマップロード
	bool Object::Map_Load(string f_)
	{		
		//メッシュ名の保存場所
		std::vector<string> chipName;
		if (this->Mesh_Load(&chipName) == false)
		{
			//メッシュ読み込みが失敗したらマップロードも失敗したことにする
			return false;
		}
		//外部ファイルから読み込み
		ifstream fin(f_);
		if (!fin)
		{
			return false;
		}		
		
		//マップ配列サイズの読み込み
		fin >> this->size;
		//マップ配列データの読みこみ
		for (size_t i = 0; i < this->size; i++)
		{			
			//チップ番号(ボックスタイプ)読み込み
			int chip;
			fin >> chip;
			//マップ中央を基準にした初期位置算出
			ML::Vec3 pos;

			if (this->fenceNumber < 8)
			{
				pos = ML::Vec3(i * this->chipSize + this->chipSize / 2, this->chipSize + this->chipSize / 2, this->chipSize + this->chipSize / 2);
				pos += ge->Map_center - ML::Vec3((this->mapSize*this->chipSize / 2), -((this->mapSize-2)*this->chipSize / 2), -((this->mapSize - 2)*this->chipSize / 2));
			}
			else
			{
				pos = ML::Vec3(this->chipSize + this->chipSize / 2, i * this->chipSize + this->chipSize / 2, this->chipSize + this->chipSize / 2);
				pos += ge->Map_center - ML::Vec3(((this->mapSize + 4)*this->chipSize / 2), (this->mapSize*this->chipSize / 2), -((this->mapSize - 2)*this->chipSize / 2));
			}
			//あたり判定用矩形
			ML::Vec3 base = ML::Vec3(this->chipSize / 2.0f, this->chipSize / 2.0f, this->chipSize / 2.0f);				

			//ボックスのID生成
			string id = to_string(this->fenceNumber) + to_string(i);			
			switch (Bbox::BoxType(chip))
			{
			case Bbox::BoxType::Wall:
				//配列に登録
				this->arr[i] = new Wall(pos, base, this->map_QT, id,chipName.at(chip));
				break;
			case Bbox::BoxType::Unstable_Wall:
				//配列に登録
				this->arr[i] = new Unstable_Wall(pos, base, this->map_QT, id, chipName.at(chip));
				break;
			default:
				continue;
				break;
			}
			
		}
		fin.close();
		return true;
	}
	//-----------------------------------------------------------
	bool Object::Mesh_Load(std::vector<string>* chips)
	{
		//0番はメッシュを使ってないので空白をpush_back
		chips->push_back("");
		//メッシュ名の読み込みおよび生成
		ifstream f("./data/mesh_Names.txt");
		if (f.is_open() == false)
		{
			//ファイルを読み込めなかった場合は生成失敗
			return false;
		}
		//メッシュの個数
		int chipnum;
		f >> chipnum;
		//読み込み
		for (int i = 0; i<chipnum; i++)
		{
			string chip_Name, chip_File_Path;
			f >> chip_Name;
			chip_File_Path = "./data/mesh/" + chip_Name;
			chips->push_back(chip_Name);
			//メッシュ生成
			DG::Mesh_CreateFromSOBFile(chip_Name, chip_File_Path);
		}
		f.close();
		return true;
	}
	//-----------------------------------------------------------------------
	void Object::Map_Check_Hit(Shape3D* ball)
	{
		std::vector<Collision_Data> col_Poligons;
		//接触三角形を判定前にクリアする
		col_Poligons.clear();

		//判定スタート
		for (size_t i = 0; i < this->size; i++)
		{
			if (this->arr[i] == nullptr)
			{
				continue;
			}
			//一定距離以内のものだけ判定をする
			ML::Vec3 d = this->arr[i]->Get_Pos() - ball->Get_Center();
			//dは絶対値の距離					
			//一定距離以上だったら判定せず次に項目に
			if (d.Length() > this->chipSize)
			{
				continue;
			}
													
			//あたり判定
			if (!this->arr[i]->Collision_Action(&col_Poligons, ball))
			{
				continue;
			}					

			//全体衝突結果に保存する
			for (auto& c : col_Poligons)
			{
				ge->collision_Result.push_back(c);
			}
			
		}
	}

	//------------------------------------------------------------------------
	//回転させる
	void Object::Map_Rotate(const ML::QT& qt)
	{
		//全体の回転値更新
		this->UpDate_Quartanion(qt);

		for (size_t i = 0; i < this->size; i++)
		{
			if (this->arr[i] == nullptr)
			{
				continue;
			}
			//回転行列生成
			ML::Mat4x4 matR;
			D3DXMatrixAffineTransformation(&matR, 1.0f, &ge->Map_center, &qt, NULL);
			//ボックスに個別で渡す
			this->arr[i]->Rotate_Box(&matR, qt);			
		}
	}

	//-------------------------------------------------------------------------------------------
	//クォータニオンを更新する関数
	void Object::UpDate_Quartanion(const ML::QT& qt)
	{
		this->map_QT *= qt;
	}
	//------------------------------------------------------------------------------------------------------------
	//連続していて使えない面を探す
	void Object::Check_Unusable_Side()
	{
		//最小値、最大値を超えないようにする
		auto Inside_Range = [this](const size_t& ci)
		{return ci >= 0 && ci < this->size; };

		//引数たちが両方壁なのか判断する
		auto Judge = [](const Bbox* b1, const Bbox* b2)
		{
			if (b1 != nullptr && b2 != nullptr)
			{
				return b1->What_Type_Is_this_Box() == Bbox::BoxType::Wall && b2->What_Type_Is_this_Box() == Bbox::BoxType::Wall;
			}
			else
			{
				return false;
			}
		};
		//各ボックスに連続するボックスがあるかを確認
		for (size_t i = 0; i < this->size; i++)
		{			
			//判別開始
			//左
			if (this->fenceNumber < 8)
			{
				if (Inside_Range(i - 1) && Judge(this->arr[i - 1], this->arr[i]))
				{
					//無効ポリゴンを表示しておく
					this->arr[i]->Marking_On_Unusable_Side(Box_Side::Xminus);
				}
				//右
				if (Inside_Range(i + 1) && Judge(this->arr[i + 1], this->arr[i]))
				{
					//無効ポリゴンを表示しておく
					this->arr[i]->Marking_On_Unusable_Side(Box_Side::Xplus);
				}
			}
			else
			{
				//下
				if (Inside_Range(i - 1) && Judge(this->arr[i - 1], this->arr[i]))
				{
					//無効ポリゴンを表示しておく
					this->arr[i]->Marking_On_Unusable_Side(Box_Side::Yminus);
				}
				//上
				if (Inside_Range(i + 1) && Judge(this->arr[i + 1], this->arr[i]))
				{
					//無効ポリゴンを表示しておく
					this->arr[i]->Marking_On_Unusable_Side(Box_Side::Yplus);
				}
			}
		}
	}
	//-------------------------------------------------------------------------------------
	//配列ソート及びボールをスタート位置に置く
	//void Object::Array_Sorting()
	//{
	//	//stl remove_ifで削除(無効データに上書きする)ところをもらいながら
	//	//データを前に積める
	//	auto remove_Point = remove_if(&this->arr[0], &this->arr[this->size], [](const Bbox& b) {return b.What_Type_Is_this_Box() == BoxType::Road; });

	//	//無効データに上書きする
	//	for (; remove_Point != &this->arr[this->size]; remove_Point++)
	//	{
	//		*remove_Point = Bbox();
	//	}
	//}
	//ボールタスクのフラグにIDを組み込める
	//void Object::Insert_Id_To_Ball()
	//{
	//	//ID登録のためにボールタスクをもらっておく
	//	auto ball = ge->GetTask_One_G<Ball::Object>("ボール");

	//	for (size_t i = 0; i < this->size; i++)
	//	{			
	//		auto now_Type = this->arr[i].What_Type_Is_this_Box();
	//		//道は配列の後ろに積めておいたので発見したらその後は処理せずにbreak
	//		if (now_Type == BoxType::Road || now_Type == BoxType::Clear)
	//		{
	//			break;
	//		}
	//		//それ以外はフラグ登録
	//		ball->Set_Id_And_Flag(this->arr[i].Get_Id());
	//		
	//	}
	//}

	//---------------------------------------------------------------------------------------------
	//レンダリングするかしないかを確認するためのメソッド
	bool Object::Is_Need_Render(const unsigned int& index)
	{
		ML::Vec3 d_Cmc = (ge->Map_center - ge->camera[0]->pos);
		const float judge = d_Cmc.Length() + (this->chipSize * this->mapSize / 3.0f);

		//判定する距離よりマップの0番とカメラとの距離が遠いならレンダリングしない
		ML::Vec3 d_Cf0 = this->arr[index]->Get_Pos() - ge->camera[0]->pos;

		//図った距離で返す
		return d_Cf0.Length() < judge;

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