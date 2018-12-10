#include "Shape.h"

//�R���X�g���N�^
Shape3D::Shape3D(const ML::Vec3& center, const ML::Vec3& length_H, const ML::QT& qt)
{
	this->center = center;
	this->half_of_Length = length_H;
	this->quaternion = qt;
	this->type = Shape3D_Type::Type_Clear;
}


//�Q�b�^�[
//----------------------------------------------------------------------------------
//���S�_
ML::Vec3 Shape3D::Get_Center() const
{
	return this->center;
}
//��������
ML::Vec3 Shape3D::Get_Length() const
{
	return this->half_of_Length;
}
//��]��
ML::QT Shape3D::Get_Quaternion() const
{
	return this->quaternion;
}

//------------------------------------------------------------------------------------

//�ړ�
void Shape3D::Offset(const ML::Vec3& vec)
{
	this->center += vec;
}

//�ʒu�̏㏑��
void Shape3D::Set_Position(const ML::Vec3& pos)
{
	this->center = pos;
}

//��]
void Shape3D::Rotation(ML::Mat4x4* mat, const ML::QT& qt)
{
	this->center = mat->TransformCoord(this->center);
	this->quaternion *= qt;
}