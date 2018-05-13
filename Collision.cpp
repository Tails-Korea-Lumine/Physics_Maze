#include "Collision.h"

void Collision::Get_Triangle_Box3D(std::vector<Triangle>* result, const ML::Box3D& box, const ML::QT& rotation)
{
	//�O�p�`���Ƃ��ɕۑ�����ꏊ
	//std::vector<Triangle> tri;

	//6�ʑ̂ɂ�12�̎O�p�`������
	Triangle t[12] = {};
	
	//�e���_�������C���f�b�N�X
	ML::Vec3 vertex_Index[8] =
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
	
	for (int i = 0; i < 8; i++)
	{
		vertex_Index[i] = matR.TransformCoord(vertex_Index[i]);
	}


	//�O�p�`�̏�����
	t[0] =
	{
		t[0].a = vertex_Index[0],
		t[0].b = vertex_Index[3],
		t[0].c = vertex_Index[4],
		t[0].normal = this->Get_Normal_to_Vector_Cross((t[0].c - t[0].a), (t[0].b - t[0].a))
	};
	t[1] =
	{
		t[1].a = vertex_Index[3],
		t[1].b = vertex_Index[4],
		t[1].c = vertex_Index[7],
		t[1].normal = this->Get_Normal_to_Vector_Cross((t[1].b - t[1].a), (t[1].c - t[1].a))
	};
	t[2] =
	{
		t[2].a = vertex_Index[1],
		t[2].b = vertex_Index[0],		
		t[2].c = vertex_Index[5],
		t[2].normal = this->Get_Normal_to_Vector_Cross((t[2].c - t[2].a), (t[2].b - t[2].a))
	};
	t[3] =
	{
		t[3].a = vertex_Index[0],
		t[3].b = vertex_Index[5],
		t[3].c = vertex_Index[4],
		t[3].normal = this->Get_Normal_to_Vector_Cross((t[3].b - t[3].a), (t[3].c - t[3].a))
	};
	t[4] =
	{
		t[4].a = vertex_Index[1],
		t[4].b = vertex_Index[5],
		t[4].c = vertex_Index[2],
		t[4].normal = this->Get_Normal_to_Vector_Cross((t[4].c - t[4].a), (t[4].b - t[4].a))
	};
	t[5] =
	{
		t[5].a = vertex_Index[5],
		t[5].b = vertex_Index[2],
		t[5].c = vertex_Index[6],
		t[5].normal = this->Get_Normal_to_Vector_Cross((t[5].b - t[5].a), (t[5].c - t[5].a))
	};
	t[6] =
	{
		t[6].a = vertex_Index[3],
		t[6].b = vertex_Index[7],
		t[6].c = vertex_Index[2],
		t[6].normal = this->Get_Normal_to_Vector_Cross((t[6].b - t[6].a),(t[4].c - t[6].a))
	};
	t[7] =
	{
		t[7].a = vertex_Index[7],
		t[7].b = vertex_Index[2],
		t[7].c = vertex_Index[6],
		t[7].normal = this->Get_Normal_to_Vector_Cross((t[7].c - t[7].a), (t[7].b - t[7].a))
	};
	t[8] =
	{
		t[8].a = vertex_Index[4],
		t[8].b = vertex_Index[7],
		t[8].c = vertex_Index[5],
		t[8].normal = this->Get_Normal_to_Vector_Cross((t[8].c - t[8].a), (t[8].b - t[8].a))
	};
	t[9] =
	{
		t[9].a = vertex_Index[7],
		t[9].b = vertex_Index[5],
		t[9].c = vertex_Index[6],
		t[9].normal = this->Get_Normal_to_Vector_Cross((t[9].b - t[9].a),(t[9].c - t[9].a))

	};
	t[10] =
	{
		t[10].a = vertex_Index[0],
		t[10].b = vertex_Index[3],
		t[10].c = vertex_Index[1],
		t[10].normal = this->Get_Normal_to_Vector_Cross((t[10].b - t[10].a),(t[10].c - t[10].a))
	};
	t[11] =
	{
		t[11].a = vertex_Index[3],
		t[11].b = vertex_Index[1],
		t[11].c = vertex_Index[2],
		t[11].normal = this->Get_Normal_to_Vector_Cross((t[11].c - t[11].a), (t[11].b - t[11].a))
	};

	//std::vector�ł܂Ƃ߂����
	for (int i = 0; i < 12; i++)
	{
		t[i].normal = t[i].normal.Normalize();
		result->push_back(t[i]);
	}
	//�߂�l�ŕԂ�
	//return tri;
}

