#include "MyMath.h"

//�@���x�N�g�������߂�
void MyMath::Get_Normal_to_Vector_Cross(ML::Vec3* normal, const ML::Vec3& v1, const ML::Vec3& v2)
{
	ML::Vec3 Normal;
	Normal.x = (v1.y*v2.z) - (v1.z*v2.y);
	Normal.y = (v1.z * v2.x) - (v1.x*v2.z);
	Normal.z = (v1.x*v2.y) - (v1.y*v2.x);

	*normal = Normal;
}
//���ς����߂�
float MyMath::Get_Vector_Dot(const ML::Vec3& v1, const ML::Vec3& v2)
{
	float Dot_to_Position;
	Dot_to_Position = (v1.x*v2.x) + (v1.y*v2.y) + (v1.z*v2.z);

	return Dot_to_Position;
}

//���ςŃR�T�C���l�����߂�
float MyMath::Vector_Dot(const ML::Vec3& v1, const ML::Vec3& v2)
{
	//�x�N�g���̒���
	float v1_Lenth, v2_Lenth;
	v1_Lenth = v1.Length();
	v2_Lenth = v2.Length();

	//���W��̓���
	float Dot_to_Position;
	Dot_to_Position = this->Get_Vector_Dot(v1, v2);

	//�R�T�C���l�����߂ĕԂ�
	float cos;

	cos = Dot_to_Position / (v1_Lenth*v2_Lenth);

	return cos;
}

//�O�ςŃT�C���l�����߂�
float MyMath::Vector_Cross(const ML::Vec3& v1, const ML::Vec3& v2)
{
	//�x�N�g���̒���
	float v1_Lenth, v2_Lenth;

	v1_Lenth = v1.Length();
	v2_Lenth = v2.Length();

	//���W��̊O��(�@���x�N�g��)
	ML::Vec3 Cross_to_Position;
	this->Get_Normal_to_Vector_Cross(&Cross_to_Position, v1, v2);

	float sin;

	sin = Cross_to_Position.Length() / (v1_Lenth * v2_Lenth);

	return sin;
}
