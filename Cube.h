#pragma warning(disable:4996)
#pragma once
#include "Shape.h"

//3次元六面体クラス

class Cube : public Shape3D
{
public:
	//ポリゴン取り出し
	virtual void Get_Triangle_Box3D(std::vector<Triangle>* result) const override;
	//あたり判定
	virtual bool Hit(std::vector<Collision_Data>* result, Shape3D* other) override;

	//コンストラクタ
	Cube(const ML::Vec3& center, const ML::Vec3& length_H, const ML::QT& qt);
};