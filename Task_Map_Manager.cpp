//-------------------------------------------------------------------
//�}�b�v�}�l�[�W��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Map_Manager.h"
#include  "Task_MapCore.h"
#include  "Task_MapSide.h"
#include  "Task_MapFence.h"
#include  "Task_Ball.h"

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
		auto in1 = DI::GPad_GetState("P1");
		//�ق��̃^�X�N�̏���������Ă���
		auto map = ge->GetTask_Group_GN<Map3d::Object>("�}�b�v", "Side");
		auto fence = ge->GetTask_Group_GN<MapFence::Object>("�}�b�v", "Fence");
		auto core = ge->GetTask_One_G<Map_Core::Object>("�}�b�v");
		auto ball = ge->GetTask_One_G<Ball::Object>("�{�[��");

		ML::QT frame_QT = ML::QT(0.0f);
		std::vector<After_Collision> Result;
		for (int i = 0; i < 10; i++)
		{
			//�X�e�B�b�N���|���ꂽ�ʂ��X�V
			if (in1.B1.on)
			{
				//�X�e�B�b�N�œ���
				if (in1.LStick.axis.x != 0)
				{
					frame_QT = ML::QT(ML::Vec3(0, 0, 1), ML::ToRadian(-in1.LStick.axis.x / 10.0f));
					//this->map_QT *= frame_QT;
				}
			}
			//������Ă��Ȃ�����Y����]��X����]
			else
			{

				if (in1.LStick.axis.y != 0)
				{
					frame_QT = ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(-in1.LStick.axis.y / 10.0f));
					//this->map_QT *= frame_QT;
				}

				if (in1.LStick.axis.x != 0)
				{
					frame_QT = ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(-in1.LStick.axis.x / 10.0f));
					//this->map_QT *= frame_QT;
				}
			}

			//�e�^�X�N�̉�]�ʂ��X�V
			core->UpDate_Quartanion(frame_QT);
			for (auto m = map->begin(); m != map->end(); m++)
			{
				(*m)->UpDate_Quartanion(frame_QT);
			}
			for (auto f = fence->begin(); f != fence->end(); f++)
			{
				(*f)->UpDate_Quartanion(frame_QT);
			}

			//����̌��ʂ�ۑ�
			Result = core->Get_Collision_Poligon();
			for (auto i = map->begin(); i != map->end(); i++)
			{
				for (auto it : (*i)->Get_Collision_Poligon())
				{
					Result.push_back(it);
				}
			}
			for (auto i = fence->begin(); i != fence->end(); i++)
			{
				for (auto it : (*i)->Get_Collision_Poligon())
				{
					Result.push_back(it);
				}
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

			//�{�[���ƃ}�b�v�̂����蔻��
			if (ball != nullptr)
			{
				core->Core_Check_Hit(ball->Get_Pos(), ball->Get_Radious(), ball->Get_Speed());
				for (auto m = map->begin(); m != map->end(); m++)
				{
					(*m)->Map_Check_Hit(ball->Get_Pos(), ball->Get_Radious(), ball->Get_Speed());
				}
				for (auto f = fence->begin(); f != fence->end(); f++)
				{
					(*f)->Map_Check_Hit(ball->Get_Pos(), ball->Get_Radious(), ball->Get_Speed());
				}
				
			}

			//�ʒu�␳���d�|����
			for (auto p : Result)
			{
				if (p.collision_Flag)
				{
					ball->Fix_Position_for_Rotate(frame_QT);
					break;
				}
			}
		}

		
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		
	}

	void  Object::Render3D_L0()
	{
		//���s�ړ��s��𐶐� //���[���h�ϊ���K�p����
		
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