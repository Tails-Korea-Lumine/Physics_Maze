#pragma once
#include "BEffect.h"

//����ǂ�����Ƃ��̃G�t�F�N�g�N���X

class Crashed : public BEffect
{
public:
	//�G�t�F�N�g�s��
	void Effect_Update() override;
	//�`��
	//void Effect_Draw() const override;


	//�R���X�g���N�^
	Crashed(const ML::Vec3& pos, const ML::Vec3& angle, const string& mesh_name, const string& sound_Name);
};