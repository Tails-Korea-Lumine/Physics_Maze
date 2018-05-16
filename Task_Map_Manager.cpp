//-------------------------------------------------------------------
//�}�b�v�}�l�[�W��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Map_Manager.h"
#include  "Task_MapCore.h"
#include  "Task_MapSide.h"
#include  "Task_MapFence.h"
#include  "Task_Ball.h"
#include "easing.h"

namespace  Map_Manager
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{		
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{		
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
		easing::Init();

		this->frame_QTxp = ML::QT(0.0f);
		this->frame_QTxm = ML::QT(0.0f);
		this->frame_QTyp = ML::QT(0.0f);
		this->frame_QTym = ML::QT(0.0f);
		this->frame_QTzp = ML::QT(0.0f);
		this->frame_QTzm = ML::QT(0.0f);
	
		//easing set
		easing::Set("Decrese_StickVolumeXP", easing::QUARTIN, 1.7f, 0, 4);
		easing::Set("Decrese_StickVolumeXM", easing::QUARTIN, 1.7f, 0, 4);
		easing::Set("Decrese_StickVolumeYP", easing::QUARTIN, 1.7f, 0, 4);
		easing::Set("Decrese_StickVolumeYM", easing::QUARTIN, 1.7f, 0, 4);
		easing::Set("Decrese_StickVolumeZP", easing::QUARTIN, 1.7f, 0, 4);
		easing::Set("Decrese_StickVolumeZM", easing::QUARTIN, 1.7f, 0, 4);
		//easing set end						 
		easing::Set_End("Decrese_StickVolumeXP");
		easing::Set_End("Decrese_StickVolumeXM");
		easing::Set_End("Decrese_StickVolumeYP");
		easing::Set_End("Decrese_StickVolumeYM");
		easing::Set_End("Decrese_StickVolumeZP");
		easing::Set_End("Decrese_StickVolumeZM");

		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���

		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			//�������p���^�X�N�̐���
			//auto nextTask = Game::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{				
		//�}�b�v�Ɋւ��邱�Ƃ͑S���}�l�[�W�����ł�� ver0.6
		this->Managing_All_Map(15);
		
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		
	}

	void  Object::Render3D_L0()
	{
		
		
	}

	//----------------------------------------------------------------------------------
	//�ǉ����\�b�h
	//�}�b�v�̏����������؂�ɊǗ�����
	void Object::Managing_All_Map(const unsigned int delicate)
	{
		auto in1 = DI::GPad_GetState("P1");
		//�ق��̃^�X�N�̏���������Ă���
		auto map = ge->GetTask_Group_GN<Map3d::Object>("�}�b�v", "Side");
		auto fence = ge->GetTask_Group_GN<MapFence::Object>("�}�b�v", "Fence");
		auto core = ge->GetTask_One_G<Map_Core::Object>("�}�b�v");
		auto ball = ge->GetTask_One_G<Ball::Object>("�{�[��");


		//std::vector<After_Collision> Result;
		for (unsigned int i = 0; i < delicate; i++)
		{
			//z����]���{�^���������Ȃ�����ver0.1
			//if (in1.B1.on)
			//{
			//	//�X�e�B�b�N�œ���
			//	if (in1.LStick.axis.x != 0)
			//	{
			//		frame_QTx = ML::QT(ML::Vec3(0, 0, 1), ML::ToRadian(-in1.LStick.axis.x / float(delicate)));
			//		//this->map_QT *= frame_QT;
			//	}
			//}
			////������Ă��Ȃ�����Y����]��X����]
			//else
			//{

			//	if (in1.LStick.axis.y != 0)
			//	{
			//		frame_QTx = ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(-in1.LStick.axis.y / float(delicate)));
			//		//this->map_QT *= frame_QT;
			//	}

			//	if (in1.LStick.axis.x != 0)
			//	{
			//		frame_QTy = ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(-in1.LStick.axis.x / float(delicate)));
			//		//this->map_QT *= frame_QT;
			//	}
			//}
			
			//��]��easing�Ō����^��������ver0.3
			
			//���͂���Ă鏊��easing�f�[�^�����Z�b�g������
			if (in1.LStick.axis.y > 0)
			{
				easing::Reset("Decrese_StickVolumeXM");
				easing::Start("Decrese_StickVolumeXM");
			}
			else if (in1.LStick.axis.y < 0)
			{
				easing::Reset("Decrese_StickVolumeXP");
				easing::Start("Decrese_StickVolumeXP");
			}
			if (in1.LStick.axis.x > 0)
			{
				easing::Reset("Decrese_StickVolumeYM");
				easing::Start("Decrese_StickVolumeYM");
			}
			else if (in1.LStick.axis.x < 0)
			{
				easing::Reset("Decrese_StickVolumeYP");
				easing::Start("Decrese_StickVolumeYP");
			}
			if (in1.R2.on)
			{
				easing::Reset("Decrese_StickVolumeZM");
				easing::Start("Decrese_StickVolumeZM");
			}
			else if (in1.L2.on)
			{
				easing::Reset("Decrese_StickVolumeZP");
				easing::Start("Decrese_StickVolumeZP");
			}

			//easing�f�[�^����]�ʂōX�V�����
			this->frame_QTxp = ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(-easing::GetPos("Decrese_StickVolumeXM")  / float(delicate)));
						
			this->frame_QTxm = ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(easing::GetPos("Decrese_StickVolumeXP") / float(delicate)));
			
			
			this->frame_QTym = ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(-easing::GetPos("Decrese_StickVolumeYM") / float(delicate)));

			this->frame_QTyp = ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(easing::GetPos("Decrese_StickVolumeYP") / float(delicate)));
						
			
			this->frame_QTzm = ML::QT(ML::Vec3(0, 0, 1), ML::ToRadian(-easing::GetPos("Decrese_StickVolumeZM") / float(delicate)));

			this->frame_QTzp = ML::QT(ML::Vec3(0, 0, 1), ML::ToRadian(easing::GetPos("Decrese_StickVolumeZP") / float(delicate)));
				
			

			//�e�^�X�N�̉�]�ʂ��X�V
			core->UpDate_Quartanion(this->frame_QTxp * this->frame_QTxm * this->frame_QTyp * this->frame_QTym *this->frame_QTzm * this->frame_QTzp);
			for (auto m = map->begin(); m != map->end(); m++)
			{
				(*m)->UpDate_Quartanion(this->frame_QTxp * this->frame_QTxm * this->frame_QTyp* this->frame_QTym *this->frame_QTzm * this->frame_QTzp);
			}
			for (auto f = fence->begin(); f != fence->end(); f++)
			{
				(*f)->UpDate_Quartanion(this->frame_QTxp * this->frame_QTxm * this->frame_QTyp* this->frame_QTym *this->frame_QTzm * this->frame_QTzp);
			}
			

			//���[���h��]�ʂɔ��f
			//ge->World_Rotation = this->map_QT;		


			//��]
			core->Rotate_Core_and_Barrier();
			for (auto m = map->begin(); m != map->end(); m++)
			{
				(*m)->Map_Rotate();
			}
			for (auto f = fence->begin(); f != fence->end(); f++)
			{
				(*f)->Map_Rotate();
			}




			//�����蔻��͖���}�b�v�̂ق��ōs��	
			ge->collision_Result.clear();
			//�{�[���ƃ}�b�v�̂����蔻��
			if (ball != nullptr)
			{
				//�R�A�ƃ{�[�����ڐG���Ă��Ȃ����A���͂�������(�o���A�̊O�ɏo���Ȃ��悤�ɂ���d�g��)
				if (!core->Core_Check_Hit(ball->Get_Pos(), ball->Get_Radious(), ball->Get_Speed()))
				{
					ML::Vec3 force = ge->Map_center - ball->Get_Pos();
					ball->Graviation_Pull(force.Normalize());
				}
				for (auto m = map->begin(); m != map->end(); m++)
				{
					(*m)->Map_Check_Hit(ball->Get_Pos(), ball->Get_Radious(), ball->Get_Speed());
				}
				for (auto f = fence->begin(); f != fence->end(); f++)
				{
					(*f)->Map_Check_Hit(ball->Get_Pos(), ball->Get_Radious(), ball->Get_Speed());
				}

			}

			//����̌��ʂ�ۑ�
			core->Get_Collision_Poligon(&ge->collision_Result);
			for (auto i = map->begin(); i != map->end(); i++)
			{
				(*i)->Get_Collision_Poligon(&ge->collision_Result);
			}
			for (auto i = fence->begin(); i != fence->end(); i++)
			{
				(*i)->Get_Collision_Poligon(&ge->collision_Result);
			}
			//ge->collision_Result = Result;

			//�ʒu�␳���d�|����
			for (auto p : ge->collision_Result)
			{
				if (p.collision_Flag)
				{
					ball->Fix_Position_for_Rotate(this->frame_QTxp * this->frame_QTxm * this->frame_QTyp* this->frame_QTym *this->frame_QTzm * this->frame_QTzp);
					break;
				}
			}
		}
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