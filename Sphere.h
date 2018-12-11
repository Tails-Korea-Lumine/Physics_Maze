#pragma warning(disable:4996)
#pragma once
#include "Shape.h"

//球のクラス

class Sphere : public Shape3D
{
public:
	//ポリゴン取り出し現在処理なし(2018/12/10)
	virtual void Get_Triangle_Box3D(std::vector<Triangle>* result) const override;
	//あたり判定
	virtual bool Hit(std::vector<Collision_Data>* result, Shape3D* other) override;

	//コンストラクタ
	Sphere(const ML::Vec3& center, const ML::Vec3& length_H, const ML::QT& qt);
};