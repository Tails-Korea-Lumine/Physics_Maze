#include "Bbox.h"
#include "MyMath.h"

#define TRIANGLE_ON_CUBE 12
#define VERTEX_ON_CUBE 8


void Bbox::Rotate_Box(ML::Mat4x4* mat, const ML::QT& q)
{
	//��]�ʂƈʒu���X�V
	this->pos = mat->TransformCoord(pos);
	this->boxQT *= q;
}

void Bbox::Get_Triangle_Box3D(std::vector<Triangle>* result, const ML::Box3D& box, const ML::QT& rotation) const
{
	//�O�p�`���Ƃ��ɕۑ�����ꏊ
	//std::vector<Triangle> tri;

	//6�ʑ̂ɂ�12�̎O�p�`������
	Triangle t[TRIANGLE_ON_CUBE] = {};

	//�e���_�������C���f�b�N�X
	ML::Vec3 vertex_Index[VERTEX_ON_CUBE] =
	{
		ML::Vec3(box.x,box.y,box.z),//0
		ML::Vec3(box.x,box.y,box.z + box.d),//1
		ML::Vec3(box.x + box.w,box.y,box.z + box.d),//2
		ML::Vec3(box.x + box.w,box.y,box.z),//3
		ML::Vec3(box.x,box.y + box.h,box.z),//4
		ML::Vec3(box.x,box.y + box.h,box.z + box.d),//5
		ML::Vec3(box.x + box.w,box.y + box.h,box.z + box.d),//6
		ML::Vec3(box.x + box.w,box.y + box.h,box.z)//7
	};

	//�e���_�����[���h��]�ʂŉ�]������
	ML::Mat4x4 matR;
	//��]�s������
	//matR.RotationQuaternion(rotation);
	ML::Vec3 center = { box.x + (box.w / 2), box.y + (box.h / 2), box.z + (box.d / 2) };
	D3DXMatrixAffineTransformation(&matR, 1, &center, &rotation, NULL);

	for (int i = 0; i < VERTEX_ON_CUBE; i++)
	{
		vertex_Index[i] = matR.TransformCoord(vertex_Index[i]);
	}


	//�O�p�`�̏�����
	t[0] =
	{
		vertex_Index[0],
		vertex_Index[3],
		vertex_Index[4],
	};
	MyMath::Get_Normal_to_Vector_Cross(&t[0].normal, (t[0].c - t[0].a), (t[0].b - t[0].a));
	t[1] =
	{
		vertex_Index[3],
		vertex_Index[4],
		vertex_Index[7],
	};
	MyMath::Get_Normal_to_Vector_Cross(&t[1].normal, (t[1].b - t[1].a), (t[1].c - t[1].a));
	t[2] =
	{
		vertex_Index[1],
		vertex_Index[0],
		vertex_Index[5],
	};
	MyMath::Get_Normal_to_Vector_Cross(&t[2].normal, (t[2].c - t[2].a), (t[2].b - t[2].a));
	t[3] =
	{
		vertex_Index[0],
		vertex_Index[5],
		vertex_Index[4],
	};
	MyMath::Get_Normal_to_Vector_Cross(&t[3].normal, (t[3].b - t[3].a), (t[3].c - t[3].a));
	t[4] =
	{
		vertex_Index[1],
		vertex_Index[5],
		vertex_Index[2],

	};
	MyMath::Get_Normal_to_Vector_Cross(&t[4].normal, (t[4].c - t[4].a), (t[4].b - t[4].a));
	t[5] =
	{
		vertex_Index[5],
		vertex_Index[2],
		vertex_Index[6],

	};
	MyMath::Get_Normal_to_Vector_Cross(&t[5].normal, (t[5].b - t[5].a), (t[5].c - t[5].a));
	t[6] =
	{
		vertex_Index[3],
		vertex_Index[7],
		vertex_Index[2],

	};
	MyMath::Get_Normal_to_Vector_Cross(&t[6].normal, (t[6].b - t[6].a), (t[4].c - t[6].a));
	t[7] =
	{
		vertex_Index[7],
		vertex_Index[2],
		vertex_Index[6],

	};
	MyMath::Get_Normal_to_Vector_Cross(&t[7].normal, (t[7].c - t[7].a), (t[7].b - t[7].a));
	t[8] =
	{
		vertex_Index[4],
		vertex_Index[7],
		vertex_Index[5],

	};
	MyMath::Get_Normal_to_Vector_Cross(&t[8].normal, (t[8].c - t[8].a), (t[8].b - t[8].a));
	t[9] =
	{
		vertex_Index[7],
		vertex_Index[5],
		vertex_Index[6],
	};
	MyMath::Get_Normal_to_Vector_Cross(&t[9].normal, (t[9].b - t[9].a), (t[9].c - t[9].a));
	t[10] =
	{
		vertex_Index[0],
		vertex_Index[3],
		vertex_Index[1],

	};
	MyMath::Get_Normal_to_Vector_Cross(&t[10].normal, (t[10].b - t[10].a), (t[10].c - t[10].a));
	t[11] =
	{
		vertex_Index[3],
		vertex_Index[1],
		vertex_Index[2],
	};
	MyMath::Get_Normal_to_Vector_Cross(&t[11].normal, (t[11].c - t[11].a), (t[11].b - t[11].a));

	//std::vector result�ɓo�^������
	for (int i = 0; i < TRIANGLE_ON_CUBE; i++)
	{
		t[i].normal = t[i].normal.Normalize();
		result->push_back(t[i]);
	}
}

