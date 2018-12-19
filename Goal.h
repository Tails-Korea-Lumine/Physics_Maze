#pragma once
#include "Bbox.h"

//�ړI�n�̃N���X

class Goal : public Bbox
{
public:
	//�����蔻��̍s��
	bool Collision_Action(std::vector<Collision_Data>* result, Shape3D* ball) override;
	//�����_�����O����
	void Rendering()const override;
	//�R���X�g���N�^
	Goal(const ML::Vec3& pos, const ML::Vec3& half_Of_Length, const ML::QT& qt, const string& id, const string& mesh_Name);
};