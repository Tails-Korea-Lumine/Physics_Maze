//-------------------------------------------------------------------
//�J�����}��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_CameraMan.h"

namespace  CameraMan
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
		//�J�����̏����ݒ��Task_Game�̕��ōs��

		this->nowPos = ge->camera[0]->pos;
		this->initPos = ge->camera[0]->pos;
		this->distance = 2750.0f;
		this->angle = ML::Vec3(0, 0, 0);
		this->maxAngle = 30;
		this->timeCnt = 0;


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

		this->nowPos = ge->Map_center - ML::Vec3(0, 0, this->distance);

		//Rstick�̓����ŃJ�������ړ�
		if (in1.RStick.axis.x != 0)
		{
			this->angle.x += in1.RStick.axis.x * 3;
		}
		if (in1.RStick.axis.y != 0)
		{
			this->angle.y -= in1.RStick.axis.y * 3;
		}

		//�ړ��͈͐ݒ�
		if (this->angle.x < -this->maxAngle)
		{
			this->angle.x = -this->maxAngle;
		}
		else if (this->angle.x > this->maxAngle)
		{
			this->angle.x = this->maxAngle;
		}
		if (this->angle.y < -this->maxAngle)
		{
			this->angle.y = -this->maxAngle;
		}
		else if (this->angle.y > this->maxAngle)
		{
			this->angle.y = this->maxAngle;
		}

		//��]�N�H�[�^�j�I������
		ML::QT rotationY = ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(this->angle.x));
		ML::QT rotationX = ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(this->angle.y));

		//�A�t�B���ϊ�
		ML::Mat4x4 matR;
		D3DXMatrixAffineTransformation(&matR, 1, &ge->Map_center, &(rotationX*rotationY), NULL);

		this->nowPos = matR.TransformCoord(this->nowPos);

		//�J�����ʒu�̍X�V
		ge->camera[0]->pos = this->nowPos;

		//���C�g���I�t����Ă���3�b��ɁA���C�g���I������
		if (this->It_Passed_3sec())
		{
			DG::EffectState().param.light[0].enable = true;
			DG::EffectState().param.lightAmbient = ML::Color(1, 0.3f, 0.3f, 0.3f);
		}
		this->timeCnt++;
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{		
	}

	void  Object::Render3D_L0()
	{
		
	}

	//---------------------------------------------------------------------
	//�ǉ����\�b�h
	//���C�g�I�t
	void Object::Turnoff_the_Light()
	{
		DG::EffectState().param.light[0].enable = true;
		DG::EffectState().param.lightAmbient = ML::Color(1, 0, 0, 0);
		this->timeCnt = 0;
	}
	//-----------------------------------------------------------------
	//3�b���������̂����m�F
	bool Object::It_Passed_3sec()
	{
		if (this->timeCnt > 180)
		{
			return true;
		}
		return false;
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