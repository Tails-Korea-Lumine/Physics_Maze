#include "Cube.h"
#include "Collision.h"

#define TRIANGLE_ON_CUBE 12
#define VERTEX_ON_CUBE 8

void Cube::Get_Triangle_Box3D(std::vector<Triangle>* result) const
{
	//�O�p�`���Ƃ��ɕۑ�����ꏊ
	//std::vector<Triangle> tri;

	//6�ʑ̂ɂ�12�̎O�p�`������
	Triangle t[TRIANGLE_ON_CUBE] = {};

	//�e���_�������C���f�b�N�X
	ML::Vec3 vertex_Index[VERTEX_ON_CUBE] =
	{
		ML::Vec3(this->center.x - this->half_of_Length.x, this->center.y - this->half_of_Length.y, this->center.z - this->half_of_Length.z),//0
		ML::Vec3(this->center.x - this->half_of_Length.x, this->center.y - this->half_of_Length.y, this->center.z + this->half_of_Length.z),//1
		ML::Vec3(this->center.x + this->half_of_Length.x, this->center.y - this->half_of_Length.y, this->center.z + this->half_of_Length.z),//2
		ML::Vec3(this->center.x + this->half_of_Length.x, this->center.y - this->half_of_Length.y, this->center.z - this->half_of_Length.z),//3
		ML::Vec3(this->center.x - this->half_of_Length.x, this->center.y + this->half_of_Length.y, this->center.z - this->half_of_Length.z),//4
		ML::Vec3(this->center.x - this->half_of_Length.x, this->center.y + this->half_of_Length.y, this->center.z + this->half_of_Length.z),//5
		ML::Vec3(this->center.x + this->half_of_Length.x, this->center.y + this->half_of_Length.y, this->center.z + this->half_of_Length.z),//6
		ML::Vec3(this->center.x + this->half_of_Length.x, this->center.y + this->half_of_Length.y, this->center.z - this->half_of_Length.z)//7
	};

	//�e���_�����[���h��]�ʂŉ�]������
	ML::Mat4x4 matR;
	//��]�s������	
	D3DXMatrixAffineTransformation(&matR, 1, &this->center, &this->quaternion, NULL);

	for (int i = 0; i < VERTEX_ON_CUBE; i++)
	{
		vertex_Index[i] = matR.TransformCoord(vertex_Index[i]);
	}

	//�O�p�`�̏�����
	//z-
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
	//x-
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
	//z+
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
	//x+
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
	//y+
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
	//y-
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

bool Cube::Hit(std::vector<Collision_Data>* result, Shape3D* other, const bool unsuable_Triangle[])
{
	//���Ƃ̂����蔻�肾����z�肵�Ă���̂ŁA���̈ȊO�������画�肵�Ȃ�
	if (other->type != Shape3D_Type::Type_Sphere)
	{
		return false;
	}

	//���ƃL���[�u���t��]���āA�ŒZ�����̓_�����
	ML::Vec3 nearest_Point;
	if (Collision::Intersect_AABB_Sphere(&nearest_Point, this, other) == false)
	{
		return false;
	}

	//����
	return Collision::Check_Collision_Cube_Sphere(result, this, nearest_Point, unsuable_Triangle) ;
	
}


//�R���X�g���N�^
Cube::Cube(const ML::Vec3& center, const ML::Vec3& length_H, const ML::QT& qt)
	:Shape3D(center,length_H,qt)
{
	this->type = Shape3D_Type::Type_Cube;
}