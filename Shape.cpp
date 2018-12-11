#include "Shape.h"

//コンストラクタ
Shape3D::Shape3D(const ML::Vec3& center, const ML::Vec3& length_H, const ML::QT& qt)
{
	this->center = center;
	this->half_of_Length = length_H;
	this->quaternion = qt;
	this->type = Shape3D_Type::Type_Clear;
	for (size_t i = 0; i < 12; i++)
	{
		this->unusable_Triangle[i] = true;
	}
}


//ゲッター
//----------------------------------------------------------------------------------
//中心点
ML::Vec3 Shape3D::Get_Center() const
{
	return this->center;
}
//長さ成分
ML::Vec3 Shape3D::Get_Length() const
{
	return this->half_of_Length;
}
//回転量
ML::QT Shape3D::Get_Quaternion() const
{
	return this->quaternion;
}

//------------------------------------------------------------------------------------

//移動
void Shape3D::Offset(const ML::Vec3& vec)
{
	this->center += vec;
}

//位置の上書き
void Shape3D::Set_Position(const ML::Vec3& pos)
{
	this->center = pos;
}

//回転
void Shape3D::Rotation(ML::Mat4x4* mat, const ML::QT& qt)
{
	this->center = mat->TransformCoord(this->center);
	this->quaternion *= qt;
}

//判定しないポリゴンにマーキングしておく処理
void Shape3D::Marking_On_Unsuable_Poligon(const size_t& i)
{
	this->unusable_Triangle[i] = false;
}