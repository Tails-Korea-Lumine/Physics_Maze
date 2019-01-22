#include "Sphere.h"
#include "Collision.h"

void Sphere::Get_Triangle_Box3D(std::vector<Triangle>* r)const
{
	//���ݏ����Ȃ��ŏI��(2018/12/10)
	return;
}

bool Sphere::Hit(std::vector<Collision_Data>* result, Shape3D* other)
{
	//���Ƃ̂����蔻�肾����z�肵�Ă���̂ŁA���̈ȊO�������画�肵�Ȃ�
	if (other->type != Shape3D_Type::Type_Cube)
	{
		return false;
	}

	//����
	return Collision::Check_Collision_Cube_Sphere(result, other, this);
}

Sphere::Sphere(const ML::Vec3& center, const ML::Vec3& length_H, const ML::QT& qt)
	:Shape3D(center,length_H,qt, Shape3D_Type::Type_Sphere)
{
	
}