#include "Collision.h"

void Collision::Get_Triangle_Box3D(std::vector<Triangle>* result, const ML::Box3D& box, const ML::QT& rotation) const
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
		vertex_Index[0],
		vertex_Index[3],
		vertex_Index[4],		
	};
	this->Get_Normal_to_Vector_Cross(&t[0].normal, (t[0].c - t[0].a), (t[0].b - t[0].a));
	t[1] =
	{
		vertex_Index[3],
		vertex_Index[4],
		vertex_Index[7],		
	};
	this->Get_Normal_to_Vector_Cross(&t[1].normal, (t[1].b - t[1].a), (t[1].c - t[1].a));
	t[2] =
	{
		vertex_Index[1],
		vertex_Index[0],
		vertex_Index[5],		
	};
	this->Get_Normal_to_Vector_Cross(&t[2].normal, (t[2].c - t[2].a), (t[2].b - t[2].a));
	t[3] =
	{
		vertex_Index[0],
		vertex_Index[5],
		vertex_Index[4],		
	};
	this->Get_Normal_to_Vector_Cross(&t[3].normal, (t[3].b - t[3].a), (t[3].c - t[3].a));
	t[4] =
	{
		vertex_Index[1],
		vertex_Index[5],
		vertex_Index[2],
		
	};
	this->Get_Normal_to_Vector_Cross(&t[4].normal, (t[4].c - t[4].a), (t[4].b - t[4].a));
	t[5] =
	{
		vertex_Index[5],
		vertex_Index[2],
		vertex_Index[6],
		
	};
	this->Get_Normal_to_Vector_Cross(&t[5].normal, (t[5].b - t[5].a), (t[5].c - t[5].a));
	t[6] =
	{
		vertex_Index[3],
		vertex_Index[7],
		vertex_Index[2],
		
	};
	this->Get_Normal_to_Vector_Cross(&t[6].normal, (t[6].b - t[6].a), (t[4].c - t[6].a));
	t[7] =
	{
		vertex_Index[7],
		vertex_Index[2],
		vertex_Index[6],
		
	};
	this->Get_Normal_to_Vector_Cross(&t[7].normal, (t[7].c - t[7].a), (t[7].b - t[7].a));
	t[8] =
	{
		vertex_Index[4],
		vertex_Index[7],
		vertex_Index[5],
		
	};
	this->Get_Normal_to_Vector_Cross(&t[8].normal, (t[8].c - t[8].a), (t[8].b - t[8].a));
	t[9] =
	{
		vertex_Index[7],
		vertex_Index[5],
		vertex_Index[6],
	};
	this->Get_Normal_to_Vector_Cross(&t[9].normal, (t[9].b - t[9].a), (t[9].c - t[9].a));
	t[10] =
	{
		vertex_Index[0],
		vertex_Index[3],
		vertex_Index[1],
		
	};
	this->Get_Normal_to_Vector_Cross(&t[10].normal, (t[10].b - t[10].a), (t[10].c - t[10].a));
	t[11] =
	{
		vertex_Index[3],
		vertex_Index[1],
		vertex_Index[2],		
	};
	this->Get_Normal_to_Vector_Cross(&t[11].normal, (t[11].c - t[11].a), (t[11].b - t[11].a));

	//std::vector�ł܂Ƃ߂����
	for (int i = 0; i < 12; i++)
	{
		t[i].normal = t[i].normal.Normalize();
		result->push_back(t[i]);
	}
	//�߂�l�ŕԂ�
	//return tri;
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
	if (A.Length() == 0.0f || B.Length() == 0.0f || C.Length() == 0.0f)
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

	this->Vector_Dot(&cosAlpha,A, B);
	this->Vector_Dot(&cosBeta, B, C);
	this->Vector_Dot(&cosGamma, A, C);
	this->Vector_Cross(&sinAlpha, A, B);
	this->Vector_Cross(&sinBeta, B, C);
	this->Vector_Cross(&sinGamma, A, C);

	float check;

	//cos(alpha + beta + gamma)�̓W�J��
	check = (cosGamma*((cosAlpha * cosBeta) - (sinAlpha * sinBeta))) - (sinGamma * ((cosAlpha*sinBeta) + (sinAlpha*cosBeta)));

	//�덷�܂Ŋm�F(�덷�͈͕̔͂ς��]�n������ 2018/03/16)
	//0.99756405026
	if (check  >= _CMATH_::cosf(ML::ToRadian(359)))
	{
		return true;
	}

	return false;
}

