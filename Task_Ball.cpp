//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Ball.h"
#include  "Task_MapSide.h"
#include  "Task_MapCore.h"

namespace  Ball
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->meshName = "Item";

		DG::Mesh_CreateFromSOBFile(this->meshName, "./data/mesh/Item.SOB");
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
	bool  Object::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化
		this->pos = ML::Vec3(1050, 500, 900);//仮の位置後で調整をかける(2018/04/20)
		this->speed = ML::Vec3(0, 0, 0);
		this->r = 20.0f;
		this->m = 10.0f;
		this->collision_Flag = false;

	
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
			
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto in1 = DI::GPad_GetState("P1");		

		//マップの情報を修得、今はタスク一個で持ってくるが
		//後にVectorで持ってくるよう調整する(2018/03/27)
		auto map = ge->GetTask_Group_GN<Map3d::Object>("マップ","Side");		
		auto core = ge->GetTask_One_G<Map_Core::Object>("マップ");
		//マップのあたり判定の結果を保存する
		std::vector<After_Collision> Result;
		Result = core->Get_Collision_Poligon();
		for (auto i = map->begin(); i != map->end(); i++)
		{
			for (auto it : (*i)->Get_Collision_Poligon())
			{
				Result.push_back(it);
			}			
		}

		//-------------------------------------
		//デバッグ用ポーズ
		if (in1.B2.down)
		{
			system("pause");
		}
		//-------------------------------------

		//重力加速
		this->speed += this->G.Accelerate(this->m);

		//もし,どこもあたり判定をせずに動いた場合
		//処理せずに次のフレームに移る
		if (Result.size() == 0)
		{	
			//移動(フレーム終了する直前に行う)
			this->pos += this->speed;
			return;
		}

		//前回フレームのあたり判定結果を確認
		if (this->Is_Collision())
		{
			//今回フレームで衝突があったことを確認するフラグ
			bool cnt = false;

			//結果の数分ループする
			for (auto p : Result)
			{				
				//前のフレームで衝突だったら、今回のフレームでの衝突判定でやること
				if (p.collision_Flag)
				{
					//今回のフレームに衝突だったら
					//斜め線加速をする
					this->speed = this->G.CollisionOver_Accelerate(this->speed, p.normal,this->m);
					//フラグを立てる
					cnt = true;
					this->collision_Flag = true;
					//どこかでくっついているときの回転反応
					if (in1.LStick.volume != 0)
					{
						//マップのフレーム回転量で回転させる
						ML::Mat4x4 matR;
						D3DXMatrixAffineTransformation(&matR, 1, &ge->Map_center, &core->Get_Frame_QT(), NULL);
						
						this->pos = matR.TransformCoord(this->pos);
					}

				}
				else if(cnt == false)
				{
					//今回のフレームに衝突しなかったら
					//衝突フラグを無効にする
					this->collision_Flag = false;
				}
			}
		}
		else
		{
			//結果の数分ループする
			for (auto p : Result)
			{
				//前のフレームで衝突ではなかったら、今回のフレームでの衝突判定でやること			
				if (p.collision_Flag)
				{
					//今回のフレームに衝突だったら
					//反射角で跳ね返す

					this->speed = this->G.Reflaction_Vector(this->speed, p.normal, this->m);

					//衝突支店の時の回転反応テスト
					if (in1.LStick.volume != 0)
					{
						//マップのフレーム回転量で回転させる
						ML::Mat4x4 matR;
						D3DXMatrixAffineTransformation(&matR, 1, &ge->Map_center, &core->Get_Frame_QT(), NULL);

						this->speed = matR.TransformNormal(this->speed);
					}

					//衝突フラグを有効にする
					this->collision_Flag = true;
				}
			}
		}		
		
		//移動(フレーム終了する直前に行う)
		this->pos += this->speed;
		
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{		
	}

	void  Object::Render3D_L0()
	{
		ML::Mat4x4 matT;
		ML::Mat4x4 matS;
		matT.Translation(this->pos);
		matS.Scaling(this->r);
		DG::EffectState().param.matWorld = matS * matT;
		DG::Mesh_Draw(this->res->meshName);
	}

	//--------------------------------------------------------------------
	//追加メソッド

	bool Object::Is_Collision()
	{
		return this->collision_Flag;
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