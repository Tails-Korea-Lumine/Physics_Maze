//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Effect_Manager.h"
#include  "easing.h"
#include "Game_Clear.h"
#include "Teleport_In.h"
#include "Teleport_Out.h"
#include "Breaking.h"
#include "Crashed.h"

namespace  EffectManager
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		//�g�p����G�t�F�N�g���b�V���̐���
		//�e���|�[�g�C��
		this-> mesh_Telein = "Teleport_In";
		DG::Mesh_CreateFromSOBFile(mesh_Telein, "./data/mesh/effect/DestroyItem.SOB");
		//�e���|�[�g�A�E�g
		this-> mesh_Teleout = "Teleport_Out";
		DG::Mesh_CreateFromSOBFile(mesh_Teleout, "./data/mesh/effect/CreateItem.SOB");
		//�Q�[���N���A
		this-> mesh_Gameclear = "Game_Clear";
		DG::Mesh_CreateFromSOBFile(mesh_Gameclear, "./data/mesh/effect/DestroyPlayer.SOB");
		//�u���C�L���O
		this->mesh_Breaking = "Breaking";
		DG::Mesh_CreateFromSOBFile(this->mesh_Breaking, "./data/mesh/effect/Breaking.SOB");
		//�N���b�V��
		this->mesh_Crashed = "Crashed";
		DG::Mesh_CreateFromSOBFile(this->mesh_Crashed, "./data/mesh/effect/Crashed.SOB");

		//�T�E���h�G�t�F�N�g
		//�e���|�[�g�C��
		this-> seTeleportIn = "SETeleIn";
		DM::Sound_CreateSE(seTeleportIn, "./data/sound/TeleportIn.wav");
		//�e���|�[�g�A�E�g
		this-> seTeleportOut = "SETeleOut";
		DM::Sound_CreateSE(seTeleportOut, "./data/sound/TeleportOut.wav");
		//�u���C�L���O
		this->se_Breaking = "SE_Breaking";
		DM::Sound_CreateSE(this->se_Breaking, "./data/sound/Breaking_Box.wav");
		//�N���b�V��
		this->se_Crashed = "SE_Crashed";
		DM::Sound_CreateSE(this->se_Crashed, "./data/sound/Crashed.wav");

		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Mesh_Erase(this->mesh_Telein);
		DG::Mesh_Erase(this->mesh_Gameclear);
		DG::Mesh_Erase(this->mesh_Teleout);
		DG::Mesh_Erase(this->mesh_Breaking);
		DG::Mesh_Erase(this->mesh_Crashed);
		DM::Sound_Erase(seTeleportIn);
		DM::Sound_Erase(seTeleportOut);
		DM::Sound_Erase(this->se_Breaking);
		DM::Sound_Erase(this->se_Crashed);
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
		//�G�t�F�N�g���X�g���N���A
		this->play_Effect_List.clear();

		this->render3D_Priority[0] = 1.0f;		

		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���		
		if (this->play_Effect_List.size() != 0)		
		{
			//���X�g�̊J��
			for (auto e : this->play_Effect_List)
			{
				delete e;
			}
		}
		this->play_Effect_List.clear();
		
		
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
		//easing function update
		easing::UpDate();
		
		//�G�t�F�N�g�̃t���[�����̕ω�
		for (auto it = this->play_Effect_List.begin(); it != this->play_Effect_List.end(); it++)
		{			
			(*it)->Effect_Update();
		}
		//��������
		this->Dec_Effect_Life();
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
	}

	void  Object::Render3D_L0()
	{
		//�G�t�F�N�g�̍Đ�
		for (auto it = this->play_Effect_List.begin(); it != this->play_Effect_List.end(); it++)
		{			
			(*it)->Effect_Draw();
		}
	}

	//--------------------------------------------------------------------------------------
	//�ǉ����\�b�h
	//-----------------------------------------------------------------------------
	void Object::Add_Effect(const ML::Vec3& pos, const ML::Vec3& angle, const BEffect::effType& handle)
	{	
		BEffect* ef;
		//�n���h���ɍ����G�t�F�N�g�𐶐����ă��X�g�ɓo�^
		switch (handle)
		{
		case BEffect::effType::Game_Clear:
			ef = new Game_Clear(pos, angle, this->res->mesh_Gameclear, "");
			break;
		case BEffect::effType::TeleportOut:
			ef = new Teleport_Out(pos, angle, this->res->mesh_Teleout, this->res->seTeleportOut);
			break;
		case BEffect::effType::Breaking:
			ef = new Breaking(pos, angle, this->res->mesh_Breaking, this->res->se_Breaking);
			break;
		case BEffect::effType::Crashed:
			ef = new Crashed(pos, angle, this->res->mesh_Crashed, this->res->se_Crashed);
			break;
		//�����ɍ����ĂȂ��G�t�F�N�g�n���h���͏������Ȃ�
		default:
			return;
		}
		//�v���C���X�g��push_back
		this->play_Effect_List.push_back(ef);
	}
	void Object::Add_Effect(const ML::Vec3& pos, const ML::Vec3& target, const ML::Vec3& angle, const BEffect::effType& handle)
	{
		BEffect* ef;
		//�n���h���ɍ����G�t�F�N�g�𐶐����ă��X�g�ɓo�^
		switch (handle)
		{		
		case BEffect::effType::Teleportin:
			ef = new Teleport_In(pos, target, angle, this->res->mesh_Telein, this->res->seTeleportIn);
			break;
			//�����ɍ����ĂȂ��G�t�F�N�g�n���h���͏������Ȃ�
		default:
			return;
		}
		//�v���C���X�g��push_back
		this->play_Effect_List.push_back(ef);
	}
	//-----------------------------------------------------------------------------------	
	//�G�t�F�N�g�������炵�y�э폜
	void Object::Dec_Effect_Life()
	{
		if (this->play_Effect_List.empty())
		{
			return;
		}
		for (auto it = this->play_Effect_List.begin(); it != this->play_Effect_List.end(); it++)
		{
			//��������
			(*it)->Dec_Eff();
			//���C�t���O�ɂȂ����G�t�F�N�g�͊J������
			if ((*it)->Is_Alive() == false)
			{
				delete (*it);
				(*it) = nullptr;
			}
		}
		//������ꂽ���̂̓��X�g����O��
		this->play_Effect_List.remove_if([](BEffect* e) {return e == nullptr; });
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