#pragma warning(disable:4996)
#pragma once
//Base of Box Class

//#include "GameEngine_Ver3_7.h"
#include "Shape.h"
//#include "MyPG.h"

//�{�b�N�X�̖�
//�����蔻��s�v�|���S���\���Ɏg�p����
enum Box_Side
{
	Xplus, Xminus,
	Yplus, Yminus,
	Zplus, Zminus,
};

class Bbox 
{
public:
	enum class BoxType
	{
		Clear = -1, //�����Ȃ�
		Road = 0, //��
		Wall = 1, //��
		Goal = 2, //�S�[���ʒu
		Teleportation = 3, //�e���|�[�e�[�V����
		LightSwitch = 4, //���C�g�̃X�C�b�`//�X�C�b�`�͐�΂ɍs���~�܂�ɐݒu���Ȃ�
		Unstable_Wall = 5, //�����

		Core = 40, //�^�񒆂̂ł����L���[�u
		Barrier = 50, //�O�ɔ�΂��Ȃ��悤�ɂ��铧���ȕ�
		Start = 60,//�X�^�[�g�ʒu
				   //�ǉ��̗]�n����
	};
protected:
	BoxType chip;//�}�X�̃}�b�v�`�b�v
	//ML::Vec3 pos;//���S�_�̍��W
	Shape3D* collision_Base;//���̔���͈�
	//ML::QT boxQT;//�}�b�v�̉�]��
	//�{�b�N�X��ID
	string box_Id;
	//���b�V����
	string mesh_Name;
	
public:	
	//���\�b�h	
	//��]�֐��@���� : (��]�����ʒu , ��]��)
	virtual void Rotate_Box(ML::Mat4x4* mat, const ML::QT& q);
	//�Փ˔���
	//����̌��ʒl�����炤�֐�
	virtual bool Collision_Action(std::vector<Collision_Data>* result, Shape3D* ball) = 0;
	//�����_�����O����
	//�M�~�b�N������Ƀ����_�����O���Ⴄ���̂̂��߂ɉ��z�֐��ɂ��Ă���
	virtual void Rendering()const;

	//�����蔻��ɕK�v�Ȃ��O�p�`��\�����Ă���
	void Marking_On_Unusable_Side(const Box_Side&);	
	
	//BoxType���m�F����
	BoxType What_Type_Is_this_Box() const;
	//�ʒu��Ԃ��֐�
	ML::Vec3 Get_Pos() const;
	//id��Ԃ��֐�
	string Get_Id() const;	

	//�R���X�g���N�^�E�f�X�g���N�^
	//�����Ȃ��R���X�g���N�^(�[���N���A)
	Bbox();
	//���� : (���̃^�C�v,�ʒu,�����蔻���`,������]��,�{�b�N�X��ID�A���b�V����)
	Bbox(const ML::Vec3& pos, const ML::Vec3& half_Of_Length, const ML::QT& qt, const string& id, const string& mesh_Name);
	

	virtual ~Bbox();	
};