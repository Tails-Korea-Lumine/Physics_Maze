//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Ball.h"
#include  "Task_MapSide.h"
#include  "Task_MapCore.h"
#include  "Task_MapFence.h"

#define TERMINATION_SPEED 6

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
		this->m = 40.0f;
		this->rot = 0.0f;
		this->collision_Flag = false;
		this->teleportation_Flag = false;

	
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->eff_Manager.lock()->Add_Effect(this->Get_Pos(), ML::Vec3(0, 0, 0), BEffect::effType::Game_Clear);

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
		//重力加速
		this->G.Accelerate(&this->speed,this->m);				
			
		//回転量上昇
		this->rot += this->speed.Length();

		//テレポートフラグを無効に
		this->teleportation_Flag = false;
	}
	//-------------------------------------------------------------------
	//あたり判定による方向転換及び移動
	void Object::Move_Ball()
	{
		//もし,どこもあたり判定をせずに動いた場合
		//処理せずに次のフレームに移る
		if (ge->collision_Result.size() == 0)
		{
			//終端速度を指定		
			if (this->speed.Length() > TERMINATION_SPEED)
			{
				this->speed = this->speed.Normalize();
				this->speed *= TERMINATION_SPEED;
			}
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
					this->G.CollisionOver_Accelerate(&this->speed, p.normal);
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

					this->G.Reflaction_Vector(&this->speed, p.normal, this->m);

					//衝突フラグを有効にする
					this->collision_Flag = true;
				}
			}

		}

		//終端速度を指定		
		if (this->speed.Length() > TERMINATION_SPEED)
		{
			this->speed = this->speed.Normalize();
			this->speed *= TERMINATION_SPEED;
		}

		//移動(フレーム終了する直前に行う)
		this->pos += this->speed;

	}
	
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{		
	}

	void  Object::Render3D_L0()
	{
		ML::Mat4x4 matT;
		ML::Mat4x4 matS;
		ML::Mat4x4 matR;
		//移動行列
		matT.Translation(this->pos);
		//拡縮行列
		matS.Scaling(this->r);
		//回転行列
		matR.RotationAxis((ge->Map_center - this->pos), ML::ToRadian(this->rot));
		DG::EffectState().param.matWorld = matS * matR * matT;
		DG::Mesh_Draw(this->res->meshName);
	}

	//--------------------------------------------------------------------
	//追加メソッド
	//外角の点を全部取り出す
	void Object::Get_Poionts_to_Sphere(std::vector<ML::Vec3>* result) const
	{
		const int increasing_Dgree = 15;
		//球の上にある点を全部取り出す処理

		//最初に回転させる点を計算
		std::vector<ML::Vec3> points;
		ML::Vec3 y = this->pos + ML::Vec3(0, this->r, 0);

		//縦方向に切った断面の片方を取る処理
		for (int i = 0; i < 180; i += increasing_Dgree)
		{
			//回転行列生成
			ML::Mat4x4 matRx;
			ML::QT qtX = ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian((float)i));

			D3DXMatrixAffineTransformation(&matRx, 1.0f, &this->pos, &qtX, NULL);

			points.push_back(matRx.TransformCoord(y));
		}

		//取り出した点を回転しながら結果保存用ヴェクターにプッシュバック
		for (int d = 0; d < 360; d += increasing_Dgree)
		{
			//回転行列生成
			ML::Mat4x4 matRy;
			ML::QT qtY = ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian((float)d));

			D3DXMatrixAffineTransformation(&matRy, 1.0f, &this->pos, &qtY, NULL);

			for (auto& p : points)
			{
				result->push_back(matRy.TransformCoord(p));
			}
		}
	}
	//-------------------------------------------------------------------------------------
	
	//あたり判定フラグを確認
	bool Object::Is_Collision() const
	{
		return this->collision_Flag;
	}

	//---------------------------------------------------------------------
	//その時のボールの情報を返す関数
	//位置
	ML::Vec3 Object::Get_Pos() const
	{
		return this->pos;
	}
	//半直径
	float Object::Get_Radious() const
	{
		return this->r;
	}
	//速度
	ML::Vec3 Object::Get_Speed() const
	{
		return this->speed;
	}

	//--------------------------------------------------------------------
	//位置補正用回転関数
	void Object::Fix_Position_for_Rotate(const ML::QT& qt)
	{
		//マップのフレーム回転量で回転させる
		ML::Mat4x4 matR;
		D3DXMatrixAffineTransformation(&matR, 1, &ge->Map_center, &qt, NULL);

		this->pos = matR.TransformCoord(this->pos);
	}

	//----------------------------------------------------------------------------
	//テレポート(指定した場所に即移動する)
	void Object::Teleportation(const ML::Vec3& objectPos)
	{
		this->teleportation_Flag = true;
		this->pos = objectPos;
	}

	//------------------------------------------------------------------------------
	//コアとあたってない時コアの方に引っ張られる
	/*void Object::Graviation_Pull(const ML::Vec3& force)
	{
		this->speed += force;
	}*/
	//-----------------------------------------------------------------------------
	//テレポートしたかを確認
	bool Object::Is_Teleport_Now() const
	{
		return this->teleportation_Flag;
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