void Collision::Get_Poionts_to_Sphere(std::vector<ML::Vec3>* result ,const ML::Vec3& pos, const float& r) const
{
	//std::vector<ML::Vec3> S;
	ML::Vec3 v[106] = {};//ver0.3�ł�6������(2018/05/01)

	//ML::Mat4x4 matR;
	
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
	//for (float i = r; i > r-2; i-=0.5f)
	{
		//ver0.3
		/*v[0] = pos + ML::Vec3(+i, 0, 0);
		v[1] = pos + ML::Vec3(-i, 0, 0);
		v[2] = pos + ML::Vec3(0, +i, 0);
		v[3] = pos + ML::Vec3(0, -i, 0);
		v[4] = pos + ML::Vec3(0, 0, +i);
		v[5] = pos + ML::Vec3(0, 0, -i);*/
		
		float i = r;
		//ver 0.6
		v[0] = pos + ML::Vec3(+i, 0, 0);
		v[1] = pos + ML::Vec3(-i, 0, 0);
		v[2] = pos + ML::Vec3(0, +i, 0);
		v[3] = pos + ML::Vec3(0, -i, 0);
		v[4] = pos + ML::Vec3(0, 0, +i);
		v[5] = pos + ML::Vec3(0, 0, -i);

		v[6] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(15))*i, _CMATH_::sinf(ML::ToRadian(15))*i, 0);
		v[7] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(30))*i, _CMATH_::sinf(ML::ToRadian(30))*i, 0);
		v[8] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(45))*i, _CMATH_::sinf(ML::ToRadian(45))*i, 0);
		v[9] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(60))*i, _CMATH_::sinf(ML::ToRadian(60))*i, 0);
		v[10] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(75))*i, _CMATH_::sinf(ML::ToRadian(75))*i, 0);
		v[11] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(105))*i, _CMATH_::sinf(ML::ToRadian(105))*i, 0);
		v[12] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(120))*i, _CMATH_::sinf(ML::ToRadian(120))*i, 0);
		v[13] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(135))*i, _CMATH_::sinf(ML::ToRadian(135))*i, 0);
		v[14] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(150))*i, _CMATH_::sinf(ML::ToRadian(150))*i, 0);
		v[15] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(165))*i, _CMATH_::sinf(ML::ToRadian(160))*i, 0);
		v[16] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-15))*i, _CMATH_::sinf(ML::ToRadian(-15))*i, 0);
		v[17] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-30))*i, _CMATH_::sinf(ML::ToRadian(-30))*i, 0);
		v[18] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-45))*i, _CMATH_::sinf(ML::ToRadian(-45))*i, 0);
		v[19] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-60))*i, _CMATH_::sinf(ML::ToRadian(-60))*i, 0);
		v[20] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-75))*i, _CMATH_::sinf(ML::ToRadian(-75))*i, 0);
		v[21] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-105))*i, _CMATH_::sinf(ML::ToRadian(-105))*i, 0);
		v[22] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-120))*i, _CMATH_::sinf(ML::ToRadian(-120))*i, 0);
		v[23] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-135))*i, _CMATH_::sinf(ML::ToRadian(-135))*i, 0);
		v[24] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-150))*i, _CMATH_::sinf(ML::ToRadian(-150))*i, 0);
		v[25] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-165))*i, _CMATH_::sinf(ML::ToRadian(-160))*i, 0);

		v[26] = pos + ML::Vec3(0,_CMATH_::cosf(ML::ToRadian(15))*i, _CMATH_::sinf(ML::ToRadian(15))*i);
		v[27] = pos + ML::Vec3(0,_CMATH_::cosf(ML::ToRadian(30))*i, _CMATH_::sinf(ML::ToRadian(30))*i);
		v[28] = pos + ML::Vec3(0,_CMATH_::cosf(ML::ToRadian(45))*i, _CMATH_::sinf(ML::ToRadian(45))*i);
		v[29] = pos + ML::Vec3(0,_CMATH_::cosf(ML::ToRadian(60))*i, _CMATH_::sinf(ML::ToRadian(60))*i);
		v[30] = pos + ML::Vec3(0,_CMATH_::cosf(ML::ToRadian(75))*i, _CMATH_::sinf(ML::ToRadian(75))*i);
		v[31] = pos + ML::Vec3(0,_CMATH_::cosf(ML::ToRadian(105))*i, _CMATH_::sinf(ML::ToRadian(105))*i);
		v[32] = pos + ML::Vec3(0,_CMATH_::cosf(ML::ToRadian(120))*i, _CMATH_::sinf(ML::ToRadian(120))*i);
		v[33] = pos + ML::Vec3(0,_CMATH_::cosf(ML::ToRadian(135))*i, _CMATH_::sinf(ML::ToRadian(135))*i);
		v[34] = pos + ML::Vec3(0,_CMATH_::cosf(ML::ToRadian(150))*i, _CMATH_::sinf(ML::ToRadian(150))*i);
		v[35] = pos + ML::Vec3(0,_CMATH_::cosf(ML::ToRadian(165))*i, _CMATH_::sinf(ML::ToRadian(160))*i);
		v[36] = pos + ML::Vec3(0,_CMATH_::cosf(ML::ToRadian(-15))*i, _CMATH_::sinf(ML::ToRadian(-15))*i);
		v[37] = pos + ML::Vec3(0,_CMATH_::cosf(ML::ToRadian(-30))*i, _CMATH_::sinf(ML::ToRadian(-30))*i);
		v[38] = pos + ML::Vec3(0,_CMATH_::cosf(ML::ToRadian(-45))*i, _CMATH_::sinf(ML::ToRadian(-45))*i);
		v[39] = pos + ML::Vec3(0,_CMATH_::cosf(ML::ToRadian(-60))*i, _CMATH_::sinf(ML::ToRadian(-60))*i);
		v[40] = pos + ML::Vec3(0,_CMATH_::cosf(ML::ToRadian(-75))*i, _CMATH_::sinf(ML::ToRadian(-75))*i);
		v[41] = pos + ML::Vec3(0,_CMATH_::cosf(ML::ToRadian(-105))*i, _CMATH_::sinf(ML::ToRadian(-105))*i);
		v[42] = pos + ML::Vec3(0,_CMATH_::cosf(ML::ToRadian(-120))*i, _CMATH_::sinf(ML::ToRadian(-120))*i);
		v[43] = pos + ML::Vec3(0,_CMATH_::cosf(ML::ToRadian(-135))*i, _CMATH_::sinf(ML::ToRadian(-135))*i);
		v[44] = pos + ML::Vec3(0,_CMATH_::cosf(ML::ToRadian(-150))*i, _CMATH_::sinf(ML::ToRadian(-150))*i);
		v[45] = pos + ML::Vec3(0,_CMATH_::cosf(ML::ToRadian(-165))*i, _CMATH_::sinf(ML::ToRadian(-160))*i);

		v[46] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(15))*i,0, _CMATH_::sinf(ML::ToRadian(15))*i);
		v[47] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(30))*i,0, _CMATH_::sinf(ML::ToRadian(30))*i);
		v[48] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(45))*i,0, _CMATH_::sinf(ML::ToRadian(45))*i);
		v[49] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(60))*i,0, _CMATH_::sinf(ML::ToRadian(60))*i);
		v[50] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(75))*i,0, _CMATH_::sinf(ML::ToRadian(75))*i);
		v[51] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(105))*i,0, _CMATH_::sinf(ML::ToRadian(105))*i);
		v[52] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(120))*i,0, _CMATH_::sinf(ML::ToRadian(120))*i);
		v[53] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(135))*i,0, _CMATH_::sinf(ML::ToRadian(135))*i);
		v[54] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(150))*i,0, _CMATH_::sinf(ML::ToRadian(150))*i);
		v[55] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(165))*i,0, _CMATH_::sinf(ML::ToRadian(160))*i);
		v[56] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-15))*i,0, _CMATH_::sinf(ML::ToRadian(-15))*i);
		v[57] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-30))*i,0, _CMATH_::sinf(ML::ToRadian(-30))*i);
		v[58] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-45))*i,0, _CMATH_::sinf(ML::ToRadian(-45))*i);
		v[59] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-60))*i,0, _CMATH_::sinf(ML::ToRadian(-60))*i);
		v[60] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-75))*i,0, _CMATH_::sinf(ML::ToRadian(-75))*i);
		v[61] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-105))*i,0, _CMATH_::sinf(ML::ToRadian(-105))*i);
		v[62] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-120))*i,0, _CMATH_::sinf(ML::ToRadian(-120))*i);
		v[63] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-135))*i,0, _CMATH_::sinf(ML::ToRadian(-135))*i);
		v[64] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-150))*i,0, _CMATH_::sinf(ML::ToRadian(-150))*i);
		v[65] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-165))*i,0, _CMATH_::sinf(ML::ToRadian(-160))*i);

		v[66] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(15))*i, _CMATH_::sinf(ML::ToRadian(15))*i, _CMATH_::cosf(ML::ToRadian(15))*i);
		v[67] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(30))*i, _CMATH_::sinf(ML::ToRadian(30))*i, _CMATH_::cosf(ML::ToRadian(30))*i);
		v[68] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(45))*i, _CMATH_::sinf(ML::ToRadian(45))*i, _CMATH_::cosf(ML::ToRadian(45))*i);
		v[69] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(60))*i, _CMATH_::sinf(ML::ToRadian(60))*i, _CMATH_::cosf(ML::ToRadian(60))*i);
		v[70] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(75))*i, _CMATH_::sinf(ML::ToRadian(75))*i, _CMATH_::cosf(ML::ToRadian(75))*i);
		v[71] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(105))*i, _CMATH_::sinf(ML::ToRadian(105))*i, _CMATH_::cosf(ML::ToRadian(105))*i);
		v[72] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(120))*i, _CMATH_::sinf(ML::ToRadian(120))*i, _CMATH_::cosf(ML::ToRadian(120))*i);
		v[73] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(135))*i, _CMATH_::sinf(ML::ToRadian(135))*i, _CMATH_::cosf(ML::ToRadian(135))*i);
		v[74] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(150))*i, _CMATH_::sinf(ML::ToRadian(150))*i, _CMATH_::cosf(ML::ToRadian(150))*i);
		v[75] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(165))*i, _CMATH_::sinf(ML::ToRadian(165))*i, _CMATH_::cosf(ML::ToRadian(165))*i);

		v[76] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(15))*i, _CMATH_::sinf(ML::ToRadian(15))*i, _CMATH_::cosf(ML::ToRadian(-15))*i);
		v[77] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(30))*i, _CMATH_::sinf(ML::ToRadian(30))*i, _CMATH_::cosf(ML::ToRadian(-30))*i);
		v[78] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(45))*i, _CMATH_::sinf(ML::ToRadian(45))*i, _CMATH_::cosf(ML::ToRadian(-45))*i);
		v[79] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(60))*i, _CMATH_::sinf(ML::ToRadian(60))*i, _CMATH_::cosf(ML::ToRadian(-60))*i);
		v[80] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(75))*i, _CMATH_::sinf(ML::ToRadian(75))*i, _CMATH_::cosf(ML::ToRadian(-75))*i);
		v[81] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(105))*i, _CMATH_::sinf(ML::ToRadian(105))*i, _CMATH_::cosf(ML::ToRadian(-105))*i);
		v[82] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(120))*i, _CMATH_::sinf(ML::ToRadian(120))*i, _CMATH_::cosf(ML::ToRadian(-120))*i);
		v[83] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(135))*i, _CMATH_::sinf(ML::ToRadian(135))*i, _CMATH_::cosf(ML::ToRadian(-135))*i);
		v[84] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(150))*i, _CMATH_::sinf(ML::ToRadian(150))*i, _CMATH_::cosf(ML::ToRadian(-150))*i);
		v[85] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(165))*i, _CMATH_::sinf(ML::ToRadian(165))*i, _CMATH_::cosf(ML::ToRadian(-165))*i);

		v[86] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-15))*i, _CMATH_::sinf(ML::ToRadian(-15))*i, _CMATH_::cosf(ML::ToRadian(15))*i);
		v[87] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-30))*i, _CMATH_::sinf(ML::ToRadian(-30))*i, _CMATH_::cosf(ML::ToRadian(30))*i);
		v[88] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-45))*i, _CMATH_::sinf(ML::ToRadian(-45))*i, _CMATH_::cosf(ML::ToRadian(45))*i);
		v[89] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-60))*i, _CMATH_::sinf(ML::ToRadian(-60))*i, _CMATH_::cosf(ML::ToRadian(60))*i);
		v[90] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-75))*i, _CMATH_::sinf(ML::ToRadian(-75))*i, _CMATH_::cosf(ML::ToRadian(75))*i);
		v[91] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-105))*i, _CMATH_::sinf(ML::ToRadian(-105))*i, _CMATH_::cosf(ML::ToRadian(105))*i);
		v[92] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-120))*i, _CMATH_::sinf(ML::ToRadian(-120))*i, _CMATH_::cosf(ML::ToRadian(120))*i);
		v[93] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-135))*i, _CMATH_::sinf(ML::ToRadian(-135))*i, _CMATH_::cosf(ML::ToRadian(135))*i);
		v[94] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-150))*i, _CMATH_::sinf(ML::ToRadian(-150))*i, _CMATH_::cosf(ML::ToRadian(150))*i);
		v[95] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-165))*i, _CMATH_::sinf(ML::ToRadian(-165))*i, _CMATH_::cosf(ML::ToRadian(165))*i);

		v[96] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-15))*i, _CMATH_::sinf(ML::ToRadian(-15))*i, _CMATH_::cosf(ML::ToRadian(-15))*i);
		v[97] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-30))*i, _CMATH_::sinf(ML::ToRadian(-30))*i, _CMATH_::cosf(ML::ToRadian(-30))*i);
		v[98] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-45))*i, _CMATH_::sinf(ML::ToRadian(-45))*i, _CMATH_::cosf(ML::ToRadian(-45))*i);
		v[99] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-60))*i, _CMATH_::sinf(ML::ToRadian(-60))*i, _CMATH_::cosf(ML::ToRadian(-60))*i);
		v[100] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-75))*i, _CMATH_::sinf(ML::ToRadian(-75))*i, _CMATH_::cosf(ML::ToRadian(-75))*i);
		v[101] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-105))*i, _CMATH_::sinf(ML::ToRadian(-105))*i, _CMATH_::cosf(ML::ToRadian(-105))*i);
		v[102] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-120))*i, _CMATH_::sinf(ML::ToRadian(-120))*i, _CMATH_::cosf(ML::ToRadian(-120))*i);
		v[103] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-135))*i, _CMATH_::sinf(ML::ToRadian(-135))*i, _CMATH_::cosf(ML::ToRadian(-135))*i);
		v[104] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-150))*i, _CMATH_::sinf(ML::ToRadian(-150))*i, _CMATH_::cosf(ML::ToRadian(-150))*i);
		v[105] = pos + ML::Vec3(_CMATH_::cosf(ML::ToRadian(-165))*i, _CMATH_::sinf(ML::ToRadian(-165))*i, _CMATH_::cosf(ML::ToRadian(-165))*i);
														 
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
bool Collision::Hit_Check(std::vector<After_Collision>* result, const ML::Box3D& box, const ML::Vec3& pos, const float& r, const ML::Vec3& speed, const ML::QT& worldR) const
{
	std::vector<ML::Vec3> all_Points;
	//���̒��_���W
	this->Get_Poionts_to_Sphere(&all_Points, pos, r);
	//�ŒZ�����̍��W���ǉ�
	this->Get_ShortisetPoints_BoxtoSphere(&all_Points, box, pos, r);
	
	//��̃}�X�ɂ���12�̎O�p�`
	std::vector<Triangle> all_Tri;
	this->Get_Triangle_Box3D(&all_Tri, box, worldR);

	bool collision_Flag;	
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
				float cosSN;
				this->Vector_Dot(&cosSN, speed, tri.normal);
				//cos�l��1�Ƃ������Ƃ͓��p��0�x���Ƃ������ƁA�܂蕨���I�ɂ��蓾�Ȃ��Փ�
				//�������̂��߂Ɍ덷�͈͂܂Ŋm�F
				if (cosSN >= _CMATH_::cos(ML::ToRadian(6)))
				{
					//�Ȃ̂Ŕ���͂������Ă��邪��������
					continue;
				}
				//�ȉ����������O�p�`�̖@���x�N�g���ƃt���O��Ԃ�����
				collision_True.collision_Flag = collision_Flag;
				collision_True.normal = tri.normal;
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
void Collision::Get_ShortisetPoints_BoxtoSphere(std::vector<ML::Vec3>* result, const ML::Box3D& box, const ML::Vec3& pos, const float& r) const
{
	//�{�b�N�X�̒��S���m�ۂ���
	ML::Vec3 center_of_Box = ML::Vec3(box.x + box.w / 2, box.y + box.h / 2, box.z + box.d / 2);
	//��̒��S�̑��΋������Z�o
	ML::Vec3 relative_Distance = center_of_Box - pos;

	//���΋�����������������c�����߂ɐ��K������
	relative_Distance = relative_Distance.Normalize();

	
	//�ŒZ�������o������̃x�N�g���ɔ����a���|����
	//�ŒZ�����̓_��Ԃ�
	for (float d = r + 2; d > r - 2; d --)
	{
		result->push_back(pos + (relative_Distance * d));
	}

	
}