//Box3d�Ƌ��̂̍ŒZ�����̓_�����
void Bbox::Get_ShortisetPoints_Box_to_Sphere(std::vector<ML::Vec3>* result, const ML::Box3D& box) const
{

	//ver2.0
	//�����ł�������{�b�N�X�ɊO�ڂ��鋅�ɓ����_�������c��	
	//�O�ڂ��鋅�̔����a
	float outer_Sphere_R = abs(ROOT3 / 2.0f*(box.w));	

	result->erase(remove_if(result->begin(), result->end(),
		[&](ML::Vec3& p)->bool {return ML::Vec3(p - this->Get_Pos()).Length() > outer_Sphere_R; }
	),
		result->end()
	);

}

void Bbox::Get_Collision_Poligon(std::vector<After_Collision>* result, std::vector<ML::Vec3> all_Points, const ML::Vec3& pos, const float& r, const ML::Vec3& speed)
{
	//�ʒu�␳���������蔻��p��`
	ML::Box3D area = this->collision_Base.OffsetCopy(this->pos);

	//�����̎O�p�`�����o��
	std::vector<Triangle> all_Tri;
	this->Get_Triangle_Box3D(&all_Tri, area, this->boxQT);	

	//��������{�[���̃h�b�g���玩���ɋ߂��ꕔ�������c��
	this->Get_ShortisetPoints_Box_to_Sphere(&all_Points, area);

	//�����蔻�茋�ʂ�result�ɕۑ�
	if (!this->col.Hit_Check(result,all_Tri, this->pos, area.w, all_Points, pos, r, speed, this->boxQT))
	{
		//������Ȃ������������[���x�N�g�����U���g��pushback����
		result->push_back(After_Collision());
	}
}

BoxType Bbox::What_Type_Is_this_Box() const
{
	return this->chip;
}

const ML::Vec3& Bbox::Get_Pos() const
{
	return this->pos;
}

//�����Ȃ��R���X�g���N�^(�[���N���A)
Bbox::Bbox()
{
	this->chip = BoxType::Clear;
	this->pos = ML::Vec3(0, 0, 0);
	this->collision_Base = ML::Box3D(0, 0, 0, 0, 0, 0);
	this->boxQT = ML::QT(0.0f);
	
}
//���� : (���̃^�C�v,�ʒu,�����蔻���`,������]��)
Bbox::Bbox(BoxType chip, ML::Vec3 pos, ML::Box3D base, ML::QT qt)
{
	this->chip = chip;
	this->pos = pos;
	this->collision_Base = base;
	this->boxQT = qt;
	
}


//�������Ă邩�������ĂȂ����_���I�Ȍ��ʂ��������߂鏈��
bool Bbox::Get_Collision_Bool(std::vector<ML::Vec3>& all_Points, const ML::Vec3& pos, const float& r, const ML::Vec3& speed)
{
	//��񂠂��蔻����s��
	std::vector<After_Collision> check;

	this->Get_Collision_Poligon(&check, all_Points, pos, r, speed);

	//������Ȃ��������̓��F�N�^�[�̈�ԑO�̃t���O��false�ŕԂ����̂�
	//��ԑO�̃t���O�Ŕ��f����
	return check[0].collision_Flag;
}