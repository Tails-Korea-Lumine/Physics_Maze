//---------------------------------------------------------------------------------
//エフェクトクラス
//---------------------------------------------------------------------------------

#include "Effect.h"
#include "easing.h"
#include "GameEngine_Ver3_7.h"

void Effect::Load_Eff(const ML::Vec3& pos, const ML::Vec3& angle)
{
	this->Eff_Initialize(pos, angle);	
}

void Effect::Load_Eff(const ML::Vec3& pos, const ML::Vec3& target, const ML::Vec3& angle)
{
	this->Eff_Initialize(pos, target, angle);
}

void Effect::Playing_Effect()
{
	//引数に合う関数を呼ぶ

	switch (this->playing_EffectHandle)
	{
	case effType::CreateCharactor:
		this->Playing_EF0();
		break;
	case effType::TeleportOut:
		this->Playing_EF1();
		break;
	case effType::Teleportin:
		this->Playing_EF2();
		break;
	case effType::EnemyLanding:
		this->Playing_EF3();
		break;
	case effType::Hit_to_Enemy:
		this->Playing_EF4();
		break;
	case effType::Hit_to_Wall:
		this->Playing_EF5();
		break;
	case effType::CreateTail:
		this->Playing_EF6();
		break;
	case effType::DestroyTail:
		this->Playing_EF7();
		break;
	case effType::Hit_to_Tail:
		this->Playing_EF8();
		break;
	case effType::Game_Clear:
		this->Playing_EF9();
		break;

	default:
		return;
	}
}


void Effect::UpDate_Effect()
{
	
	//引数に合う関数を呼ぶ

	switch (this->playing_EffectHandle)
	{
	case effType::CreateCharactor:
		this->UpDate_EF0();
		break;
	case effType::TeleportOut:
		this->UpDate_EF1();
		break;
	case effType::Teleportin:
		this->UpDate_EF2();
		break;
	case effType::EnemyLanding:
		this->UpDate_EF3();
		break;
	case effType::Hit_to_Enemy:
		this->UpDate_EF4();
		break;
	case effType::Hit_to_Wall:
		this->UpDate_EF5();
		break;
	case effType::CreateTail:
		this->UpDate_EF6();
		break;
	case effType::DestroyTail:
		this->UpDate_EF7();
		break;
	case effType::Hit_to_Tail:
		this->UpDate_EF8();
		break;
	case effType::Game_Clear:
		this->UpDate_EF9();
		break;

	default:
		return;
	}
}

