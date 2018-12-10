#include "Bbox.h"
#include "Cube.h"
#include "MyMath.h"

#define TRIANGLE_ON_CUBE 12
#define VERTEX_ON_CUBE 8


void Bbox::Rotate_Box(ML::Mat4x4* mat, const ML::QT& q)
{
	//��]�ʂƈʒu���X�V
	this->collision_Base->Rotation(mat, q);
}


//�����蔻��ɕK�v�Ȃ��O�p�`��\�����Ă���
void Bbox::Marking_On_Unusable_Side(const Box_Side& side)
{
	//�����̕����ɑ�������ԍ��̂Ƃ�����}�[�L���O
	switch (side)
	{
	case Box_Side::Xplus:
		this->unusable_Triagle[6] = true;
		this->unusable_Triagle[7] = true;
		break;
	case Box_Side::Xminus:
		this->unusable_Triagle[2] = true;
		this->unusable_Triagle[3] = true;
		break;
	case Box_Side::Yplus:
		this->unusable_Triagle[8] = true;
		this->unusable_Triagle[9] = true;
		break;
	case Box_Side::Yminus:
		this->unusable_Triagle[10] = true;
		this->unusable_Triagle[11] = true;
		break;
	case Box_Side::Zplus:
		this->unusable_Triagle[4] = true;
		this->unusable_Triagle[5] = true;
		break;
	case Box_Side::Zminus:
		this->unusable_Triagle[0] = true;
		this->unusable_Triagle[1] = true;
		break;
	}
}

bool Bbox::Get_Marking_Unsuable_Side(const int& i)
{
	//�͈͊O�Ȃ�false��Ԃ�
	if (i > TRIANGLE_ON_CUBE || i < 0)
	{
		return false;
	}

	//���肪����Ȃ��O�p�`�Ȃ�true��Ԃ�
	return this->unusable_Triagle[i];
}

Bbox::BoxType Bbox::What_Type_Is_this_Box() const
{
	return this->chip;
}

ML::Vec3 Bbox::Get_Pos() const
{
	return this->collision_Base->Get_Center();
}

string Bbox::Get_Id() const
{
	return this->box_Id;
}

//�����Ȃ��R���X�g���N�^(�[���N���A)
Bbox::Bbox()
{
	this->chip = BoxType::Clear;
	//this->pos = ML::Vec3(0, 0, 0);
	this->collision_Base = nullptr;
	//this->boxQT = ML::QT(0.0f);
	for (int i = 0; i < TRIANGLE_ON_CUBE; i++)
	{
		this->unusable_Triagle[i] = true;
	}
}
//���� : (���̃^�C�v,�ʒu,�����蔻���`,������]��,�{�b�N�X��ID)
Bbox::Bbox(const ML::Vec3& pos, const ML::Vec3& half_Of_Length, const ML::QT& qt, const string id)
	:box_Id(id)
{
	this->chip = BoxType::Clear;
	//this->pos = pos;
	this->collision_Base = new Cube(pos, half_Of_Length, qt);
	//this->boxQT = qt;
	for (int i = 0; i < TRIANGLE_ON_CUBE; i++)
	{
		this->unusable_Triagle[i] = false;
	}
}