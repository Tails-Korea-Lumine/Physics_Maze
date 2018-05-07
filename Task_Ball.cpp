//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Ball.h"
#include  "Task_MapSide.h"
#include  "Task_MapCore.h"
#include  "Task_MapFence.h"

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
		this->pos = ML::Vec3(1000, 500, 900);//仮の位置後で調整をかける(2018/04/20)
		this->speed = ML::Vec3(0, 0, 0);
		this->moveVec = ML::Vec3(0, 0, 0);
		this->r = 30.0f;
		this->m = 200.0f;
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
		//重力加速
		this->speed += this->G.Accelerate(this->m);
		
		//ver0.4の残骸ベクトル計算の精密調整が必要となればまた使うかもしれない(2018/05/03)
		//for (int i = 0; i < 15; i++)
		
			////判定スタート
			//core->Core_Check_Hit(this->pos, this->r, this->speed);
			//for (auto m = map->begin(); m != map->end(); m++)
			//{
			//	(*m)->Map_Check_Hit(this->pos, this->r, this->speed);
			//}
			
			

			//-------------------------------------
			//デバッグ用ポーズ
			if (in1.B2.down)
			{
				system("pause");
			}
			//-------------------------------------

			//もし,どこもあたり判定をせずに動いた場合
			//処理せずに次のフレームに移る
			if (ge->collision_Result.size() == 0)
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
				for (auto p : ge->collision_Result)
				{
					//前のフレームで衝突だったら、今回のフレームでの衝突判定でやること
					if (p.collision_Flag)
					{
						//今回のフレームに衝突だったら
						//斜め線加速をする
						this->speed = this->G.CollisionOver_Accelerate(this->speed, p.normal, this->m);
						//フラグを立てる
						cnt = true;
						this->collision_Flag = true;

					}
					else if (cnt == false)
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
				for (auto p : ge->collision_Result)
				{
					//前のフレームで衝突ではなかったら、今回のフレームでの衝突判定でやること			
					if (p.collision_Flag)
					{
						//今回のフレームに衝突だったら
						//反射角で跳ね返す

						this->speed = this->G.Reflaction_Vector(this->speed, p.normal, this->m);


						//衝突フラグを有効にする
						this->collision_Flag = true;
					}
				}				
				
			}			
			
			//終端速度を指定
			if (this->speed.Length() > 6.0f)
			{
				this->speed = this->speed.Normalize();
				this->speed *= 6.0f;
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

	//---------------------------------------------------------------------
	//その時のボールの情報を返す関数
	//位置
	ML::Vec3 Object::Get_Pos()
	{
		return this->pos;
	}
	//半直径
	float Object::Get_Radious()
	{
		return this->r;
	}
	//速度
	ML::Vec3 Object::Get_Speed()
	{
		return this->speed;
	}

	//--------------------------------------------------------------------
	//位置補正用回転関数
	void Object::Fix_Position_for_Rotate(ML::QT qt)
	{
		//マップのフレーム回転量で回転させる
		ML::Mat4x4 matR;
		D3DXMatrixAffineTransformation(&matR, 1, &ge->Map_center, &qt, NULL);

		this->pos = matR.TransformCoord(this->pos);
	}

	//----------------------------------------------------------------------------
	//テレポート(指定した場所に即移動する)
	void Object::Teleportation(ML::Vec3 objectPos)
	{
		this->pos = objectPos;
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