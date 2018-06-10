//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Effect_Manager.h"
#include  "easing.h"

namespace  EffectManager
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
		//�g�p����G�t�F�N�g���b�V���̐���
		//�e���|�[�g�C��
		DG::Mesh_CreateFromSOBFile("DestroyItem", "./data/mesh/effect/DestroyItem.SOB");
		//�e���|�[�g�A�E�g
		DG::Mesh_CreateFromSOBFile("CreateItem", "./data/mesh/effect/CreateItem.SOB");
		//�Q�[���N���A
		DG::Mesh_CreateFromSOBFile("DestroyCharactor", "./data/mesh/effect/DestroyPlayer.SOB");
		//�G�t�F�N�g���X�g���N���A
		this->efList.clear();

		this->render3D_Priority[0] = 1.0f;
		//�_�~�[�f�[�^
		Effect* dummmy0 = new Effect;
		dummmy0->Set_Dummy();

		this->efList.push_back(dummmy0);

		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		//���X�g�̊J��
		DG::Mesh_Clear();
		if (this->efList.size() != 0)
		{
			for (auto i : this->efList)
			{
				i->Finalize();
			}
		}
		this->efList.clear();
		/*for (auto it = this->efList.begin(); it != this->efList.end(); it++)
		{
			if ((*it) != nullptr)
			{
				delete (*it);
			}
		}*/
		
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

		if (this->efList.size() > 60)
		{
			this->efList.pop_front();
		}
		//�G�t�F�N�g�̃t���[�����̕ω�
		for (auto it = this->efList.begin(); it != this->efList.end(); it++)
		{
			if ((*it) == nullptr)
			{
				continue;
			}
			(*it)->UpDate_Effect((*it)->Get_Type());
		}
		this->Dec_Effect_Life();
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
	}

	void  Object::Render3D_L0()
	{
		if (this->efList.size() <= 1)
		{
			return;
		}
		//�G�t�F�N�g�̍Đ�
		for (auto it = this->efList.begin(); it != this->efList.end(); it++)
		{
			if ((*it) == nullptr)
			{
				continue;
			}
			(*it)->Playing_Effect((*it)->Get_Type());
		}
	}

	//--------------------------------------------------------------------------------------
	//�ǉ����\�b�h
	//-----------------------------------------------------------------------------
	void Object::Add_Effect(ML::Vec3 pos, ML::Vec3 angle, BEffect::effType handle)
	{
		Effect* NewEF = new Effect();
		NewEF->Load_Eff(pos, angle, handle);

		this->efList.push_back(NewEF);
	}
	void Object::Add_Effect(ML::Vec3 pos, ML::Vec3 target, ML::Vec3 angle, BEffect::effType handle)
	{
		Effect* NewEF = new Effect();
		NewEF->Load_Eff(pos, target, angle, handle);

		this->efList.push_back(NewEF);
	}
	void Object::Dec_Effect_Life()
	{
		if (this->efList.size() <= 1)
		{
			return;
		}
		for (auto it = this->efList.begin(); it != this->efList.end(); it++)
		{
			if ((*it)->Is_Alive())
			{
				(*it)->Dec_Eff();
			}
			//���C�t���O�ɂȂ����G�t�F�N�g�̓q-�v����J��
			else if((*it)->Get_Type() != BEffect::effType::CLEAR)
			{
				(*it)->Finalize();				
			}
		}
		//���C�t���O�ɂȂ����G�t�F�N�g�̓��X�g����O��
		//return it->effect_Life == 0
		this->efList.remove_if(Effect::Eff_Judge);
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