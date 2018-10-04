#include "Collision.h"
#include "MyMath.h"

#define TRIANGLE_ON_CUBE 12
#define VERTEX_ON_CUBE 8
#define ROOT3 1.732050f

void Collision::Get_Triangle_Box3D(std::vector<Triangle>* result, const ML::Box3D& box, const ML::QT& rotation) const
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

bool Collision::Check_Collision(const Triangle& tri, const ML::Vec3& p) const
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
	if (check  >= this->judge)
	{
		return true;
	}

	return false;
}

void Collision::Get_Poionts_to_Sphere(std::vector<ML::Vec3>* result ,const ML::Vec3& pos, const float& r) const
{
	const int increasing_Dgree = 10;
	//���̏�ɂ���_��S�����o������

	//�ŏ��ɉ�]������_���v�Z
	std::vector<ML::Vec3> points;
	ML::Vec3 y = pos + ML::Vec3(0, r, 0);

	//�c�����ɐ؂����f�ʂ̕Е�����鏈��
	for (int i = 0; i < 180; i += increasing_Dgree)
	{
		//��]�s�񐶐�
		ML::Mat4x4 matRx;
		ML::QT qtX = ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian((float)i));

		D3DXMatrixAffineTransformation(&matRx, 1.0f, &pos, &qtX, NULL);

		points.push_back(matRx.TransformCoord(y));
	}
	
	//���o�����_����]���Ȃ��猋�ʕۑ��p���F�N�^�[�Ƀv�b�V���o�b�N
	for (int d = 0; d < 360; d += increasing_Dgree)
	{
		//��]�s�񐶐�
		ML::Mat4x4 matRy;
		ML::QT qtY = ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian((float)d));

		D3DXMatrixAffineTransformation(&matRy, 1.0f, &pos, &qtY, NULL);
		
		for (auto& p : points)
		{
			result->push_back(matRy.TransformCoord(p));
		}
	}
}

//�}�X�ʂɌĂ΂��֐�
bool Collision::Hit_Check(std::vector<After_Collision>* result, const ML::Box3D& box, const ML::Vec3& pos, const float& r, const ML::Vec3& speed, const ML::QT& Rotation)
{	
	//��̃}�X�ɂ���12�̎O�p�`
	std::vector<Triangle> all_Tri;
	this->Get_Triangle_Box3D(&all_Tri, box, Rotation);

	std::vector<ML::Vec3> all_Points;
	//�ŒZ�����̍��W���ǉ�
	this->Get_ShortisetPoints_Box_to_Sphere(&all_Points, box);	
	//���̒��_���W
	this->Get_Poionts_to_Sphere(&all_Points, pos, r);
	//�R���X�g���N�^�ɂ���ă[���x�N�g����false�Ő��������
	After_Collision collision_True;

	//�ȑO�ɔ��f�������Ƃɑ΂��ď�����ʂɂ���
	//�ȑO�̔���ŏՓ˂��N����\�����������̂Ő������������
	if (this->pricision_Flag)
	{
		//�Փ˔���X�^�[�g
		for (const auto& tri : all_Tri)
		{
			collision_True = After_Collision();
			for (const auto& p : all_Points)
			{
				//�}�X�ƃ}�X�ڐG�ʂł�������������h��
				//�ړ��x�N�g���ƏՓ˂����O�p�`�̖@���x�N�g����cos�l
				float cosSN;
				MyMath::Vector_Dot(&cosSN, speed, tri.normal);
				//cos�l��1�Ƃ������Ƃ͓��p��0�x���Ƃ������ƁA�܂蕨���I�ɂ��蓾�Ȃ��Փ�
				//�������̂��߂Ɍ덷�͈͂܂Ŋm�F
				if (cosSN >= this->judge)
				{
					//�Ȃ̂Ŗ�������
					continue;
				}

				//����ȊO�̏ꍇ�ł����蔻����s��
				if (this->Check_Collision(tri, p))
				{
					//�ȉ����������O�p�`�̖@���x�N�g���ƃt���O��Ԃ�����
					collision_True.collision_Flag = true;
					collision_True.normal = tri.normal;
					result->push_back(collision_True);
					//�|���S��1������1�̓_�̏Փ˂��N�����炻��Ŏ��̃|���S���̔��������
					break;
				}
			}
		}
	}	
	
	//��������͌��݈ʒu�ōs���Đ�������t���O�͈ړ������Ɨ\�z�����Ƃ���Ŕ��������
	ML::Vec3 next_Pos = pos + speed;
	ML::Vec3 box_Center = { box.x + (box.w / 2), box.y + (box.h / 2), box.z + (box.d / 2) };
	//���̈ʒu���{�b�N�X�̑Ίp���𔼒��a�Ƃ��鋅�Ƃ������Ă���Ɛ�������t���O�𗧂Ă�
	ML::Vec3(next_Pos - box_Center).Length() <= r + ROOT3 / 2.0f*(box.x) ? this->pricision_Flag = true : this->pricision_Flag = false;	
	
	
	if (collision_True.collision_Flag)
	{
		return true;
	}
	//������Ȃ�������false��Ԃ�	
	return false;
}

//Box3d�Ƌ��̂̍ŒZ�����̓_�����
void Collision::Get_ShortisetPoints_Box_to_Sphere(std::vector<ML::Vec3>* result, const ML::Box3D& box) const
{
	//ver1.0
	////�{�b�N�X�̒��S���m�ۂ���
	//ML::Vec3 center_of_Box = ML::Vec3(box.x + box.w / 2, box.y + box.h / 2, box.z + box.d / 2);
	////��̒��S�̑��΋������Z�o
	//ML::Vec3 relative_Distance = center_of_Box - pos;

	////���΋�����������������c�����߂ɐ��K������
	//relative_Distance = relative_Distance.Normalize();

	//
	////�ŒZ�������o������̃x�N�g���ɔ����a���|����
	////�ŒZ�����̓_��Ԃ�
	//for (float d = r + 2; d > r - 2; d --)
	//{
	//	result->push_back(pos + (relative_Distance * d));
	//}

	

	//ver2.0
	//�����ł�������{�b�N�X�ɊO�ڂ��鋅�ɓ����_�������c��	
	//�O�ڂ��鋅�̔����a
	float outer_Sphere_R = ROOT3 / 2.0f*(box.x);
	//�O�ڂ��鋅�̒��S�_
	ML::Vec3 box_Center = { box.x + (box.w / 2), box.y + (box.h / 2), box.z + (box.d / 2) };

	result->erase(remove_if(result->begin(), result->end(),
							[&](ML::Vec3 p)->bool{return ML::Vec3(p - box_Center).Length() <= outer_Sphere_R;}
						),
		result->end()
	);
	
}