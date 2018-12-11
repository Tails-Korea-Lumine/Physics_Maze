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

	//OBB��AABB�ɖ߂��Ĉ�ԋ߂��_���Z�o���鏈��
	void Intersect_OBB_Sphere(ML::Vec3* result, const Shape3D* OBB, const Shape3D* Sphere);
	

	//���̂ƘZ�ʑ̏Փ˔���֐�
	//�����F(���ʂ�ۑ�����std::vector�̃A�h���X�A3�����}�`�̔��肷�鑤�A����鑤)
	bool Check_Collision_Cube_Sphere(std::vector<Collision_Data>* result, const Shape3D* cube, const ML::Vec3& nearest_point);
	
};

//static Collision col = Collision();