#include "Physics.h"
#include "MyPG.h"


const static ML::Vec3 G_acceleration = ML::Vec3(0, -980.0f, 0);

void Physics::Gravity_Accelerate(ML::Vec3* speed, const float& Weight)
{
	//9.8m/s^2
	float delta2 = ge->g_Time.Delta_Time() * ge->g_Time.Delta_Time();
	*speed += (Weight * (G_acceleration * delta2));	
}

void Physics::Resizing_Normal(ML::Vec3* result, const ML::Vec3& force, const ML::Vec3& normal)
{
	//�ǖʂ̖@���x�N�g���̑傫����ύX
	float fn;
	MyMath::Get_Vector_Dot(&fn, force, normal);
	//��Βl�ɕς���
	fn = abs(fn);

	*result = normal * fn;
}

void Physics::Diagonal_Accelerate(ML::Vec3* speed, const ML::Vec3& normal)
{
	//������͂ɍ��킹����̃x�N�g��
	ML::Vec3 after_Normal;

	Physics::Resizing_Normal(&after_Normal, *speed, normal);

	//�x�N�g������
	*speed += after_Normal;	
}

void Physics::Reflaction_Vector(ML::Vec3* force, const ML::Vec3& normal)
{	
	//������͂ɍ��킹����̃x�N�g��
	ML::Vec3 after_Normal;

	Physics::Resizing_Normal(&after_Normal, *force, normal);

	//���ˊp�ɕϊ������x�N�g��
	*force += (1.5f * after_Normal);
	//���Ղ����͂��l����60���ɂ���
	//*force *= 0.8f;	
}
