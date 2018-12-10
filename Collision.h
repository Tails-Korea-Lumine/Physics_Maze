#pragma warning(disable:4996)
#pragma once
#include "myLib.h"
#include "MyMath.h"
#include "Shape.h"

namespace Collision
{
	//�����蔻���l
	static const float judge = _CMATH_::cosf(ML::ToRadian(359));	

	//��̓_�ƎO�p�`�̏Փ˔�����s��
	bool Check_Collision_Triangle_Point(const Triangle& tri, const ML::Vec3& p);

	//OBB��AABB�ɖ߂��Ĕ���
	bool Intersect_AABB_Sphere(ML::Vec3* result, const Shape3D* owner, const Shape3D* visitor);
	

	//���̂ƘZ�ʑ̏Փ˔���֐�
	//�����F(���ʂ�ۑ�����std::vector�̃A�h���X�A3�����}�`�̔��肷�鑤�A����鑤�A��O�̃����_��)
	bool Check_Collision_Cube_Sphere(std::vector<Collision_Data>* result, const Shape3D* cube, const ML::Vec3& nearest_point , const bool unsuable_Triangle[]);
	
};

//static Collision col = Collision();