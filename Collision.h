#pragma warning(disable:4996)
#pragma once
#include "myLib.h"
#include "MyMath.h"
#include "Shape.h"

class Collision
{
private:
	//��̓_�ƎO�p�`�̏Փ˔�����s��
	static bool Check_Collision_Triangle_Point(const Triangle& tri, const ML::Vec3& p);
	//OBB��AABB�ɖ߂��Ĉ�ԋ߂��_���Z�o���鏈��
	static void Intersect_OBB_Sphere(ML::Vec3* result, const Shape3D* OBB, const Shape3D* Sphere);	
public:
	//���̂ƘZ�ʑ̏Փ˔���֐�
	//�����F(���ʂ�ۑ�����std::vector�̃A�h���X�A3�����}�`�̔��肷�鑤�A����鑤)
	static bool Check_Collision_Cube_Sphere(std::vector<Collision_Data>* result, const Shape3D* cube, const Shape3D* Sphere);
	
};