//------------------------------------------------------------------------------------------------------
//描画関数
//0番：キャラクタ生成
void Effect::Playing_EF0()
{
	//if (this->effect_Life < 0)
	//{
	//	return;
	//}
	//ML::Mat4x4 matS, matR, matT;
	//matS.Scaling(this->scale);
	//matR.RotationY(this->angle.y);
	//matT.Translation(this->pos);

	//
	////透明度設定(光なので加算合成)
	//auto bsBuf = DG::EffectState().BS_Get();
	//DG::EffectState().BS_Alpha();
	//DG::EffectState().param.objectColor = ML::Color(this->alpha, 1, 1, 1);
	//DG::EffectState().param.matWorld = matS * matR * matT;
	//DG::Mesh_Draw(this->meshName);

	////透明度を元に戻す	
	//DG::EffectState().BS_Set(bsBuf);
	//DG::EffectState().param.objectColor = ML::Color(1, 1, 1, 1);
}
//1番：テレポートアウト
void Effect::Playing_EF1()
{
	if (this->effect_Life <= 0)
	{
		return;
	}
	ML::Mat4x4 matS;	
	matS.Scaling(this->scale);
	ML::Mat4x4 matT;
	matT.Translation(this->pos);

	//透明度を設定
	auto bsBuf = DG::EffectState().BS_Get();
	DG::EffectState().BS_Alpha();
	//エフェクトが持つ透明度で描画
	DG::EffectState().param.objectColor = ML::Color(this->alpha, 1, 1, 1);
	DG::EffectState().param.matWorld = matS * matT;
	DG::Mesh_Draw(this->meshName);

	//透明度を元に戻す
	DG::EffectState().BS_Set(bsBuf);
	DG::EffectState().param.objectColor = ML::Color(1, 1, 1, 1);
}
//2番：テレポートイン
void Effect::Playing_EF2()
{
	if (this->effect_Life <= 0)
	{
		return;
	}
	ML::Mat4x4 matT,matS;
	matT.Translation(this->pos);
	matS.Scaling(this->scale);

	//透明度設定(光なので加算合成)
	auto bsBuf = DG::EffectState().BS_Get();
	DG::EffectState().BS_AlphaAdd();
	DG::EffectState().param.objectColor = ML::Color(this->alpha, 1, 1, 1);
	DG::EffectState().param.matWorld = matS * matT;
	DG::Mesh_Draw(this->meshName);

	//透明度を元に戻す
	DG::EffectState().BS_Set(bsBuf);
	DG::EffectState().param.objectColor = ML::Color(1, 1, 1, 1);
}
//3番：エネミー着地
void Effect::Playing_EF3()
{
	//if (this->effect_Life < 0)
	//{
	//	return;
	//}
	//
	//
	//ML::Mat4x4 matT;
	//matT.Translation(this->pos);
	//ML::Mat4x4 matS;
	//matS.Scaling(this->scale);
	//

	////透明度を設定
	//auto bsBuf = DG::EffectState().BS_Get();
	//DG::EffectState().BS_Alpha();
	////エフェクトが持つ透明度で描画
	//DG::EffectState().param.objectColor = ML::Color(this->alpha, 1, 1, 1);
	//DG::EffectState().param.matWorld = matS * matT;
	//DG::Mesh_Draw(this->meshName);

	////透明度を元に戻す
	//DG::EffectState().BS_Set(bsBuf);
	//DG::EffectState().param.objectColor = ML::Color(1, 1, 1, 1);
}
//4番：エネミーと衝突
void Effect::Playing_EF4()
{
	//if (this->effect_Life < 0)
	//{
	//	return;
	//}

	//ML::Mat4x4 matT;
	//matT.Translation(this->pos);
	//ML::Mat4x4 matR;
	//matR.RotationY(-this->angle.y);
	//ML::Mat4x4 matS;
	//matS.Scaling(this->scale);

	////ライティングを無効にする
	//DG::EffectState().param.lightsEnable = false;
	////透明度を設定
	//auto bsBuf = DG::EffectState().BS_Get();
	//DG::EffectState().BS_AlphaAdd();
	////エフェクトが持つ透明度で描画
	//DG::EffectState().param.objectColor = ML::Color(this->alpha, 1, 1, 1);
	//DG::EffectState().param.matWorld = matS * matR * matT;
	//DG::Mesh_Draw(this->meshName);

	////透明度,ライティングを元に戻す
	//DG::EffectState().param.lightsEnable = true;
	//DG::EffectState().BS_Set(bsBuf);
	//DG::EffectState().param.objectColor = ML::Color(1, 1, 1, 1);
}
//5番：壁と衝突
void Effect::Playing_EF5()
{
	////エネミーと衝突に大きさが変化する
	//if (this->effect_Life < 0)
	//{
	//	return;
	//}

	//ML::Mat4x4 matT;
	//matT.Translation(this->pos);
	//ML::Mat4x4 matR;
	//matR.RotationY(-this->angle.y);
	//ML::Mat4x4 matS;
	//matS.Scaling(this->scale);

	////ライティングを無効にする
	//DG::EffectState().param.lightsEnable = false;
	////透明度を設定
	//auto bsBuf = DG::EffectState().BS_Get();
	//DG::EffectState().BS_Alpha();
	////エフェクトが持つ透明度で描画
	//DG::EffectState().param.objectColor = ML::Color(this->alpha, 1, 1, 1);
	//DG::EffectState().param.matWorld = matS * matR * matT;
	//DG::Mesh_Draw(this->meshName);

	////透明度,ライティングを元に戻す
	//DG::EffectState().param.lightsEnable = true;
	//DG::EffectState().BS_Set(bsBuf);
	//DG::EffectState().param.objectColor = ML::Color(1, 1, 1, 1);
	//
}
//6番：しっぽ生成
void Effect::Playing_EF6()
{
	//if (this->effect_Life <= 0)
	//{
	//	return;
	//}
	//ML::Mat4x4 matS;	
	//matS.Scaling(this->scale);
	//ML::Mat4x4 matT;
	//matT.Translation(this->pos);

	////透明度設定
	//auto bsBuf = DG::EffectState().BS_Get();
	//DG::EffectState().BS_AlphaAdd();
	//DG::EffectState().param.objectColor = ML::Color(this->alpha, 1, 1, 1);
	//DG::EffectState().param.matWorld = matS*matT;
	//DG::Mesh_Draw(this->meshName);

	////透明度を元に戻す
	//DG::EffectState().BS_Set(bsBuf);
	//DG::EffectState().param.objectColor = ML::Color(1, 1, 1, 1);
}
//7番：しっぽ消滅
void Effect::Playing_EF7()
{
	//if (this->effect_Life < 0)
	//{
	//	return;
	//}

	//ML::Mat4x4 matS;
	//matS.Scaling(this->scale);
	//ML::Mat4x4 matT;
	//matT.Translation(this->pos);
	//ML::Mat4x4 matR;
	//matR.RotationY(this->angle.y);


	////透明度を設定
	//auto bsBuf = DG::EffectState().BS_Get();
	//DG::EffectState().BS_AlphaAdd();
	////エフェクトが持つ透明度で描画
	//DG::EffectState().param.objectColor = ML::Color(this->alpha, 1, 1, 1);
	//DG::EffectState().param.matWorld = matS * matR * matT;
	//DG::Mesh_Draw(this->meshName);

	////透明度を元に戻す
	//DG::EffectState().BS_Set(bsBuf);
	//DG::EffectState().param.objectColor = ML::Color(1, 1, 1, 1);
}
//8番：しっぽと衝突
void Effect::Playing_EF8()
{
	//4番：エネミーと衝突と全く同じ
	//this->Playing_EF4();
}
//9番：クリアエフェクト
void Effect::Playing_EF9()
{
	if (this->effect_Life < 0)
	{
		return;
	}
	ML::Mat4x4 matS,matR, matT;
	matS.Scaling(this->scale);
	matR.RotationY(this->angle.y);
	matT.Translation(this->pos);

	//ライティングを無効にする
	DG::EffectState().param.lightsEnable = false;
	//透明度設定
	auto bsBuf = DG::EffectState().BS_Get();
	DG::EffectState().BS_Alpha();
	DG::EffectState().param.objectColor = ML::Color(this->alpha, 1, 1, 1);
	DG::EffectState().param.matWorld = matS * matR * matT;
	DG::Mesh_Draw(this->meshName);

	//透明度,ライティングを元に戻す
	DG::EffectState().param.lightsEnable = true;
	DG::EffectState().BS_Set(bsBuf);
	DG::EffectState().param.objectColor = ML::Color(1, 1, 1, 1);	
}


