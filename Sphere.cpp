#include "Sphere.h"
#include "Collision.h"

void Sphere::Get_Triangle_Box3D(std::vector<Triangle>* r)const
{
	//現在処理なしで終了(2018/12/10)
	return;
}

bool Sphere::Hit(std::vector<Collision_Data>* result, Shape3D* other)
{
	//球とのあたり判定だけを想定しているので、その以外だったら判定しない
	if (other->type != Shape3D_Type::Type_Cube)
	{
		return false;
	}

	//球とキューブを逆回転して、最短距離の点を取る
	ML::Vec3 nearest_Point;
	Collision::Intersect_OBB_Sphere(&nearest_Point, other, this);

	//判定
	return Collision::Check_Collision_Cube_Sphere(result, other, nearest_Point);
}

Sphere::Sphere(const ML::Vec3& center, const ML::Vec3& length_H, const ML::QT& qt)
	:Shape3D(center,length_H,qt)
{
	this->type = Shape3D_Type::Type_Sphere;
}