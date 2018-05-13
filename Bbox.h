#pragma warning(disable:4996)
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
	Goal = 2, //�S�[���ʒu
	Teleportaion = 3, //�e���|�[�e�[�V����
	LightSwitch = 4, //���C�g�̃X�C�b�`//�X�C�b�`�͐�΂ɍs���~�܂�ɐݒu���Ȃ�
	Obstacle3 = 5, //��Q���O��(����)

	Core = 40, //�^�񒆂̂ł����L���[�u
	Barrier = 50, //�O�ɔ�΂��Ȃ��悤�ɂ��铧���ȕ�
	Start = 60,//�X�^�[�g�ʒu
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
	void Get_Collision_Poligon(std::vector<After_Collision>* result, const ML::Vec3& pos, const float& r, const ML::Vec3& speed);
	//�Q�[���N���A����
	bool Player_was_Clear_the_Game(const ML::Vec3& pos, const float& r, const ML::Vec3 speed);
	//�e���|�[�g���邩�𔻒�
	bool Player_was_Hit_the_Door(const ML::Vec3& pos, const float& r, const ML::Vec3 speed);
	//���C�g�̃X�C�b�`�����������𔻕�
	bool Player_Turnoff_the_Switch(const ML::Vec3& pos, const float& r, const ML::Vec3 speed);

	//�R���X�g���N�^�E�f�X�g���N�^
	Bbox();
	Bbox(BoxType chip, ML::Vec3 pos, ML::Box3D base, ML::QT qt);
	virtual ~Bbox(){}
};