//--------------------------------------------------------------------------------------------------
//アップデート関数
//0番：キャラクタ生成
void Effect::UpDate_EF0()
{
	////easing::UpDate();

	//if (this->effect_Life == 140)
	//{
	//	easing::Start("scale_CreateCharactor");
	//	easing::Start("alpha_CreateCharactor");
	//}
	//else if(this->effect_Life < 140 && this->effect_Life >30)
	//{
	//	//easingで、大きさと透明度を変化する
	//	this->scale.x = easing::GetPos("scale_CreateCharactor");
	//	this->scale.y = easing::GetPos("scale_CreateCharactor");
	//	this->scale.z = easing::GetPos("scale_CreateCharactor");
	//	this->alpha = easing::GetPos("alpha_CreateCharactor");
	//}
	////30フレーム未満が残ったら破裂するように動く
	//else if (this->effect_Life == 30)
	//{
	//	easing::Set("scale_CreateCharactor", easing::ELASTICOUT, 100, 300, 30);
	//	easing::Start("scale_CreateCharactor");
	//}
	//else if (this->effect_Life < 30)
	//{
	//	this->scale.x = easing::GetPos("scale_CreateCharactor");
	//	this->scale.y = easing::GetPos("scale_CreateCharactor");
	//	this->scale.z = easing::GetPos("scale_CreateCharactor");
	//	this->alpha -= 0.05f;
	//}

	////easingに関わらず少しずつ回転している
	//this->angle.y += ML::ToRadian(2);
}
//1番：テレポートアウト
void Effect::UpDate_EF1()
{	
	//フレーム毎薄く、大きくなる
	this->alpha -= 0.008f;

	this->scale.y += 100.0f;
	
	//透明度は0以下に落ちないようにする
	if (this->alpha < 0.0)
	{
		this->alpha = 0.0;
	}
}
//2番：テレポートイン
void Effect::UpDate_EF2()
{
	this->moveVec = this->target - this->pos;//進行方向をもらう
	//移動
	this->pos += (this->moveVec * 0.25f);
	
	this->alpha -= 0.01f;
	//透明度は0以下に落ちないようにする
	if (this->alpha < 0.0)
	{
		this->alpha = 0.0;
	}	
}
//3番：エネミー着地
void Effect::UpDate_EF3()
{
	////easing start
	//if (this->effect_Life == 30)
	//{
	//	easing::Start("alpha_EnemyLanding");
	//	easing::Start("scaleXZ_EnemyLanding");
	//}
	////easing::UpDate();

	////XZ平面に広がる
	//this->scale.x = easing::GetPos("scaleXZ_EnemyLanding");
	//this->scale.z = easing::GetPos("scaleXZ_EnemyLanding");

	//this->alpha = easing::GetPos("alpha_EnemyLanding");
	//
}
//4番：エネミーと衝突
void Effect::UpDate_EF4()
{
	////カメラに対して角度変更
	//this->angle = ge->camera[0]->pos - this->pos;

	//this->angle.y = atan2(this->angle.z, this->angle.x);

	////登場は早く、消滅までは遅く(easing functionに代わる可能性あり 2018/02/01)
	//if (this->effect_Life > 50)
	//{
	//	this->alpha += 0.1f;
	//}
	//else
	//{
	//	this->alpha -= 0.02f;
	//}

	////透明度は0~1の間にある
	//if (this->alpha > 1.0f)
	//{
	//	this->alpha = 1.0f;
	//}
	//else if (this->alpha < 0.0f)
	//{
	//	this->alpha = 0.0f;
	//}

}
//5番：壁と衝突
void Effect::UpDate_EF5()
{
	////有効時間以外はエネミーと衝突と同じ
	////カメラに対して角度変更
	//this->angle = ge->camera[0]->pos - this->pos;

	//this->angle.y = atan2(this->angle.z, this->angle.x);

	//this->scale += ML::Vec3(10.0f, 10.0f, 10.0f);

	////登場は早く、消滅までは遅く(easing functionに代わる可能性あり 2018/02/01)
	//if (this->effect_Life > 8)
	//{
	//	this->alpha += 0.42f;
	//}
	//else
	//{
	//	this->alpha -= 0.04f;
	//}

	////透明度は0~1の間にある
	//if (this->alpha > 1.0f)
	//{
	//	this->alpha = 1.0f;
	//}
	//else if (this->alpha < 0.0f)
	//{
	//	this->alpha = 0.0f;
	//}

}
//6番：しっぽ生成
void Effect::UpDate_EF6()
{
	//if (this->effect_Life > 60)
	//{
	//	return;
	//}
	////しっぽについていく
	//auto opm = ge->GetTask_One_G<OptionManager::Object>("オプション");
	//if (opm == nullptr)
	//{		
	//	return;
	//}

	//auto target = opm->tailList;
	//auto t = target.begin();

	//this->pos = (*t);

	////残ったライフが30以上なら大きく以下なら小さく
	//ML::Vec3 scailing(100.0f, 200.0f, 100.0f);
	//this->scale = scailing * sin(ML::ToRadian(float(60 - this->effect_Life) * 3));
	//
	////小さくなる瞬間から薄くなる
	//if (this->effect_Life <= 30)
	//{
	//	this->alpha -= 0.04f;
	//}

	////透明度は0以下に落ちないようにする
	//if (this->alpha < 0.0)
	//{
	//	this->alpha = 0.0;
	//}
}
//7番：しっぽ消滅
void Effect::UpDate_EF7()
{
	//if(this->effect_Life == 300)
	//{
	//	easing::Start("posY_DestroyTail");
	//	easing::Start("scaleY_DestroyTail");
	//}
	////カメラに対して角度変更
	//this->angle = ge->camera[0]->pos - this->pos;

	//this->angle.y = atan2(this->angle.z, this->angle.x);
	////easing function
	//
	////easing::UpDate();
	//this->pos.y = easing::GetPos("posY_DestroyTail");
	//this->scale.y = easing::GetPos("scaleY_DestroyTail");
}
//8番：しっぽと衝突
void Effect::UpDate_EF8()
{
	//4番：エネミーと衝突と全く同じ
	//this->UpDate_EF4();
}
//9番：クリアエフェクト
void Effect::UpDate_EF9()
{
	if (this->effect_Life > 200)
	{
		return;
	}
	else if (this->effect_Life > 10)
	{
		this->scale += ML::Vec3(8.0f, 8.0f, 8.0f);
		this->angle.y += ML::ToRadian(0.3f);
		this->alpha += 0.8f;		
	}
	else if (this->effect_Life == 10)
	{
		easing::Reset("scale_EF9");
		easing::Start("scale_EF9");
	}
	else if (this->effect_Life < 10)
	{
		//easing::UpDate();
		this->scale.x = easing::GetPos("scale_EF9");
		this->scale.y = easing::GetPos("scale_EF9");
		this->scale.z = easing::GetPos("scale_EF9");
	}
}



void Effect::Dec_Eff()
{
	this->effect_Life--;
}

bool Effect::Is_Alive() const
{
	return this->effect_Life > 0 ? true : false;
}

BEffect::effType Effect::Get_Type() const
{
	return this->playing_EffectHandle;
}

bool Effect::Eff_Judge() const
{
	return this->effect_Life == 0;
}