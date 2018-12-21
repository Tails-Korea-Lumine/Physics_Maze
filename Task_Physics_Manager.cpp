//-------------------------------------------------------------------
//�}�b�v�}�l�[�W��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Physics_Manager.h"
#include "easing.h"
#include "MyMath.h"
#include  "Task_CameraMan.h"
#include  "Task_MapCore.h"
#include  "Task_MapSide.h"
#include  "Task_MapFence.h"
#include  "Task_Ball.h"


#define PRECISION 3

namespace  Physics_Manager
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
		

		this->esasing_Name_QTxp = "Decrese_StickVolumeXP";
		this->esasing_Name_QTxm = "Decrese_StickVolumeXM";
		this->esasing_Name_QTyp = "Decrese_StickVolumeYP";
		this->esasing_Name_QTym = "Decrese_StickVolumeYM";
		this->esasing_Name_QTzp = "Decrese_StickVolumeZP";
		this->esasing_Name_QTzm = "Decrese_StickVolumeZM";

		//���̏����l
		this->anckerX = ML::Vec3(1, 0, 0);
		this->anckerY = ML::Vec3(0, 1, 0);
		this->anckerZ = ML::Vec3(0, 0, 1);
	
		//easing set
		easing::Set(this->esasing_Name_QTxp, easing::QUARTIN, 1.7f, 0, 7);
		easing::Set(this->esasing_Name_QTxm, easing::QUARTIN, 1.7f, 0, 7);
		easing::Set(this->esasing_Name_QTyp, easing::QUARTIN, 1.7f, 0, 7);
		easing::Set(this->esasing_Name_QTym, easing::QUARTIN, 1.7f, 0, 7);
		easing::Set(this->esasing_Name_QTzp, easing::QUARTIN, 1.7f, 0, 7);
		easing::Set(this->esasing_Name_QTzm, easing::QUARTIN, 1.7f, 0, 7);
		//easing set end						 
		easing::Set_End(this->esasing_Name_QTxp);
		easing::Set_End(this->esasing_Name_QTxm);
		easing::Set_End(this->esasing_Name_QTyp);
		easing::Set_End(this->esasing_Name_QTym);
		easing::Set_End(this->esasing_Name_QTzp);
		easing::Set_End(this->esasing_Name_QTzm);

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
		this->Managing(PRECISION);

		this->Input_Analog_Action();
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
	void Object::Managing(const unsigned int precision)
	{	
		auto in1 = DI::GPad_GetState("P1");
		//�ق��̃^�X�N�̏���������Ă���
		auto map = ge->GetTask_Group_GN<Map_Side::Object>("�}�b�v", "Side");
		auto fence = ge->GetTask_Group_GN<MapFence::Object>("�}�b�v", "Fence");
		auto core = ge->GetTask_One_G<Map_Core::Object>("�}�b�v");
		auto ball = ge->GetTask_One_G<Ball::Object>("�{�[��");
		auto cm = ge->GetTask_One_G<CameraMan::Object>("�J�����}��");

		//�{�[�������݂��Ȃ��ꍇ���J�����ړ����Ȃ珈�����Ȃ�
		if (ball == nullptr || cm->Is_Moving_Now() == true)
		{
			return;
		}

		//�{�[�������݂��Ă���ʂ̖@���x�N�g��
		std::vector<ML::Vec3> side_Normals;
		float delicate = 0.0f;
		//�������@�\		
		delicate = in1.B2.on ? (float)precision *2.0f : (float)precision;

		for (unsigned int i = 0; i < precision; i++)
		{			
			//�t���[����]�ʂ̂܂Ƃ�				
			ML::QT frame_QT_All = this->Calculate_Frame_Quatanion(delicate);
			//�@���x�N�g���ۑ��ꏊ�N���A
			side_Normals.clear();
			//�e�^�X�N�̉�]�ʂ��X�V�y�щ�]
			core->Rotate_Core_and_Barrier(frame_QT_All);
			for (auto m = map->begin(); m != map->end(); m++)
			{
				(*m)->Map_Rotate(frame_QT_All);
				//�}�b�v�̖@���x�N�g��������
				side_Normals.push_back((*m)->Get_Normal_Side());
			}
			for (auto f = fence->begin(); f != fence->end(); f++)
			{
				(*f)->Map_Rotate(frame_QT_All);
			}
			//�ʒu�␳���d�|����
			ball->Fix_Position_for_Rotate(frame_QT_All);

			ge->collision_Result.clear();
		
			//�{�[���ƃ}�b�v�̂����蔻��y�ѕۑ�
			//�R�A
			core->Core_Check_Hit(ball->Get_Collision_Area());
			//��
			for (auto m = map->begin(); m != map->end(); m++)
			{				
				(*m)->Map_Check_Hit(ball->Get_Collision_Area());
			}
			//�t�F���X
			for (auto f = fence->begin(); f != fence->end(); f++)
			{
				(*f)->Map_Check_Hit(ball->Get_Collision_Area());
			}		
			
			//���茋�ʂɖ��ʂȃf�[�^�������Ă���Ȃ��菜��			
			ge->collision_Result.remove_if([](const Collision_Data& c) {return !c.collision_Flag; });
			
			//�{�[�����ړ�������
			ball->Physics_Actioin(precision);
		}
		
		//�J�����ړI�n���{�[��������ʂ�������悤�ɐݒ�
		if (in1.B4.down || ball->Is_Teleport_Now())
		{
			//�@���x�N�g���I��
			ML::Vec3 side_Normal = this->Select_Nearest_Side(side_Normals, (ball->Get_Pos() - ge->Map_center));
			if (!side_Normal.Is_Zero_Vec())
			{
				//�[���x�N�g���łȂ���΃J�������ړ�������
				cm->Set_Destination(side_Normal);
			}
		}
	}
	//�J�����ړ��̖@���x�N�g���I������
	ML::Vec3 Object::Select_Nearest_Side(const std::vector<ML::Vec3>& normals, const ML::Vec3& b)
	{
		//�S�̒��S����{�[���܂ł̃x�N�g���Ɩ@���x�N�g����������ς�����
		float cos[6];		
		for (size_t i =0; i<normals.size(); i++)
		{
			MyMath::Vector_Dot(&cos[i], normals[i], b);
		}

		//�ő�l�����
		size_t max_Num = 0;

		for (size_t i = 1; i < 6; i++)
		{
			if (cos[max_Num] < cos[i])
			{
				max_Num = i;
			}
		}

		return normals.at(max_Num);
	}

	//----------------------------------------------------------------------
	//���͂ɂ�鏈��
	void Object::Input_Analog_Action()
	{
		auto in1 = DI::GPad_GetState("P1");
		if (ge->game.lock()->GET_READY())
		{
			//���͂���Ă鏊��easing�f�[�^�����Z�b�g������
			if (in1.LStick.axis.y > 0.1f)
			{
				easing::Re_Start(this->esasing_Name_QTxm);
			}
			else if (in1.LStick.axis.y < -0.1f)
			{
				easing::Re_Start(this->esasing_Name_QTxp);
			}
			if (in1.LStick.axis.x > 0.1f)
			{
				easing::Re_Start(this->esasing_Name_QTym);
			}
			else if (in1.LStick.axis.x < -0.1f)
			{
				easing::Re_Start(this->esasing_Name_QTyp);
			}
			if (in1.Triger.axis.x <-0.1f)
			{
				easing::Re_Start(this->esasing_Name_QTzm);
			}
			else if (in1.Triger.axis.x >0.1f)
			{
				easing::Re_Start(this->esasing_Name_QTzp);
			}
		}
	}
	//----------------------------------------------------------------------
	//�N�H�[�^�j�I���v�Z
	ML::QT Object::Calculate_Frame_Quatanion(const float& precision)
	{
		//easing�f�[�^����]�ʂōX�V�����
		ML::QT frame_QTxm = ML::QT(this->anckerX, ML::ToRadian(-(easing::GetPos(this->esasing_Name_QTxm)) / precision));

		ML::QT frame_QTxp = ML::QT(this->anckerX, ML::ToRadian((easing::GetPos(this->esasing_Name_QTxp)) / precision));


		ML::QT frame_QTym = ML::QT(this->anckerY, ML::ToRadian(-(easing::GetPos(this->esasing_Name_QTym)) / precision));

		ML::QT frame_QTyp = ML::QT(this->anckerY, ML::ToRadian((easing::GetPos(this->esasing_Name_QTyp)) / precision));


		ML::QT frame_QTzm = ML::QT(this->anckerZ, ML::ToRadian(-(easing::GetPos(this->esasing_Name_QTzm)) / precision));

		ML::QT frame_QTzp = ML::QT(this->anckerZ, ML::ToRadian((easing::GetPos(this->esasing_Name_QTzp)) / precision));

		return frame_QTxp *  frame_QTxm *  frame_QTyp *  frame_QTym * frame_QTzm *  frame_QTzp;
	}
	//------------------------------------------------------------------------
	//��]���Čv�Z
	void Object::Ancker_Calculating()
	{
		//�J��������}�b�v���S�̃x�N�g�����V����Z��]��
		ML::Vec3 NEW_AnckerZ = ge->Map_center - ge->camera[0]->pos;

		//�V�A��Z��]���̖@���x�N�g���ł܂�������Ȃ���]��������1�v�Z
		ML::Vec3 NEW_AnckerY, NEW_AnckerX;
		//�J�����̏�����x�N�g�����V����Y��]��
		NEW_AnckerY = ge->camera[0]->up;
		//X��]���͐V������̎��̖@��
		MyMath::Get_Normal_to_Vector_Cross(&NEW_AnckerX, NEW_AnckerY, NEW_AnckerZ);

		this->anckerX = NEW_AnckerX.Normalize();
		this->anckerY = NEW_AnckerY.Normalize();
		this->anckerZ = NEW_AnckerZ.Normalize();
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