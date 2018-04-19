#pragma once
//Base of Box Class

#include "GameEngine_Ver3_7.h"
#include "Collision.h"
#include "MyPG.h"

struct mapData
{
	int chip;//�}�X�̃}�b�v�`�b�v
	ML::Vec3 pos;//���S�_�̍��W
	ML::Box3D collision_Base;//���̔���͈�
};

class Bbox : public  BTask
{
public:

	//�ϐ�
	mapData arr[30][30][30];
	int sizeZ, sizeY, sizeX;
	float chipSize;
	ML::Box3D hitBase;//�}�b�v�S�̂̔���͈�
	string chipName[10];
	After_Collision collision_Tri;//�{�[���̂ق��ɓn�����
	std::vector< After_Collision> col_Poligons;

	ML::QT map_QT;//�}�b�v�̉�]��
	Collision col;


	//���\�b�h
	bool Map_Load(string f_);
	bool Map_Check_Hit(const ML::Box3D& hit);
	void Map_Check_Hit(ML::Vec3 pos, float r, ML::Vec3 speed);//���ƃ}�b�v�̂����蔻��		
	void Map_Rotate();//Update�ŌĂԊ֐�
	

	//�R���X�g���N�^�E�f�X�g���N�^
	Bbox();
	virtual ~Bbox(){}
};