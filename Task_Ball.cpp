//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Ball.h"
#include  "Task_MapSide.h"
#include  "Task_MapCore.h"
#include  "Task_MapFence.h"
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
		//this->pos = ML::Vec3(1000, 500, 900);//���̈ʒu��Œ�����������(2018/04/20)
		this->speed = ML::Vec3(0, 0, 0);
		//this->moveVec = ML::Vec3(0, 0, 0);

		float r = 30.0f;
		this->sphere = new Sphere(ML::Vec3(1000, 500, 900), ML::Vec3(r, r, r), ML::QT());
		this->m = 15.0f;
		//this->rot = 0.0f;
		this->collision_Flag.clear();
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
		this->collision_Flag.clear();
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
		//�d�͉���
		Physics::Gravity_Accelerate(&this->speed,this->m);				
			
		//��]�ʏ㏸
		this->Rotate();

		//�e���|�[�g�t���O�𖳌���
		this->teleportation_Flag = false;
		
		//�f�o�b�O�p
		float sl = this->speed.Length();
		if (sl > 4.0f)
		{
			cout << sl << endl;
		}		
	}
	//-------------------------------------------------------------------
	//�����蔻��ɂ������]���y�шړ�
	void Object::Move_Ball(const unsigned int& precisioin)
	{
		int vol;
		//�I�[���x�y�ѕ��������x�ɂ�鑬�x����
		auto Clamp_Speed = [&](ML::Vec3& speed)
		{
			//1�t���[���̏I�[���x
			const float termination_Speed = 6.0f;
			//�I�[���x�ɒ�������ꍇ
			if (speed.Length() > termination_Speed)
			{
				speed = speed.Normalize();
				speed *= termination_Speed;
			}
			return speed / (float)precisioin;
		};
		
		//����,�ǂ��������蔻��������ɓ������ꍇ
		//���������Ɏ��̃t���[���Ɉڂ�
		if (ge->collision_Result.empty())
		{
			//����̃t���[���ɏՓ˂��Ȃ�������
			//�Փ˃t���O�𖳌��ɂ���
			this->collision_Flag.clear();
			
			//�ړ�(�t���[���I�����钼�O�ɍs��)
			this->sphere->Offset(Clamp_Speed(this->speed));
			return;
		}
		//�O��t���[���̂����蔻�茋�ʂ��m�F
		
		//���ʂ̐������[�v����
		for (auto p : ge->collision_Result)
		{
			//���x�Ɩ@���x�N�g���̊p�x�����������Ȃ��������
			float cos;
			MyMath::Vector_Dot(&cos, this->speed, p.normal);
			if (cos >= cosf(ML::ToRadian(85)))
			{
				continue;
			}
			//�O�̏����ŏՓ˂�������A����̃t���[���ł̏Փ˔���ł�邱��
			if (this->Is_Collision(p.collision_Id))
			{
				//����̃t���[���ɏՓ˂�������
				//�΂ߐ�����������
				Physics::Diagonal_Accelerate(&this->speed, p.normal);
			}		
			//�O�̏����ŏՓ˂ł͂Ȃ�������A����̃t���[���ł̏Փ˔���ł�邱��			
			else
			{
				//����̃t���[���ɏՓ˂�������
				//���ˊp�Œ��˕Ԃ�
				Physics::Reflaction_Vector(&this->speed, p.normal);
				//DM::Sound_Volume(this->res->hit_Se_Name, int((this->speed.Length() / 6) * 1000));
				DM::Sound_Play(this->res->hit_Se_Name, false);
			}
		}		

		//����g�����t���O�͑S�������āA����̂����蔻�茋�ʂ̃t���O����������
		this->collision_Flag.clear();
		for (auto& p : ge->collision_Result)
		{			
			this->collision_Flag.push_back(p.collision_Id);
		}
		
		//�ړ�(�t���[���I�����钼�O�ɍs��)
		this->sphere->Offset(Clamp_Speed(this->speed));
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

	//�����蔻��t���O���m�F
	bool Object::Is_Collision(const string& id) const
	{
		for (auto& cf : this->collision_Flag)
		{
			if (cf == id)
			{
				return true;
			}
		}
		return false;
	}
	//--------------------------------------------------------------
	//�]����\���̂��߂ɉ�]������
	void Object::Rotate()
	{
		ML::Vec3 anker;
		float cos;
		//��]�����v�Z����
		for (auto& col : ge->collision_Result)
		{
			//���ςő��x��90�x�̖@���x�N�g����T��
			MyMath::Vector_Dot(&cos, this->speed, col.normal);

			if (cos >= _CMATH_::cosf(ML::ToRadian(90)))
			{
				//��]�������
				MyMath::Get_Normal_to_Vector_Cross(&anker, this->speed, col.normal);
				//��]�s��ƃN�H�[�^�j�I���̐���
				ML::QT rot = ML::QT(anker, this->speed.Length());
				ML::Mat4x4 matR;
				D3DXMatrixAffineTransformation(&matR, 1.0f, &this->sphere->Get_Center(), &rot, NULL);

				//��]�ʃA�b�v�f�[�g
				this->sphere->Rotation(&matR, rot);
			}
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
			DM::Sound_Volume(this->res->hit_Se_Name, 1000);
			return;
		}
		//�}�b�v�̃t���[����]�ʂŉ�]������
		ML::Mat4x4 matR;
		D3DXMatrixAffineTransformation(&matR, 1, &ge->Map_center, &qt, NULL);

		//this->pos = matR.TransformCoord(this->pos);
		this->sphere->Rotation(&matR, qt);

		DM::Sound_Volume(this->res->hit_Se_Name, 0);
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