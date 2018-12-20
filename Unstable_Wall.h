#pragma once
#include "Bbox.h"

//����ǃN���X


class Unstable_Wall : public Bbox
{
private:
	//����܂ł̃��C�t
	float life;
	//�G�t�F�N�g��A���Ő��������Ȃ����߂̃t���O
	bool hit_Flag;
	//���C�t��������
	void Decrease_Life(const float& dec);
	//�G�t�F�N�g����
	void Be_Effect(const ML::Vec3& );
public:
	//�����蔻��̍s��
	bool Collision_Action(std::vector<Collision_Data>* result, Shape3D* ball) override;
	//�����_�����O����
	void Rendering()const override;	

	//�R���X�g���N�^
	Unstable_Wall(const ML::Vec3& pos, const ML::Vec3& half_Of_Length, const ML::QT& qt, const string& id, const string& mesh_Name);

};