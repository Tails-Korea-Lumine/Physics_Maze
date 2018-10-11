//-------------------------------------------------------------------
//キューブ外壁のマップ
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_MapSide.h"
#include  "Task_Ball.h"
#include  "Task_Game.h"
#include  "Task_CameraMan.h"

#define JUDGE_DEGREE_EIGHTYFIVE 85

namespace  Map3d
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->seTeleportIn = "SETeleIn";
		DM::Sound_CreateSE(this->seTeleportIn, "./data/sound/TeleportIn.wav");
		this->seTeleportOut = "SETeleOut";
		DM::Sound_CreateSE(this->seTeleportOut, "./data/sound/TeleportOut.wav");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{		
		DM::Sound_Erase(this->seTeleportOut);
		DM::Sound_Erase(this->seTeleportIn);
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
		this->rendering_Judge = _CMATH_::cosf(ML::ToRadian(JUDGE_DEGREE_EIGHTYFIVE));
		//this->render3D_Priority[0] = 1.0f;
		//データのゼロクリア
		//ZeroMemory(this->arr, sizeof(this->arr));
		this->sizeX = 0;
		this->sizeZ = 0;
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
		switch(sideNumber)
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
		
		
		this->Array_Sorting();
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
		this->sizeZ = 0;
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
		//ボールの情報を修得
		//auto ball = ge->GetTask_One_G<Ball::Object>("ボール");

		//this->frame_QT = ML::QT(0.0f);
		//auto in1 = DI::GPad_GetState("P1");

		//for (int i = 0; i < 10; i++)
		//{
		//	//スティックが倒された量を更新
		//	if (in1.B1.on)
		//	{
		//		//スティックで入力
		//		if (in1.LStick.axis.x != 0)
		//		{
		//			this->frame_QT = ML::QT(ML::Vec3(0, 0, 1), ML::ToRadian(-in1.LStick.axis.x / 10.0f));
		//			this->map_QT *= this->frame_QT;
		//		}
		//	}
		//	//押されていない時はY軸回転とX軸回転
		//	else
		//	{

		//		if (in1.LStick.axis.y != 0)
		//		{
		//			this->frame_QT = ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(-in1.LStick.axis.y / 10.0f));
		//			this->map_QT *= this->frame_QT;
		//		}

		//		if (in1.LStick.axis.x != 0)
		//		{
		//			this->frame_QT = ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(-in1.LStick.axis.x / 10.0f));
		//			this->map_QT *= this->frame_QT;
		//		}
		//	}

		//	//ワールド回転量に反映
		//	//ge->World_Rotation = this->map_QT;		


		//	//回転

		//	this->Map_Rotate();



		//	//あたり判定は毎回マップのほうで行う	

		//	//ボールとマップのあたり判定
		//	if (ball != nullptr)
		//	{
		//		this->Map_Check_Hit(ball->Get_Pos(), ball->Get_Radious(), ball->Get_Speed());
		//	}

		//	//位置補正を仕掛ける
		//	for (auto p : this->col_Poligons)
		//	{
		//		if (p.collision_Flag)
		//		{
		//			ball->Fix_Position_for_Rotate(this->frame_QT);
		//			break;
		//		}
		//	}
		//}
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
				
		//行列生成
		ML::Mat4x4 matW;
		for (int z = 0; z < this->sizeZ; z++)
		{
			for (int x = 0; x < this->sizeX; x++)
			{
				//道は配列の後ろに積めておいたので発見したらその後は処理せずにbreak
				BoxType now_Type = this->arr[z][x].What_Type_Is_this_Box();
				if (now_Type == BoxType::Road)
				{
					break;
				}
				else if (now_Type == BoxType::Start)
				{
					continue;
				}
				
				//アフィン変換
				D3DXMatrixAffineTransformation(&matW, this->chipSize , NULL, &this->map_QT, &this->arr[z][x].Get_Pos());				
				//ワールド行列に上書き
				DG::EffectState().param.matWorld = matW;
				//レンダリング
				DG::Mesh_Draw(this->chipName[(int)now_Type]);
			}
		}
		
	}

	//-----------------------------------------------------------------------------------
	//追加メソッド	
	//あたっているかを返す関数	

	/*void Object::Get_Collision_Poligon(std::vector<After_Collision>* result) const
	{
		for (auto p : this->col_Poligons)
		{
			result->push_back(p);
		}
	}*/
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
			this->chipName[c] = "Map" + to_string(this->sideNumber) + "Chip" + std::to_string(c);
			DG::Mesh_CreateFromSOBFile(this->chipName[c], chipFilePath);
		}
		//マップ配列サイズの読み込み
		fin >> this->sizeX >> this->sizeZ;
		//マップ配列データの読みこみ
		
		for (int z = this->sizeZ - 1; z >= 0; z--)
		{
			for (int x = 0; x < this->sizeX; x++)
			{
				int chip;
				fin >> chip;
				ML::Vec3 pos = ML::Vec3(x*this->chipSize + this->chipSize / 2, this->chipSize + this->chipSize / 2, z*this->chipSize + this->chipSize / 2);
				pos += ge->Map_center - ML::Vec3((this->mapSize*this->chipSize / 2), -((this->mapSize - 2)*this->chipSize / 2), (this->mapSize*this->chipSize / 2));

				ML::Box3D base = ML::Box3D(-this->chipSize / 2, -this->chipSize / 2, -this->chipSize / 2, this->chipSize, this->chipSize, this->chipSize);				
				ML::Mat4x4 matR;
				
				//生成すること以外に何か処理を加える必要があるもの
				switch (BoxType(chip))
				{				
				//テレポート
				case BoxType::Teleportaion:		

					ge->TM.Update_Door_Position(this->sideNumber, pos);
					break;
				//スイッチはあたり判定範囲を小さく	
				case BoxType::LightSwitch:
					base = ML::Box3D(base.x / 10, base.y / 10, base.z / 10, base.w / 10, base.h / 10, base.d / 10);
					break;
				}

				
				this->arr[z][x] = Bbox(BoxType(chip), pos, base, this->map_QT);
			}
			
		}
		fin.close();
		return true;
	}
	//-----------------------------------------------------------------------
	bool Object::Map_Check_Hit(std::vector<ML::Vec3>& all_Points, const ML::Vec3& pos, const float& r, const ML::Vec3& speed)
	{
		//多重衝突まで適用したver0.3(2018/04/16)

		//接触三角形を判定前にクリアする
		this->col_Poligons.clear();
		//std::vector<After_Collision> poligon;
		bool ball_on_This_Side = false;
		//判定スタート
		
		for (int z = 0; z < this->sizeZ; z++)
		{
			for (int x = 0; x < this->sizeX; x++)
			{
				//道は配列の後ろに積めておいたので発見したらその後は処理せずにbreak
				BoxType now_Type = this->arr[z][x].What_Type_Is_this_Box();
				if (now_Type == BoxType::Road)
				{
					break;
				}
				//一定距離以内のものだけ判定をする
				ML::Vec3 d = this->arr[z][x].Get_Pos() - pos;
				//dは絶対値の距離(distance)				
				//一定距離以上だったら判定せず次に項目に
				if (d.Length() > this->chipSize)
				{
					continue;
				}
				ball_on_This_Side = true;
				//boxType別に処理を分ける
				switch (now_Type)
				{
				//スタート位置は判定なし
				case BoxType::Start:
					continue;
					break;
				//ゴール旗はクリア判定
				case BoxType::Goal:
					if (this->arr[z][x].Get_Collision_Bool(all_Points,pos, r, speed))
					{
						ML::Vec3 distance = this->arr[z][x].Get_Pos() - pos;
						auto ball = ge->GetTask_One_G<Ball::Object>("ボール");
						ball->Teleportation(pos + (distance*0.01f));
							
						ge->game.lock()->Game_Clear();							
					}
					break;
				//扉はテレポート
				case BoxType::Teleportaion:						
					if (this->arr[z][x].Get_Collision_Bool(all_Points, pos, r, speed))
					{						
						auto ball = ge->GetTask_One_G<Ball::Object>("ボール");
						ML::Vec3 exitpos;
						if (ge->TM.Find_Exit(this->sideNumber,&exitpos))
						{								
							ball->Teleportation(exitpos);
							auto eff = ge->eff_Manager.lock();
							//テレポートインのエフェクト
							eff->Add_Effect(pos, this->arr[z][x].Get_Pos(), ML::Vec3(0, 0, 0), BEffect::effType::Teleportin);
							//テレポートインの音を鳴らす
							DM::Sound_Play(this->res->seTeleportIn, false);
							//テレポートアウトのエフェクト
							eff->Add_Effect(exitpos, this->Normal_Side, BEffect::effType::TeleportOut);
							//テレポートアウトの音を鳴らす
							DM::Sound_Play(this->res->seTeleportOut, false);
						}
					}
					break;
				case BoxType::LightSwitch:
					if (this->arr[z][x].Get_Collision_Bool(all_Points, pos, r, speed))
					{
						//カメラマンにライトを3秒間オフする命令を送る
						ge->GetTask_One_G<CameraMan::Object>("カメラマン")->Turnoff_the_Light();
					}
					break;
				//壁はただのあたり判定
				case BoxType::Wall:
					this->arr[z][x].Get_Collision_Poligon(&this->col_Poligons, all_Points, pos, r, speed);
					break;
				}
				//this->collision_Tri = this->col.Hit_Check(Mass, pos, r, this->map_QT); //(ver0.2で使った処理)				

				//全体衝突結果に保存する
				for (auto& c : this->col_Poligons)
				{
					ge->collision_Result.push_back(c);
				}
			}
			
		}
		
		return ball_on_This_Side;
	}

	//------------------------------------------------------------------------
	//回転させる
	void Object::Map_Rotate(const ML::QT& qt)
	{
		//マップ全体の回転値更新
		this->UpDate_Quartanion(qt);

		for (int z = 0; z < this->sizeZ; z++)
		{
			for (int x = 0; x < this->sizeX; x++)
			{

				//ML::Vec3 pos = ML::Vec3(x*this->chipSize + this->chipSize / 2, this->chipSize + this->chipSize / 2, z*this->chipSize + this->chipSize / 2);
				//pos += ge->Map_center - ML::Vec3((this->mapSize*this->chipSize / 2), -((this->mapSize-2)*this->chipSize / 2), (this->mapSize*this->chipSize / 2));
				//D3DXMatrixAffineTransformation(&matR, this->chipSize / 100.0f, &ML::Vec3(1050,50,1050), &qtW, NULL);
				//matR.Inverse();

				/*if (this->map_Rotation.Length() != 0)
				{
				ML::Mat4x4 matMove;
				D3DXMatrixAffineTransformation(&matMove, this->chipSize / 100.0f, &ge->Map_center, &qtM, NULL);
				matR *= matMove;
				}*/
				ML::Mat4x4 matR;
				D3DXMatrixAffineTransformation(&matR, this->chipSize / 100.0f, &ge->Map_center, &qt, NULL);
				//ML::Vec3 temp = matR.TransformCoord(this->arr[z][y][x].Get_Pos());
				//pos = matR.TransformCoord(pos);

				//ボックスの回転
				this->arr[z][x].Rotate_Box(&matR, qt);
				//テレポート扉の位置更新
				if (this->arr[z][x].What_Type_Is_this_Box() == BoxType::Teleportaion)
				{
					ge->TM.Update_Door_Position(this->sideNumber, this->arr[z][x].Get_Pos());
				}
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
		return c > this->rendering_Judge? false : true;
	}
	//-----------------------------------------------------------------------------------------
	//マップの法線ベクトルを返すメソッド
	void Object::Get_Normal_Side(ML::Vec3* result)const
	{
		*result = this->Normal_Side;
	}

	//-------------------------------------------------------------------------------------
	//配列ソート及びボールをスタート位置に置く
	void Object::Array_Sorting()
	{
		//一時的にコピーする場所
		Bbox temp;
		//配列のボックスタイプが道ならば後ろに置く
		for (int z = 0; z < this->sizeZ; z++)
		{
			for (int x = 0; x < this->sizeX - 1; x++)
			{
				//道のボックスは後ろに積める
				if (this->arr[z][x].What_Type_Is_this_Box() == BoxType::Road)
				{					
					temp = this->arr[z][x];
					//1個先のものに上書きする
					for (int i = x; i < this->sizeX - 1; i++)
					{
						this->arr[z][i] = this->arr[z][i + 1];
					}
					this->arr[z][this->sizeX - 1] = temp;					
				}
				else if (this->arr[z][x].What_Type_Is_this_Box() == BoxType::Start)
				{
					//ボールをその位置に置く
					ge->GetTask_One_G<Ball::Object>("ボール")->Teleportation(this->arr[z][x].Get_Pos());
				}
			}
			//ちゃんと積んでいるかを確認
			for (int x = 0; x < this->sizeX-1; x++)
			{
				//道を発見したのに1個先のものが道以外だったら
				if (this->arr[z][x].What_Type_Is_this_Box() == BoxType::Road && this->arr[z][x+1].What_Type_Is_this_Box() != BoxType::Road)
				{
					//積める処理のやり直し
					z--;
					break;
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