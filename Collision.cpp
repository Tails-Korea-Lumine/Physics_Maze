#include "Collision.h"
#include "Sphere.h"
#include "Cube.h"

#define TRIANGLE_ON_CUBE 12


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
bool Collision::Intersect_AABB_Sphere(ML::Vec3* result, const Shape3D* owner, const Shape3D* visitor)
{
	//���܂ł̉�]�ʂŋt�s������
	ML::Mat4x4 matIR;

	D3DXMatrixAffineTransformation(&matIR, 1.0f, &owner->Get_Center(), &owner->Get_Quaternion(), NULL);
	matIR = matIR.Inverse();


	//�t�s��ɉ����ĉ�]�������̒��S�_
	ML::Vec3 alined_Sphere_Center = matIR.TransformCoord(visitor->Get_Center());

	//��ԋ߂��_���v�Z
	float x = max(owner->center.x - owner->half_of_Length.x, min(alined_Sphere_Center.x, owner->center.x + owner->half_of_Length.x));
	float y = max(owner->center.y - owner->half_of_Length.y, min(alined_Sphere_Center.y, owner->center.y + owner->half_of_Length.y));
	float z = max(owner->center.z - owner->half_of_Length.z, min(alined_Sphere_Center.z, owner->center.z + owner->half_of_Length.z));

	//*result = ML::Vec3(x, y, z);

	//�����ɗ���Ă���Ȃ�false��Ԃ�
	ML::Vec3 dist = ML::Vec3(x, y, z) - visitor->Get_Center();

	*result = dist.Length() < visitor->Get_Length().x ? ML::Vec3(x, y, z) : visitor->Get_Center() + dist.Normalize() * visitor->Get_Length().x;
	return true;
}


//�}�X�ʂɌĂ΂��֐�
bool Collision::Check_Collision_Cube_Sphere(std::vector<Collision_Data>* result, const Shape3D* owner, const ML::Vec3& nearest_point, const bool unsuable_Triangle[])
{		
	//�R���X�g���N�^�ɂ���ă[���x�N�g����false�Ő��������
	Collision_Data collision_True;


	//�ȑO�ɔ��f�������Ƃɑ΂��ď�����ʂɂ���
	//�ȑO�̔���ŏՓ˂��N����\�����������̂Ő������������
	
		////�Փ˔���X�^�[�g
		//for (const auto& tri : all_Tri)
		//{			
		//	//�}�X�ƃ}�X�ڐG�ʂł�������������h��
		//	//�ړ��x�N�g���ƏՓ˂����O�p�`�̖@���x�N�g����cos�l
		//	float cosSN;
		//	MyMath::Vector_Dot(&cosSN, speed, tri.normal);
		//	//cos�l��1�Ƃ������Ƃ͓��p��0�x���Ƃ������ƁA�܂蕨���I�ɂ��蓾�Ȃ��Փ�
		//	//�������̂��߂Ɍ덷�͈͂܂Ŋm�F
		//	if (cosSN >= judge)
		//	{
		//		//�Ȃ̂Ŗ�������
		//		continue;
		//	}

		//	collision_True = After_Collision();
		//	for (const auto& p : all_Points)
		//	{
		//		//����ȊO�̏ꍇ�ł����蔻����s��
		//		if (Collision::Check_Collision(tri, p))
		//		{
		//			//�ȉ����������O�p�`�̖@���x�N�g���ƃt���O��Ԃ�����
		//			collision_True.collision_Flag = true;
		//			collision_True.normal = tri.normal;
		//			//�����o�^����Ă�����̂Ȃ̂��m�F����
		//			if (result->size() == 0 || collision_True != result->at(result->size() - 1))
		//			{
		//				result->push_back(collision_True);
		//			}
		//			//�|���S��1������1�̓_�̏Փ˂��N�����炻��Ŏ��̃|���S���̔��������
		//			break;
		//		}
		//	}
		//}

	//����p�̎O�p�`�����o��
	std::vector<Triangle> all_Tri;
	owner->Get_Triangle_Box3D(&all_Tri);

	for (int i = 0; i < TRIANGLE_ON_CUBE; i++)
	{
		//��O�łȂ����O�p�`�Ɠ_�̂����蔻�肪�����Ă�ꍇ�ŕۑ�����
		//unsuable_Triangle[i] == false &&
		if ( Check_Collision_Triangle_Point(all_Tri[i], nearest_point))
		{
			//�ȉ����������O�p�`�̖@���x�N�g���ƃt���O��Ԃ�����
			collision_True.collision_Flag = true;
			collision_True.normal = all_Tri[i].normal;
			//�����o�^����Ă�����̂Ȃ̂��m�F����
			if (result->size() == 0 || collision_True != result->at(result->size() - 1))
			{
				result->push_back(collision_True);
			}			
		}
	}
		
	
	//
	////��������͌��݈ʒu�ōs���Đ�������t���O�͈ړ������Ɨ\�z�����Ƃ���Ŕ��������
	//ML::Vec3 next_Pos = ball_Pos + speed;
	////���̈ʒu���{�b�N�X�̑Ίp���𔼒��a�Ƃ��鋅�Ƃ������Ă���Ɛ�������t���O�𗧂Ă�
	//this->pricision_Flag = ML::Vec3(next_Pos - box_Center).Length() <= r + abs( ROOT3 / 2.0f*(box_Length));
	
	return collision_True.collision_Flag;
}

