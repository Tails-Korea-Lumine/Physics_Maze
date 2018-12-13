#pragma once
#include "BEffect.h"

//�G�t�F�N�g�Q�[���N���A�N���X

class Game_Clear : public BEffect
{

public:

	//�G�t�F�N�g�s��
	void Effect_Update() override;
	//�`��
	void Effect_Draw() const override;
	

	//�R���X�g���N�^
	Game_Clear(const ML::Vec3& pos, const ML::Vec3& angle, const string& mesh_name, const string& sound_Name);
};