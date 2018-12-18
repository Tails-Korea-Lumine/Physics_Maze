#pragma once
#include "Bbox.h"

//��Q���ǂ̃N���X

class Wall : public Bbox
{
public:
	//�����蔻��̍s��
	bool Collision_Action(std::vector<Collision_Data>* result, Shape3D* ball) override;

	//�R���X�g���N�^
	Wall(const ML::Vec3& pos, const ML::Vec3& half_Of_Length, const ML::QT& qt, const string id);
};