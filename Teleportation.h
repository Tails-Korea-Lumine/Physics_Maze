#pragma once
#include "Bbox.h"

//��Q���e���|�[�g�N���X

class Teleportation : public Bbox
{
private:
	//���݂��Ă���ʂ̔ԍ�
	unsigned int side_Number;
public:
	//�����蔻��̍s��
	bool Collision_Action(std::vector<Collision_Data>* result, Shape3D* ball) const override;
	//��]
	void Rotate_Box(ML::Mat4x4* mat, const ML::QT& q) override;

	//�R���X�g���N�^
	Teleportation(const ML::Vec3& pos, const ML::Vec3& half_Of_Length, const ML::QT& qt, const string id, const int& side_Number);
};