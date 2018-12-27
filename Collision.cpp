#include "Collision.h"
#include "Sphere.h"
#include "Cube.h"


bool Collision::Check_Collision_Triangle_Point(const Triangle& tri, const ML::Vec3& p)
{
	//�Ƃ���_p�ƎO�p�`�̓����蔻��
	//p����O�p�`�̊e���_�ւ̃x�N�g��A,B,C
	//����A,B,C���\������p�x�̘a��360	��������
	//�_���͎O�p�`�̕ӂ̏�܂��͓���Ă���B

	ML::Vec3 A, B, C;
	A = tri.a - p;
	B = tri.b - p;
	C = tri.c - p;

	//�O�p�`�̒��_�Ɠ������Ȃ����ꍇ
	//if (A.Length() == 0.0f || B.Length() == 0.0f || C.Length() == 0.0f)
	if(A.Is_Zero_Vec() || B.Is_Zero_Vec() || C.Is_Zero_Vec())
	{
		return true; //�v�Z�s�\���O�p�`�ɓ����Ă���̂ł������Ă锻��ŕԂ�
	}	
	

	//alpha �� A��B�̊p�x
	//beta = B��C�̊p�x
	//gamma = A��C�̊p�x
	float cosAlpha;
	float cosBeta;
	float cosGamma;
	float sinAlpha;
	float sinBeta;
	float sinGamma;

	MyMath::Vector_Dot(&cosAlpha,A, B);
	MyMath::Vector_Dot(&cosBeta, B, C);
	MyMath::Vector_Dot(&cosGamma, A, C);
	MyMath::Vector_Cross(&sinAlpha, A, B);
	MyMath::Vector_Cross(&sinBeta, B, C);
	MyMath::Vector_Cross(&sinGamma, A, C);

	float check;

	//cos(alpha + beta + gamma)�̓W�J��
	check = (cosGamma*((cosAlpha * cosBeta) - (sinAlpha * sinBeta))) - (sinGamma * ((cosAlpha*sinBeta) + (sinAlpha*cosBeta)));

	//�덷�܂Ŋm�F(�덷�͈͕̔͂ς��]�n������ 2018/03/16)
	//0.99756405026
	return check >= judge;
}

//OBB��AABB�ɖ߂��Ĕ���
void Collision::Intersect_OBB_Sphere(ML::Vec3* result, const Shape3D* owner, const Shape3D* visitor)
{
	//���܂ł̉�]�ʂŋt�s������
	ML::Mat4x4 matR, matIR;
	//�L���[�u�̏���������Ă���
	ML::Vec3 o_Center = owner->Get_Center();
	ML::Vec3 o_Halfl = owner->Get_Length();

	D3DXMatrixAffineTransformation(&matR, 1.0f, &owner->Get_Center(), &owner->Get_Quaternion(), NULL);
	matIR = matR.Inverse();

	//�t�s��ɉ����ĉ�]�������̒��S�_
	ML::Vec3 alined_Sphere_Center = matIR.TransformCoord(visitor->Get_Center());

	//��ԋ߂��_���v�Z
	float x = max(o_Center.x - o_Halfl.x, min(alined_Sphere_Center.x, o_Center.x + o_Halfl.x));					  
	float y = max(o_Center.y - o_Halfl.y, min(alined_Sphere_Center.y, o_Center.y + o_Halfl.y));					  
	float z = max(o_Center.z - o_Halfl.z, min(alined_Sphere_Center.z, o_Center.z + o_Halfl.z));
	
	//���΋����x�N�g�����������
	ML::Vec3 dist = ML::Vec3(x, y, z) - alined_Sphere_Center;
	//���΃x�N�g���̒����ɂ���ċ߂��_���v�Z
	*result = dist.Length() <= visitor->Get_Length().x ? ML::Vec3(x, y, z) : alined_Sphere_Center + dist.Normalize() * -visitor->Get_Length().x;
	//��]�����ɖ߂��ď����I��
	*result = matR.TransformCoord(*result);
}


//�}�X�ʂɌĂ΂��֐�
bool Collision::Check_Collision_Cube_Sphere(std::vector<Collision_Data>* result, const Shape3D* owner, const ML::Vec3& nearest_point)
{		
	//�ꎞ�I�ɔ���̖@���x�N�g����ۑ����Ă����R���e�i
	std::vector<ML::Vec3> datas;
	//�R���X�g���N�^�ɂ���ă[���x�N�g����false�Ő��������
	Collision_Data cd;

	//����p�̎O�p�`�����o��
	std::vector<Triangle> all_Tri;
	owner->Get_Triangle_Box3D(&all_Tri);

	for (auto& tri : all_Tri)
	{
		//Collision_Data first;
		//��O�łȂ����O�p�`�Ɠ_�̂����蔻�肪�����Ă�ꍇ�ŕۑ�����
		if (Check_Collision_Triangle_Point(tri, nearest_point))
		{
			//������������̃f�[�^���R���e�i�[�ɕۑ�				
			datas.push_back(tri.normal);
		}
	}
	//�{�b�N�X1�ɓ����ɉ��̃|���S���������ɓ���������
	//�@���x�N�g������������
	if (datas.empty() == false)
	{
		cd.collision_Flag = true;
		for (auto& d : datas)
		{
			cd.normal += d;
		}
		//���������@���x�N�g���𐳋K��
		cd.normal = cd.normal.Normalize();
	}
	//���U���g�ɕۑ�
	result->push_back(cd);

	return cd.collision_Flag;
}