//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Ball.h"
#include  "Task_MapSide.h"
#include  "Task_MapCore.h"

namespace  Ball
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->meshName = "Item";

		DG::Mesh_CreateFromSOBFile(this->meshName, "./data/mesh/Item.SOB");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{		
		DG::Mesh_Erase(this->meshName);
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
		this->pos = ML::Vec3(1050, 500, 900);//���̈ʒu��Œ�����������(2018/04/20)
		this->speed = ML::Vec3(0, 0, 0);
		this->r = 20.0f;
		this->m = 10.0f;
		this->collision_Flag = false;

	
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
			
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto in1 = DI::GPad_GetState("P1");		

		//�}�b�v�̏����C���A���̓^�X�N��Ŏ����Ă��邪
		//���Vector�Ŏ����Ă���悤��������(2018/03/27)
		auto map = ge->GetTask_Group_GN<Map3d::Object>("�}�b�v","Side");		
		auto core = ge->GetTask_One_G<Map_Core::Object>("�}�b�v");
		//�}�b�v�̂����蔻��̌��ʂ�ۑ�����
		std::vector<After_Collision> Result;
		Result = core->Get_Collision_Poligon();
		for (auto i = map->begin(); i != map->end(); i++)
		{
			for (auto it : (*i)->Get_Collision_Poligon())
			{
				Result.push_back(it);
			}			
		}

		//-------------------------------------
		//�f�o�b�O�p�|�[�Y
		if (in1.B2.down)
		{
			system("pause");
		}
		//-------------------------------------

		//�d�͉���
		this->speed += this->G.Accelerate(this->m);

		//����,�ǂ��������蔻��������ɓ������ꍇ
		//���������Ɏ��̃t���[���Ɉڂ�
		if (Result.size() == 0)
		{	
			//�ړ�(�t���[���I�����钼�O�ɍs��)
			this->pos += this->speed;
			return;
		}

		//�O��t���[���̂����蔻�茋�ʂ��m�F
		if (this->Is_Collision())
		{
			//����t���[���ŏՓ˂����������Ƃ��m�F����t���O
			bool cnt = false;

			//���ʂ̐������[�v����
			for (auto p : Result)
			{				
				//�O�̃t���[���ŏՓ˂�������A����̃t���[���ł̏Փ˔���ł�邱��
				if (p.collision_Flag)
				{
					//����̃t���[���ɏՓ˂�������
					//�΂ߐ�����������
					this->speed = this->G.CollisionOver_Accelerate(this->speed, p.normal,this->m);
					//�t���O�𗧂Ă�
					cnt = true;
					this->collision_Flag = true;
					//�ǂ����ł������Ă���Ƃ��̉�]����
					if (in1.LStick.volume != 0)
					{
						//�}�b�v�̃t���[����]�ʂŉ�]������
						ML::Mat4x4 matR;
						D3DXMatrixAffineTransformation(&matR, 1, &ge->Map_center, &core->Get_Frame_QT(), NULL);
						
						this->pos = matR.TransformCoord(this->pos);
					}

				}
				else if(cnt == false)
				{
					//����̃t���[���ɏՓ˂��Ȃ�������
					//�Փ˃t���O�𖳌��ɂ���
					this->collision_Flag = false;
				}
			}
		}
		else
		{
			//���ʂ̐������[�v����
			for (auto p : Result)
			{
				//�O�̃t���[���ŏՓ˂ł͂Ȃ�������A����̃t���[���ł̏Փ˔���ł�邱��			
				if (p.collision_Flag)
				{
					//����̃t���[���ɏՓ˂�������
					//���ˊp�Œ��˕Ԃ�

					this->speed = this->G.Reflaction_Vector(this->speed, p.normal, this->m);

					//�Փˎx�X�̎��̉�]�����e�X�g
					if (in1.LStick.volume != 0)
					{
						//�}�b�v�̃t���[����]�ʂŉ�]������
						ML::Mat4x4 matR;
						D3DXMatrixAffineTransformation(&matR, 1, &ge->Map_center, &core->Get_Frame_QT(), NULL);

						this->speed = matR.TransformNormal(this->speed);
					}

					//�Փ˃t���O��L���ɂ���
					this->collision_Flag = true;
				}
			}
		}		
		
		//�ړ�(�t���[���I�����钼�O�ɍs��)
		this->pos += this->speed;
		
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{		
	}

	void  Object::Render3D_L0()
	{
		ML::Mat4x4 matT;
		ML::Mat4x4 matS;
		matT.Translation(this->pos);
		matS.Scaling(this->r);
		DG::EffectState().param.matWorld = matS * matT;
		DG::Mesh_Draw(this->res->meshName);
	}

	//--------------------------------------------------------------------
	//�ǉ����\�b�h

	bool Object::Is_Collision()
	{
		return this->collision_Flag;
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