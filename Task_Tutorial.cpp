//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Tutorial.h"
#include  "Task_Title.h"

namespace  Tutorial
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imageName[0] = "Tutorial_Outline_Img";
		this->imageName[1] = "Tutorial_Control_Img";
		this->imageName[2] = "Tutorial_Obstacle_Img";
		DG::Image_Create(this->imageName[0], "./data/image/tutorial_text.png");//���̃C���[�W
		DG::Image_Create(this->imageName[1], "./data/image/tutorial_text.png");//���̃C���[�W
		DG::Image_Create(this->imageName[2], "./data/image/tutorial_text.png");//���̃C���[�W
	
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		for (int i = 0; i < 3; i++)
		{
			DG::Image_Erase(this->imageName[i]);
		}
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize(Tutorial_Column tc)
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		this->posy = 0;
		this->column = Tutorial_Column::OutLine;
			
		DG::EffectState().param.bgColor = ML::Color(1, 0, 0, 0);
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
			auto nextTask = Title::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto in = DI::GPad_GetState("P1");

		if (in.LStick.D.on || in.HD.on)
		{
			this->posy -= 2;
		}
		if (in.LStick.U.on || in.HU.on)
		{
			this->posy += 2;
		}

		//posy�͈̔͐ݒ�
		if (this->posy < -528)
		{
			this->posy = -528;
		}
		if (this->posy > 0)
		{
			this->posy = 0;
		}

		if (in.ST.down)
		{
			if (in.ST.down)
			{
				//���g�ɏ��ŗv��
				this->Kill();
			}
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw(0, 0, 1280, 1248);
		ML::Box2D src(0, 0,1280, 1248);

		draw.Offset(0, this->posy);
		switch (this->column)
		{
		case Tutorial_Column::OutLine:
			DG::Image_Draw(this->res->imageName[0], draw, src);
			break;
		case Tutorial_Column::Control:
			DG::Image_Draw(this->res->imageName[1], draw, src);
			break;
		case Tutorial_Column::Obstacle:
			DG::Image_Draw(this->res->imageName[2], draw, src);
			break;
		}
		
	}

	void  Object::Render3D_L0()
	{
		
	}
	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_, Tutorial_Column tc)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
			}
			if (!ob->B_Initialize(tc)) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize(Tutorial_Column tc)
	{
		return  this->Initialize(tc);
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