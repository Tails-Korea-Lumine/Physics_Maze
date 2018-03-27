//-------------------------------------------------------------------------
//エフェクト基礎クラス
//-------------------------------------------------------------------------
#include "BEffect.h"
#include "easing.h"

void BEffect::LoadEffect(effType handle)
{	
	switch (handle)
	{
	case CreateCharactor://0
		this->meshName = "CreateCharactor";
		this->filePath = "./data/mesh/effect/CreateCharactor.SOB";
		this->alpha = 0.0f;
		this->effect_Life = 140;
		easing::Set("scale_CreateCharactor", easing::EXPOOUT, 0, 100, 110);
		easing::Set("alpha_CreateCharactor", easing::EXPOOUT, 0, 1, 110);


		break;
		
	case CreateItem://1
		this->meshName = "CreateItem";
		this->filePath = "./data/mesh/effect/CreateItem.SOB";
		this->alpha = 0.6f;
		this->effect_Life = 60;
		this->scale = ML::Vec3(100, 100, 100);
		easing::Set("scaleY_CreateItem", easing::QUINTOUT, 100, 1200, 60);

		break;
	case DestroyItem://2
		this->meshName = "DestroyItem";
		this->filePath = "./data/mesh/effect/DestroyItem.SOB";
		this->alpha = 1.0f;
		this->effect_Life = 30;
		this->scale = ML::Vec3(100.0f, 100.0f, 100.0f);
		this->pos.y += 100.0f;

		break;

	case EnemyLanding: //3
		this->meshName = "EnemyLanding";
		this->filePath = "./data/mesh/effect/EnemyLanding.SOB";
		this->alpha =0.6f;
		this->effect_Life = 30;
		this->scale = ML::Vec3(0, 20.0f, 0);
		this->pos.y -= 40;
		easing::Set("alpha_EnemyLanding", easing::EXPOIN, 0.6f, 0, 30);
		easing::Set("scaleXZ_EnemyLanding", easing::EXPOOUT, 0, 160, 30);

		break;
		
	case Hit_to_Enemy://4
		this->meshName = "Hit_to_Enemy";
		this->filePath = "./data/mesh/effect/Hit_the_Enemy.SOB";
		this->alpha = 0.0f;
		this->effect_Life = 60;
		this->scale = ML::Vec3(600.0f, 600.0f, 600.0f);

		break;

	case Hit_to_Wall: //5
		this->meshName = "Hit_to_Wall";
		this->filePath = "./data/mesh/effect/Hit_the_Wall.SOB";
		this->alpha = 0.0f;
		this->effect_Life = 10;
		this->scale = ML::Vec3(50.0f, 50.0f, 50.0f);
		
		break;

	case CreateTail: //6
		this->meshName = "CreateTail";
		this->filePath = "./data/mesh/effect/CreateTail.SOB";
		this->alpha = 1.0f;
		this->effect_Life = 67;

		break;

	case DestroyTail://7
		this->meshName = "DestroyTail";
		this->filePath = "./data/mesh/effect/DestroyTail.SOB";
		this->alpha = 0.8f;
		this->effect_Life = 300;
		this->scale = ML::Vec3(100.0f, 100.0f, 100.0f);
		easing::Set("posY_DestroyTail", easing::BACKIN, this->pos.y+100.0f, this->pos.y + 1500.0f, 300);
		easing::Set("scaleY_DestroyTail", easing::CUBICOUT, 0, 100, 200);
		

		break;

	case Hit_to_Tail: //8
		this->meshName = "Hit_to_Tail";
		this->filePath = "./data/mesh/effect/Hit_the_Enemy.SOB";
		this->alpha = 0.0f;
		this->effect_Life = 60;
		this->scale = ML::Vec3(600.0f, 600.0f, 600.0f);

		break;
		
	case DestroyCharactor: //9
		this->meshName = "DestroyCharactor";
		this->filePath = "./data/mesh/effect/DestroyPlayer.SOB";
		this->alpha = 0.0f;
		this->effect_Life = 300;
		this->scale = ML::Vec3(0.0f, 0.0f, 0.0f);

		easing::Set("scale_EF9", easing::CUBICIN, 130.0f, 0, 20);

		break;
	default:
		return;
		break;
	
	}	
}


void BEffect::Eff_Initialize(ML::Vec3 pos, ML::Vec3 angle, effType handle)
{
	this->pos = pos;
	this->angle = angle;
	this->playing_EffectHandle = handle;

	LoadEffect(handle);	
}