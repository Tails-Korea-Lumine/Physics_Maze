#pragma once
#include "BEffect.h"

//���Ă����G�t�F�N�g�N���X

class Breaking : public BEffect
{
public:
	//�G�t�F�N�g�s��
	void Effect_Update() override;
	//�`��
	void Effect_Draw() const override;


	//�R���X�g���N�^
	Breaking(const ML::Vec3& pos, const ML::Vec3& angle, const string& mesh_name, const string& sound_Name);
};