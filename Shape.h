#pragma warning(disable:4996)
#pragma once
#include "MyPG.h"
//#include "Collision.h"


//3D�}�`�̊�{�`

class Shape3D
{
public:
	enum Shape3D_Type
	{
		Type_Clear = -1,//�|�����t����ĂȂ���
		Type_Cube = 1,//�Z�ʑ�
		Type_Sphere = 2,//��
	};

	//���S�_
	ML::Vec3 center;
	//�e���̒����̔���
	ML::Vec3 half_of_Length;
	//��]��
	ML::QT quaternion;
	//�ǂ̂��̂Ƀ|�����t���ꂽ���m�F���郁���o�[
	Shape3D_Type type;
	//�O�p�`�Ŕ�����s���ꍇ�A�g���Ȃ��O�p�`���m�F����t���O
	bool unusable_Triangle[12];

private:	

	
public:
	//�Q�b�^�[
	//���S�_
	ML::Vec3 Get_Center() const;
	//��������
	ML::Vec3 Get_Length() const;
	//��]��
	ML::QT Get_Quaternion() const;	

	//���z�֐�
	//�\���|���S��
	virtual void Get_Triangle_Box3D(std::vector<Triangle>* result) const =0;
	//�����蔻��
	//���� : (�����ۑ����郔�F�N�^�[�A������s��3�����}�`�A��O�I�ɔ�������Ȃ������̃����_��)
	virtual bool Hit(std::vector<Collision_Data>* result, Shape3D* other) =0;

	//�ړ�
	//���� : (�ړ���)
	void Offset(const ML::Vec3& vec);
	//�ʒu�̏㏑��
	//���� : (�㏑������ꏊ)
	void Set_Position(const ML::Vec3& pos);

	//��]
	//���� : (��]��)
	void Rotation(ML::Mat4x4* mat, const ML::QT& qt);

	//���肵�Ȃ��|���S���Ƀ}�[�L���O���Ă�������
	void Marking_On_Unsuable_Poligon(const size_t&);

	//�R���X�g���N�^
	//���� : (���S�_�A�����̔����A�����̉�]��)
	Shape3D(const ML::Vec3& center, const ML::Vec3& length_H, const ML::QT& qt);
	//�f�X�g���N�^
	virtual ~Shape3D(){}
};