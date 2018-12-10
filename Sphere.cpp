#include "Sphere.h"
#include "Collision.h"

void Sphere::Get_Triangle_Box3D(std::vector<Triangle>* r)const
{
	//���ݏ����Ȃ��ŏI��(2018/12/10)
	return;
}

bool Sphere::Hit(std::vector<Collision_Data>* result, Shape3D* other,const bool unsuable_Triangle[])
{
	//���Ƃ̂����蔻�肾����z�肵�Ă���̂ŁA���̈ȊO�������画�肵�Ȃ�
	if (other->type != Shape3D_Type::Type_Cube)
	{
		return false;
	}

	//���ƃL���[�u���t��]���āA�ŒZ�����̓_�����
	ML::Vec3 nearest_Point;
	if (Collision::Intersect_AABB_Sphere(&nearest_Point, other, this) == false)
	{
		return false;
	}

	//����
	return Collision::Check_Collision_Cube_Sphere(result, other, nearest_Point, unsuable_Triangle);
}

Sphere::Sphere(const ML::Vec3& center, const ML::Vec3& length_H, const ML::QT& qt)
	:Shape3D(center,length_H,qt)
{
	this->type = Shape3D_Type::Type_Sphere;
}