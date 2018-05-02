#pragma once
//Base of Box Class

#include "GameEngine_Ver3_7.h"
#include "Collision.h"
#include "MyPG.h"

enum BoxType
{
	Clear = -1, //�����Ȃ�
	Road = 0, //��
	Wall = 1, //��
	Obstacle0 = 2, //��Q���O��(����)
	Obstacle1 = 3, //��Q���O��(����)
	Obstacle2 = 4, //��Q���O��(����)
	Obstacle3 = 5, //��Q���O��(����)

	Core = 40, //�^�񒆂̂ł����L���[�u
	Barrier = 50, //�O�ɔ�΂��Ȃ��悤�ɂ��铧���ȕ�
	//�ǉ��̗]�n����
};

class Bbox 
{
private:
	BoxType chip;//�}�X�̃}�b�v�`�b�v
	ML::Vec3 pos;//���S�_�̍��W
	ML::Box3D collision_Base;//���̔���͈�
	ML::QT boxQT;//�}�b�v�̉�]��
	Collision col;
	
public:

	//���\�b�h	
	//��]�֐��@���� : (��]�����ʒu , ��]��)
	void Rotate_Box(ML::Vec3 pos, ML::QT q);
	
	//�����ǂ������m�F����
	BoxType What_Type_Is_this_Box();
	//�ʒu��Ԃ��֐�
	ML::Vec3 Get_Pos();
	//�Փ˔���
	std::vector<After_Collision> Get_Collision_Poligon(ML::Vec3 pos, float r, ML::Vec3 speed);

	//�R���X�g���N�^�E�f�X�g���N�^
	Bbox();
	Bbox(BoxType chip, ML::Vec3 pos, ML::Box3D base, ML::QT qt);
	virtual ~Bbox(){}
};