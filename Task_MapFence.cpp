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
		this->size = 0;
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
		this->Check_Unusable_Side();
		this->Array_Sorting();
		this->Insert_Id_To_Ball();
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
		this->size = 0;
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
		//先にレンダリングが必要なのかを確認する
		/*if (this->Is_Need_Render(0) == false)
		{
			return;
		}*/
		
		ML::Mat4x4 matW;
		//matS.Scaling(this->chipSize);
		for (int i = 0; i < this->size; i++)
		{
			
			BoxType now_Type = this->arr[i].What_Type_Is_this_Box();
			//道はレンダリングしない
			if (now_Type == BoxType::Road)
			{
				return;
			}
			//個別にもレンダリングが必要かを確認(フレームが上がるかのテスト)
			if (this->Is_Need_Render(i) == false)
			{
				continue;
			}

			//描画
			D3DXMatrixAffineTransformation(&matW, this->chipSize, NULL, &this->map_QT, &this->arr[i].Get_Pos());

			DG::EffectState().param.matWorld = matW;
			DG::Mesh_Draw(this->chipName[(int)now_Type]);
				
			
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
		fin >> this->size;
		//マップ配列データの読みこみ
		for (int i = 0; i < this->size; i++)
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
			ML::Box3D base = ML::Box3D(-this->chipSize / 2, -this->chipSize / 2, -this->chipSize / 2, this->chipSize, this->chipSize, this->chipSize);				

			//ボックスのID生成
			string id = to_string(this->fenceNumber) + to_string(i);			
			//配列に登録
			this->arr[i] = Bbox(BoxType(chip), pos, base, this->map_QT,id);
				
			
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
		for (int i = 0; i < this->size; i++)
		{
			//道は配列の後ろに積めておいたので発見したらその後は処理せずにbreak
			if (this->arr[i].What_Type_Is_this_Box() == BoxType::Road)
			{
				break;
			}
			//一定距離以内のものだけ判定をする
			ML::Vec3 d = this->arr[i].Get_Pos() - pos;
			//dは絶対値の距離					
			//一定距離以上だったら判定せず次に項目に
			if (d.Length() > this->chipSize)
			{
				continue;
			}
													
			//this->collision_Tri = this->col.Hit_Check(Mass, pos, r, this->map_QT); //(ver0.2で使った処理)
			//std::vector<After_Collision> poligon 
			this->arr[i].Get_Collision_Poligon(&this->col_Poligons, all_Points, pos, r, speed);
					

			//全体衝突結果に保存する
			for (auto& c : this->col_Poligons)
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

		for (int i = 0; i < this->size; i++)
		{
			
			//回転行列生成
			ML::Mat4x4 matR;
			D3DXMatrixAffineTransformation(&matR, this->chipSize / 100.0f, &ge->Map_center, &qt, NULL);
			//ボックスに個別で渡す
			this->arr[i].Rotate_Box(&matR, qt);				
			
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
		auto Judge = [](const Bbox& b1, const Bbox& b2)
		{return b1.What_Type_Is_this_Box() == BoxType::Wall && b2.What_Type_Is_this_Box() == BoxType::Wall; };
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
					this->arr[i].Check_Unusable_Poligon(2);
					this->arr[i].Check_Unusable_Poligon(3);
				}
				//右
				if (Inside_Range(i + 1) && Judge(this->arr[i + 1], this->arr[i]))
				{
					//無効ポリゴンを表示しておく
					this->arr[i].Check_Unusable_Poligon(6);
					this->arr[i].Check_Unusable_Poligon(7);
				}
			}
			else
			{
				//下
				if (Inside_Range(i - 1) && Judge(this->arr[i - 1], this->arr[i]))
				{
					//無効ポリゴンを表示しておく
					this->arr[i].Check_Unusable_Poligon(10);
					this->arr[i].Check_Unusable_Poligon(11);
				}
				//上
				if (Inside_Range(i + 1) && Judge(this->arr[i + 1], this->arr[i]))
				{
					//無効ポリゴンを表示しておく
					this->arr[i].Check_Unusable_Poligon(8);
					this->arr[i].Check_Unusable_Poligon(9);
				}
			}
		}
	}
	//-------------------------------------------------------------------------------------
	//配列ソート及びボールをスタート位置に置く
	void Object::Array_Sorting()
	{
		//一時的にコピーする場所
		Bbox temp;

		bool sort_Not_Over = true;
		while (sort_Not_Over)
		{
			int i = 0;
			//道を発見したら一番後ろに積む
			for (; i < this->size; i++)
			{
				if (this->arr[i].What_Type_Is_this_Box() == BoxType::Road)
				{
					//一時保存
					temp = this->arr[i];
					//1個ずつ前に引っ張る
					for (int j = i; j < this->size - 1; j++)
					{
						this->arr[j] = this->arr[j + 1];
					}
					//一番後ろと書き換え
					this->arr[this->size - 1] = temp;
				}
			}
			//ちゃんとソートできたかを確認
			for (i = 0; i < this->size - 1; i++)
			{
				if (this->arr[i].What_Type_Is_this_Box() == BoxType::Road && this->arr[i + 1].What_Type_Is_this_Box() != BoxType::Road)
				{
					break;
				}
			}
			//ちゃんと終わったのでソート終了
			if (i == this->size - 1)
			{
				sort_Not_Over = false;
			}
		}
	}
	//ボールタスクのフラグにIDを組み込める
	void Object::Insert_Id_To_Ball()
	{
		//ID登録のためにボールタスクをもらっておく
		auto ball = ge->GetTask_One_G<Ball::Object>("ボール");

		for (size_t i = 0; i < this->size; i++)
		{			
			//道は配列の後ろに積んでおいたので見つかったらbreak
			if (this->arr[i].What_Type_Is_this_Box() == BoxType::Road)
			{
				break;
			}
			//それ以外はフラグ登録
			ball->Set_Id_And_Flag(this->arr[i].Get_Id());
			
		}
	}

	//---------------------------------------------------------------------------------------------
	//レンダリングするかしないかを確認するためのメソッド
	bool Object::Is_Need_Render(const unsigned int& index)
	{
		ML::Vec3 d_Cmc = (ge->Map_center - ge->camera[0]->pos);
		const float judge = d_Cmc.Length() + (this->chipSize * this->mapSize / 3.0f);

		//判定する距離よりマップの0番とカメラとの距離が遠いならレンダリングしない
		ML::Vec3 d_Cf0 = this->arr[index].Get_Pos() - ge->camera[0]->pos;

		//図った距離で返す
		return d_Cf0.Length() < judge ? true : false;

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