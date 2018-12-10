#pragma once
#include "Bbox.h"

//��Q�����C�g�X�C�b�`�N���X

class Light_Switch : public Bbox
{
public:
	//�����蔻��̍s��
	bool Collision_Action(std::vector<Collision_Data>* result, Shape3D* ball) const override;

	//�R���X�g���N�^
	Light_Switch(const ML::Vec3& pos, const ML::Vec3& half_Of_Length, const ML::QT& qt, const string id);
};