bool Collision::Check_Collision(const Triangle& tri, const ML::Vec3& p)
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
	if (A.Length() == 0.0f || B.Length() == 0.0f || C.Length() == 0.0f)
	{
		return true; //�v�Z�s�\���O�p�`�ɓ����Ă���̂ł������Ă锻��ŕԂ�
	}
	//�ӂ̏�̎�
	
	

	//alpha �� A��B�̊p�x
	//beta = B��C�̊p�x
	//gamma = A��C�̊p�x
	float cosAlpha;
	float cosBeta;
	float cosGamma;
	float sinAlpha;
	float sinBeta;
	float sinGamma;

	cosAlpha = this->Vector_Dot(A, B);
	cosBeta = this->Vector_Dot(B, C);
	cosGamma = this->Vector_Dot(A, C);
	sinAlpha = this->Vector_Cross(A, B);
	sinBeta = this->Vector_Cross(B, C);
	sinGamma = this->Vector_Cross(A, C);

	float check;

	//cos(alpha + beta + gamma)�̓W�J��
	check = (cosGamma*((cosAlpha * cosBeta) - (sinAlpha * sinBeta))) - (sinGamma * ((cosAlpha*sinBeta) + (sinAlpha*cosBeta)));

	//�덷�܂Ŋm�F(�덷�͈͕̔͂ς��]�n������ 2018/03/16)
	//0.99756405026
	if (check  >= _CMATH_::cos(ML::ToRadian(359)))
	{
		return true;
	}

	return false;
}

void Collision::Get_Poionts_to_Sphere(std::vector<ML::Vec3>* result ,const ML::Vec3& pos, const float& r, const ML::QT& rotation)
{
	//std::vector<ML::Vec3> S;
	ML::Vec3 v[26] = {};//ver0.3�ł�6������(2018/05/01)

	ML::Mat4x4 matR;
	
	//D3DXMatrixAffineTransformation(&matR, 1, &pos, &rotation, NULL);

	//6�������Ƃ�ver.1
	/*v[0] = pos + ML::Vec3(+r, 0, 0);
	v[1] = pos + ML::Vec3(-r, 0, 0);
	v[2] = pos + ML::Vec3(0, +r, 0);
	v[3] = pos + ML::Vec3(0, -r, 0);
	v[4] = pos + ML::Vec3(0, 0, +r);
	v[5] = pos + ML::Vec3(0, 0, -r);

	for (int i = 0; i < 6; i++)
	{
		v[i] = matR.TransformCoord(v[i]);
		S.push_back(v[i]);
	}*/
	//�����a�̌덷�͈͂��܂߂Ď��ver.2
	for (float i = r; i > r-3; i-=0.5f)
	{
		//ver0.3
		/*v[0] = pos + ML::Vec3(+i, 0, 0);
		v[1] = pos + ML::Vec3(-i, 0, 0);
		v[2] = pos + ML::Vec3(0, +i, 0);
		v[3] = pos + ML::Vec3(0, -i, 0);
		v[4] = pos + ML::Vec3(0, 0, +i);
		v[5] = pos + ML::Vec3(0, 0, -i);*/
		

		//ver 0.4
		v[0] = pos + ML::Vec3(+i, 0, 0);
		v[1] = pos + ML::Vec3(-i, 0, 0);
		v[2] = pos + ML::Vec3(0, +i, 0);
		v[3] = pos + ML::Vec3(0, -i, 0);
		v[4] = pos + ML::Vec3(0, 0, +i);
		v[5] = pos + ML::Vec3(0, 0, -i);
		v[6] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(45))*i, _CMATH_::sinf(ML::ToRadian(45))*i, 0);
		v[7] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(135))*i, _CMATH_::sinf(ML::ToRadian(135))*i, 0);
		v[8] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-45))*i, _CMATH_::sinf(ML::ToRadian(-45))*i, 0);
		v[9] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-135))*i, _CMATH_::sinf(ML::ToRadian(-135))*i, 0);
		v[10] = pos + ML::Vec3(0,_CMATH_::cosf(ML::ToRadian(45))*i, _CMATH_::sinf(ML::ToRadian(45))*i);
		v[11] = pos + ML::Vec3(0,_CMATH_::cosf(ML::ToRadian(135))*i, _CMATH_::sinf(ML::ToRadian(135))*i);
		v[12] = pos + ML::Vec3(0, _CMATH_::cosf(ML::ToRadian(-45))*i, _CMATH_::sinf(ML::ToRadian(-45))*i);
		v[13] = pos + ML::Vec3(0, _CMATH_::cosf(ML::ToRadian(-135))*i, _CMATH_::sinf(ML::ToRadian(-135))*i);
		v[14] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(45))*i,0, _CMATH_::sinf(ML::ToRadian(45))*i);
		v[15] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(135))*i, 0, _CMATH_::sinf(ML::ToRadian(135))*i);
		v[16] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-45))*i, 0, _CMATH_::sinf(ML::ToRadian(-45))*i);
		v[17] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-135))*i, 0, _CMATH_::sinf(ML::ToRadian(-135))*i);
		v[18] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(45))*i, _CMATH_::sinf(ML::ToRadian(45))*i, _CMATH_::cosf(ML::ToRadian(45))*i);
		v[19] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(135))*i, _CMATH_::sinf(ML::ToRadian(135))*i, _CMATH_::cosf(ML::ToRadian(135))*i);
		v[20] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(45))*i, _CMATH_::sinf(ML::ToRadian(45))*i, _CMATH_::cosf(ML::ToRadian(-45))*i);
		v[21] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(135))*i, _CMATH_::sinf(ML::ToRadian(135))*i, _CMATH_::cosf(ML::ToRadian(-135))*i);
		v[22] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-45))*i, _CMATH_::sinf(ML::ToRadian(-45))*i, _CMATH_::cosf(ML::ToRadian(45))*i);
		v[23] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-135))*i, _CMATH_::sinf(ML::ToRadian(-135))*i, _CMATH_::cosf(ML::ToRadian(135))*i);
		v[24] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-45))*i, _CMATH_::sinf(ML::ToRadian(-45))*i, _CMATH_::cosf(ML::ToRadian(-45))*i);
		v[25] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-135))*i, _CMATH_::sinf(ML::ToRadian(-135))*i, _CMATH_::cosf(ML::ToRadian(-135))*i);
	
		for (auto j : v)
		{			
			//j = matR.TransformCoord(j);
			//result.push_back(j);
			result->push_back(j);
		}
	

	}
	//return S;
}

