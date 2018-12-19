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

void Bbox::Rendering() const
{
	//�s�񐶐�
	ML::Mat4x4 matRT,matS;
	//�X�P�[�����O
	matS.Scaling(this->collision_Base->Get_Length()*2.0f);
	//�A�t�B���ϊ�
	D3DXMatrixAffineTransformation(&matRT, 1.0f, NULL, &this->collision_Base->Get_Quaternion(), &this->collision_Base->Get_Center());
	//���[���h�s��ɏ㏑��
	DG::EffectState().param.matWorld = matS * matRT;
	//�����_�����O
	DG::Mesh_Draw(this->mesh_Name);
}


//�����蔻��ɕK�v�Ȃ��O�p�`��\�����Ă���
void Bbox::Marking_On_Unusable_Side(const Box_Side& side)
{
	//�����̕����ɑ�������ԍ��̂Ƃ�����}�[�L���O
	switch (side)
	{
	case Box_Side::Xplus:
		this->collision_Base->Marking_On_Unsuable_Poligon(6) ;
		this->collision_Base->Marking_On_Unsuable_Poligon(7) ;
		break;
	case Box_Side::Xminus:
		this->collision_Base->Marking_On_Unsuable_Poligon(2) ;
		this->collision_Base->Marking_On_Unsuable_Poligon(3) ;
		break;
	case Box_Side::Yplus:
		this->collision_Base->Marking_On_Unsuable_Poligon(8) ;
		this->collision_Base->Marking_On_Unsuable_Poligon(9) ;
		break;
	case Box_Side::Yminus:
		this->collision_Base->Marking_On_Unsuable_Poligon(10);
		this->collision_Base->Marking_On_Unsuable_Poligon(11);
		break;
	case Box_Side::Zplus:
		this->collision_Base->Marking_On_Unsuable_Poligon(4) ;
		this->collision_Base->Marking_On_Unsuable_Poligon(5) ;
		break;
	case Box_Side::Zminus:
		this->collision_Base->Marking_On_Unsuable_Poligon(0) ;
		this->collision_Base->Marking_On_Unsuable_Poligon(1) ;
		break;
	}
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
	this->collision_Base = nullptr;	
}
//���� : (���̃^�C�v,�ʒu,�����蔻���`,������]��,�{�b�N�X��ID�A���b�V����)
Bbox::Bbox(const ML::Vec3& pos, const ML::Vec3& half_Of_Length, const ML::QT& qt, const string& id, const string& mesh_Name)
	:box_Id(id)
	,mesh_Name(mesh_Name)
{
	this->chip = BoxType::Clear;	
	this->collision_Base = new Cube(pos, half_Of_Length, qt);	
}

Bbox::~Bbox()
{
	if (this->collision_Base != nullptr)
	{
		delete this->collision_Base;
	}
	DG::Mesh_Erase(this->mesh_Name);
}