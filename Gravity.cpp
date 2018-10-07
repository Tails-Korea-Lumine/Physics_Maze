#include "Gravity.h"
#include "MyPG.h"

void Gravity::Accelerate(ML::Vec3* speed, const float& Weight) const
{
	*speed += (Weight * this->G_acceleration );
}

void Gravity::CollisionOver_Accelerate(ML::Vec3* speed, const ML::Vec3& normal) const
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
	//���̂�����ver0.1

	float fn;
	MyMath::Get_Vector_Dot(&fn, *speed, normal);
	//fn *= speed.Length();
	//��Βl�ɕς���
	if (fn < 0)
	{
		fn *= -1;
	}	
	
	//������͂ɍ��킹����̃x�N�g���̑傫��
	ML::Vec3 after_Normal;

	after_Normal = normal * fn;

	//���ˊp�ɕϊ������x�N�g��
	//ML::Vec3 after_Collision;

	*speed += after_Normal;
	//�d���������ɉe����^����
	//return after_Collision;
}

void Gravity::Reflaction_Vector(ML::Vec3* force, const ML::Vec3& normal, const float& weight) const
{	
	//�ǖʂ̖@���x�N�g���̑傫����ύX
	float fn;
	MyMath::Get_Vector_Dot(&fn, *force, normal);
	//fn *= force.Length();
	//��Βl�ɕς���
	if (fn < 0)
	{
		fn *= -1;
	}	

	//������͂ɍ��킹����̃x�N�g���̑傫��
	ML::Vec3 after_Normal;

	after_Normal = normal * fn;

	//���ˊp�ɕϊ������x�N�g��
	//ML::Vec3 after_Reflection;
	
	*force += (2 * after_Normal);
	*force *= 0.75f;

	//�d���ɉ����Č��炵�ĕԂ�
	//return after_Reflection *0.6f;
}