//�}�X�ʂɌĂ΂��֐�
bool Collision::Hit_Check(std::vector<After_Collision>* result, const ML::Box3D& box, const ML::Vec3& pos, const float& r, const ML::Vec3& speed, const ML::QT& worldR)
{
	std::vector<ML::Vec3> all_Points;
	//���̒��_���W
	this->Get_Poionts_to_Sphere(&all_Points, pos, r, worldR);
	//�ŒZ�����̍��W���ǉ�
	this->Get_ShortisetPoints_BoxtoSphere(&all_Points, box, pos, r);
	
	//��̃}�X�ɂ���12�̎O�p�`
	std::vector<Triangle> all_Tri;
	this->Get_Triangle_Box3D(&all_Tri, box, worldR);

	bool collision_Flag;
	//�߂�l
	//std::vector<After_Collision> R;
	//�R���X�g���N�^�ɂ���ă[���x�N�g����false�Ő��������
	After_Collision collision_True;
	//�Փ˔���X�^�[�g
	for (const auto& tri : all_Tri)
	{
		collision_True = After_Collision();
		for (const auto& p : all_Points)
		{
			if (collision_Flag = this->Check_Collision(tri, p))
			{
				//�}�X�ƃ}�X�ڐG�ʂł�������������h��
				//�ړ��x�N�g���ƏՓ˂����O�p�`�̖@���x�N�g����cos�l
				float cosSN = Gravity::Vector_Dot(speed , tri.normal);
				//cos�l��1�Ƃ������Ƃ͓��p��0�x���Ƃ������ƁA�܂蕨���I�ɂ��蓾�Ȃ��Փ�
				//�������̂��߂Ɍ덷�͈͂܂Ŋm�F
				if (cosSN >= _CMATH_::cos(ML::ToRadian(6)))
				{
					//�Ȃ̂Ŕ���͂������Ă��邪��������
					continue;
				}
				//�ȉ����������O�p�`�̖@���x�N�g���ƃt���O��Ԃ�����
				collision_True.collision_Flag = collision_Flag;
				collision_True.normal = tri.normal.Normalize();
				result->push_back(collision_True);
				//�|���S��1������1�̓_�̏Փ˂��N�����炻��Ŏ��̃|���S���̔��������
				break;
			}
		}
	}
	if (collision_True.collision_Flag)
	{
		return true;
	}
	//������Ȃ�������false��Ԃ�	
	return false;
}

//Box3d�Ƌ��̂̍ŒZ�����̓_�����
void Collision::Get_ShortisetPoints_BoxtoSphere(std::vector<ML::Vec3>* result, const ML::Box3D& box, const ML::Vec3& pos, const float& r)
{
	//�{�b�N�X�̒��S���m�ۂ���
	ML::Vec3 center_of_Box = ML::Vec3(box.x + box.w / 2, box.y + box.h / 2, box.z + box.d / 2);
	//��̒��S�̑��΋������Z�o
	ML::Vec3 relative_Distance = center_of_Box - pos;

	//���΋�����������������c�����߂ɐ��K������
	relative_Distance = relative_Distance.Normalize();

	
	//�ŒZ�������o������̃x�N�g���ɔ����a���|����
	//�ŒZ�����̓_��Ԃ�(for���͌덷�͈͍��͑��߂ɓ_����� 2018/05/09)
	for (float d = r; d > r /2; d --)
	{
		result->push_back(pos + (relative_Distance * d));
	}

	//return P;
}