#pragma once
#include "BEffect.h"

//�G�t�F�N�g�e���|�[�g�C���̃N���X

class Teleport_In : public BEffect
{
public:

	//�G�t�F�N�g�s��
	void Effect_Update() override;
	//�`��
	void Effect_Draw() const override;	

	//�R���X�g���N�^
	Teleport_In(const ML::Vec3& pos, const ML::Vec3& target, const ML::Vec3& angle, const string& mesh_name, const string& sound_Name);
};