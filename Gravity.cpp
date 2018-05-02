#include "Gravity.h"
#include <cmath>

//�@���x�N�g�������߂�
ML::Vec3 Gravity::Get_Normal_to_Vector_Cross(ML::Vec3 v1, ML::Vec3 v2)
{
	ML::Vec3 Normal;
	Normal.x = (v1.y*v2.z) - (v1.z*v2.y);
	Normal.y = (v1.z * v2.x) - (v1.x*v2.z);
	Normal.z = (v1.x*v2.y) - (v1.y*v2.x);

	return Normal;
}
//���ς����߂�
float Gravity::Get_Vector_Dot(ML::Vec3 v1, ML::Vec3 v2)
{
	float Dot_to_Position;
	Dot_to_Position = (v1.x*v2.x) + (v1.y*v2.y) + (v1.z*v2.z);

	return Dot_to_Position;
}

//���ςŃR�T�C���l�����߂�
float Gravity::Vector_Dot(ML::Vec3 v1, ML::Vec3 v2)
{
	//�x�N�g���̒���
	float v1_Lenth, v2_Lenth;
	v1_Lenth = v1.Length();
	v2_Lenth = v2.Length();

	//���W��̓���
	float Dot_to_Position;
	Dot_to_Position = Gravity::Get_Vector_Dot(v1, v2);

	//�R�T�C���l�����߂ĕԂ�
	float cos;

	cos = Dot_to_Position / (v1_Lenth*v2_Lenth);

	return cos;
}

//�O�ςŃT�C���l�����߂�
float Gravity::Vector_Cross(ML::Vec3 v1, ML::Vec3 v2)
{
	//�x�N�g���̒���
	float v1_Lenth, v2_Lenth;

	v1_Lenth = v1.Length();
	v2_Lenth = v2.Length();	

	//���W��̊O��(�@���x�N�g��)
	ML::Vec3 Cross_to_Position;
	Cross_to_Position = Gravity::Get_Normal_to_Vector_Cross(v1, v2);

	float sin;

	sin = Cross_to_Position.Length() / (v1_Lenth * v2_Lenth);

	return sin;
}

float Gravity::Cheak_angle(ML::Vec3 normal)
{
	return Gravity::Vector_Dot(this->G_acceleration, normal);
}

ML::Vec3 Gravity::Accelerate(float Weight)
{
	return Weight * this->G_acceleration;
}

ML::Vec3 Gravity::CollisionOver_Accelerate(ML::Vec3 speed, ML::Vec3 normal, float weight)
{
	/*//�@���x�N�g���Ƒ��x�̊p�x�ƃT�C���l
	float sin , seta;

	sin = Gravity::Vector_Dot(-speed, normal);

	seta = _CMATH_::asin(sin);

	//�������ς������̗̗͂�
	float force;

	force = this->G_acceleration.y * sin;

	//��]���͕ǖʂ̖@���x�N�g���Ƒ��x�x�N�g���̊O�ςł��炦��ʂ̖@���x�N�g��
	//��]�ʂ͔����v���Ȃ̂�-(90-��) = ��-90

	//��]�p�s��
	ML::Mat4x4 rotation;

	rotation.RotationAxis(Gravity::Get_Normal_to_Vector_Cross(-speed, normal), (seta - 90));

	rotation.TransformNormal(speed);
	
	//�e�v�f�ɕω���̗̗͂ʂ��|����
	speed *= force;*/

	//----------------------------------------------------------------------------
	//���̂�����ver.1

	float fn;
	fn = Gravity::Get_Vector_Dot(speed, normal);
	//fn *= speed.Length();
	//��Βl�ɕς���
	if (fn < 0)
	{
		fn *= -1;
	}
	////�}�X�ƃ}�X�ڐG�ʂł�������������h��
	//float cosSN = Gravity::Vector_Dot((speed - Gravity::Accelerate(weight)), normal);
	////cos�l��1�Ƃ������Ƃ͓��p��0�x���Ƃ������ƁA�܂蕨���I�ɂ��蓾�Ȃ��Փ�
	//if (cosSN >=_CMATH_::cos(ML::ToRadian(2)))
	//{
	//	//�Ȃ̂ŉ����Ȃ���return
	//	//return speed - Gravity::Accelerate(weight);
	//	return speed;
	//}
	
	//������͂ɍ��킹����̃x�N�g���̑傫��
	ML::Vec3 after_Normal;

	after_Normal = normal * fn;

	//���ˊp�ɕϊ������x�N�g��
	ML::Vec3 after_Collision;

	after_Collision = speed + after_Normal;
	//�d���������ɉe����^����
	return after_Collision;
}

ML::Vec3 Gravity::Reflaction_Vector(ML::Vec3 force, ML::Vec3 normal, float weight)
{	
	//�ǖʂ̖@���x�N�g���̑傫����ύX
	float fn;
	fn = Gravity::Get_Vector_Dot(force, normal);
	//fn *= force.Length();
	//��Βl�ɕς���
	if (fn < 0)
	{
		fn *= -1;
	}
	
	////�}�X�ƃ}�X�ڐG�ʂł�������������h��
	//float cosSN = Gravity::Vector_Dot((force - Gravity::Accelerate(weight)), normal);
	////cos�l��1�Ƃ������Ƃ͓��p��0�x���Ƃ������ƁA�܂蕨���I�ɂ��蓾�Ȃ��Փ�
	//if (cosSN >= _CMATH_::cos(ML::ToRadian(2)))
	//{
	//	//�Ȃ̂ŉ����Ȃ���return
	//	//return force - Gravity::Accelerate(weight);
	//	return force;
	//}

	//������͂ɍ��킹����̃x�N�g���̑傫��
	ML::Vec3 after_Normal;

	after_Normal = normal * fn;

	//���ˊp�ɕϊ������x�N�g��
	ML::Vec3 after_Reflection;
	
	after_Reflection = force + (2 * after_Normal);

	//�d���ɉ����Č��炵�ĕԂ�
	return after_Reflection *0.3f;
}

void Gravity::Rotation_on_Gravity(float angle, ML::Vec3 centor)
{
	//�}�b�v�̐݌v���I�������Ɏ����\��(2018/03/15)
}