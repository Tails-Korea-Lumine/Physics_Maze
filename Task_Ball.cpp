//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Ball.h"
#include  "Task_MapSide.h"
#include  "Task_MapCore.h"
#include  "Task_MapFence.h"
#include "Task_CameraMan.h"
#include "Physics.h"
#include <iostream>

namespace  Ball
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->meshName = "Item";
		DG::Mesh_CreateFromSOBFile(this->meshName, "./data/mesh/Item.SOB");
		this->hit_Se_Name = "Hit_the_Ball";
		DM::Sound_CreateSE(this->hit_Se_Name, "./data/sound/Ball_Hit.wav");
	
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{		
		DG::Mesh_Erase(this->meshName);	
		DM::Sound_Erase(this->hit_Se_Name);
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize()
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������		
		this->speed = ML::Vec3(0, 0, 0);

		float r = 30.0f;
		this->sphere = new Sphere(ML::Vec3(1000, 500, 900), ML::Vec3(r, r, r), ML::QT());//���������ʒu�͉��̈ʒu
		this->m = 35.0f;
		this->teleportation_Flag = false;

	
		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->eff_Manager.lock()->Add_Effect(this->Get_Pos(), ML::Vec3(0, 0, 0), BEffect::effType::Game_Clear);
		delete this->sphere;

		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			//�������p���^�X�N�̐���
			
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto cm = ge->GetTask_One_G<CameraMan::Object>("�J�����}��");
		if (cm != nullptr && cm->Is_Moving_Now() == false)
		{
			//�d�͉���
			Physics::Gravity_Accelerate(&this->speed, this->m);
		}		

		//�e���|�[�g�t���O�𖳌���
		this->teleportation_Flag = false;		
	}
	//-------------------------------------------------------------------
	//�ړ�
	void Object::Move_Ball(const unsigned int& precisioin)
	{	
		//�ړ�
		this->sphere->Offset(this->speed * ge->g_Time.Delta_Time() / (float)precisioin);
		//��]�ʏ㏸
		this->Rotate(precisioin);
	}

	//�����蔻��ɂ�镨������
	void Object::Physics_Actioin(const unsigned int& precision)
	{
		const float sin = sinf(ML::ToRadian(89));
		//���ʂ̐������[�v����
		for (auto p : ge->collision_Result)
		{
			//���x�Ɩ@���x�N�g���̊p�x���s�p�Ȃ��������
			float cos;
			MyMath::Vector_Dot(&cos, this->speed, p.normal);
			if (cos >= 0.0f)
			{
				continue;
			}
			//���ˊp�Œ��˕Ԃ�
			Physics::Reflaction_Vector(&this->speed, p.normal);

			//�Փ˂̉����o������
			float s;
			MyMath::Vector_Cross(&s, this->speed, p.normal);
			
			//���ˊp�����p�x�ȉ��܂��́A1.3m/s�ȉ��͉����o���Ȃ�
			if ((s <= sin) && speed.Length() >= 130.0f)
			{
				DM::Sound_Play(this->res->hit_Se_Name, false);
			}
		}		
		//�ړ�
		this->Move_Ball(precision);
	}
	
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{		
	}

	void  Object::Render3D_L0()
	{
		ML::Mat4x4 mat;

		D3DXMatrixAffineTransformation(&mat, this->sphere->Get_Length().x, NULL, &this->sphere->Get_Quaternion(), &this->sphere->Get_Center());

		DG::EffectState().param.matWorld = mat;

		DG::Mesh_Draw(this->res->meshName);
	}

	//--------------------------------------------------------------------
	//�ǉ����\�b�h
	
	//�]����\���̂��߂ɉ�]������
	void Object::Rotate(const unsigned int& precision)
	{
		ML::Vec3 anker;
		float rot;
		//��]�����v�Z����
		for (auto& col : ge->collision_Result)
		{
			//��]�������
			MyMath::Get_Normal_to_Vector_Cross(&anker, this->speed, col.normal);
			anker = anker.Normalize();
			//�����[���x�N�g���Ȃ珈�����Ȃ�
			if (anker.Is_Zero_Vec())
			{
				continue;
			}
			//���x�ɑ΂����]�ʌv�Z
			MyMath::Get_Rotation_By_Speed(&rot, this->sphere->Get_Length().x, this->speed.Length() * ge->g_Time.Delta_Time()/ precision);
			//��]�s��ƃN�H�[�^�j�I���̐���
			ML::QT qt = ML::QT(anker, -rot);
			ML::Mat4x4 matR;
			D3DXMatrixAffineTransformation(&matR, 1.0f, &this->sphere->Get_Center(), &qt, NULL);

			//��]�ʃA�b�v�f�[�g
			this->sphere->Rotation(&matR, qt);			
		}
	}

	//---------------------------------------------------------------------
	//���̎��̃{�[���̏���Ԃ��֐�
	//�ʒu
	ML::Vec3 Object::Get_Pos() const
	{
		return this->sphere->Get_Center();
	}
	//�����a
	float Object::Get_Radious() const
	{
		return this->sphere->Get_Length().x;
	}
	//���x
	ML::Vec3 Object::Get_Speed() const
	{
		return this->speed;
	}

	//--------------------------------------------------------------------
	//�ʒu�␳�p��]�֐�
	void Object::Fix_Position_for_Rotate(const ML::QT& qt)
	{
		if (ge->collision_Result.empty())
		{			
			return;
		}
		//�}�b�v�̃t���[����]�ʂŉ�]������
		ML::Mat4x4 matR;
		D3DXMatrixAffineTransformation(&matR, 1, &ge->Map_center, &qt, NULL);

		//this->pos = matR.TransformCoord(this->pos);
		this->sphere->Rotation(&matR, qt);		
	}

	//----------------------------------------------------------------------------
	//�e���|�[�g(�w�肵���ꏊ�ɑ��ړ�����)
	void Object::Teleportation(const ML::Vec3& objectPos)
	{
		this->teleportation_Flag = true;
		this->sphere->Set_Position(objectPos);
	}

	//------------------------------------------------------------------------------
	//�R�A�Ƃ������ĂȂ����R�A�̕��Ɉ���������
	/*void Object::Graviation_Pull(const ML::Vec3& force)
	{
		this->speed += force;
	}*/
	//-----------------------------------------------------------------------------
	//�e���|�[�g���������m�F
	bool Object::Is_Teleport_Now() const
	{
		return this->teleportation_Flag;
	}
	//�����蔻��͈͂����炤
	Shape3D* Object::Get_Collision_Area()
	{
		return this->sphere;
	}
	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize()
	{
		return  this->Initialize();
	}
	//-------------------------------------------------------------------
	Object::~Object() { this->B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object() {	}
	//-------------------------------------------------------------------
	//���\�[�X�N���X�̐���
	Resource::SP  Resource::Create()
	{
		if (auto sp = instance.lock()) {
			return sp;
		}
		else {
			sp = Resource::SP(new  Resource());
			if (sp) {
				sp->Initialize();
				instance = sp;
			}
			return sp;
		}
	}
	//-------------------------------------------------------------------
	Resource::Resource() {}
	//-------------------------------------------------------------------
	Resource::~Resource() { this->Finalize(); }
}