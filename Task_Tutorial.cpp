//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Tutorial.h"
#include  "Task_Title.h"
#include  "easing.h"

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
		this->Bg_Img = "Tutorial_BG";
		DG::Image_Create(this->imageName[0], "./data/image/Tutorial_Outline.png");//�T�v�̃e���[�g���A��
		DG::Image_Create(this->imageName[1], "./data/image/Tutorial_Control.png");//����̃e���[�g���A��
		DG::Image_Create(this->imageName[2], "./data/image/Tutorial_Obstacle.png");//��Q���̃e���[�g���A��
		DG::Image_Create(this->Bg_Img, "./data/image/background.jpg");

		this->bgmName = "tutorialBGM";
		DM::Sound_CreateStream(this->bgmName, "./data/sound/tutorial.wav");
	
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
		DG::Image_Erase(this->Bg_Img);
		DM::Sound_Erase(this->bgmName);
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
		this->column = tc;

		this->timeCnt = 990;//0~80�ł͂Ȃ����l�ŏ�����
		this->page_Change_Flag = false;

		switch(this->column)
		{
		case Tutorial_Column::OutLine:
			for (int i = 0; i < 3; i++)
			{
				this->posx[i] = ge->screenWidth * i;
				//easing::Set("Change_Page" + to_string(i), easing::CIRCIN, ge->screenWidth*i, ge->screenWidth*(i+1), 80);
			}
			break;
		case Tutorial_Column::Control:
			for (int i = 0; i < 3; i++)
			{
				this->posx[i] = ge->screenWidth * (i-1);
				//easing::Set("Change_Page" + to_string(i), easing::CIRCIN, ge->screenWidth*(i-1), ge->screenWidth*(i), 80);
			}
			break;
		case Tutorial_Column::Obstacle:
			for (int i = 0; i < 3; i++)
			{
				this->posx[i] = ge->screenWidth * (i-2);
				//easing::Set("Change_Page" + to_string(i), easing::CIRCIN, ge->screenWidth*(i-2), ge->screenWidth*(i - 1), 80);
			}
			break;
		}

		DM::Sound_Play(this->res->bgmName, true);
			
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

		if (in.LStick.L.down || in.HL.down)
		{
			if (this->Can_I_Change_the_Page())
			{
				this->page_Change_Flag = false;
				this->timeCnt = 0;
				this->posy = 0;
			}
		}
		else if (in.LStick.R.down || in.HR.down)
		{
			if (this->Can_I_Change_the_Page())
			{
				this->page_Change_Flag = true;
				this->timeCnt = 0;
				this->posy = 0;
			}
		}

		this->Page_Chage(this->page_Change_Flag);

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
		this->timeCnt++;
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D drawBG(0, 0, 1280, 720);
		ML::Box2D srcBG(0, 0, 1280, 960);

		DG::Image_Draw(this->res->Bg_Img, drawBG, srcBG);

		ML::Box2D draw[3] = 
		{
			{0, 0, 1280, 1248},
			{ 0, 0, 1280, 1248 },
			{ 0, 0, 1280, 1248 },		
		};
		ML::Box2D src(0, 0,1280, 1248);
		for (int i = 0; i < 3; i++)
		{
			draw[i].Offset(this->posx[i], this->posy);
			DG::Image_Draw(this->res->imageName[i], draw[i], src);
		}
		
		/*DG::Image_Draw(this->res->imageName[1], draw, src);
		DG::Image_Draw(this->res->imageName[2], draw, src);*/
		
		
	}

	void  Object::Render3D_L0()
	{
		
	}

	//-----------------------------------------------------------------------------------
	//�y�[�W�؂�ւ�
	void Object::Page_Chage(bool page_Move_Right)
	{
		if (this->timeCnt > 80 )
		{
			return;
		}

		//�E�̃y�[�W������
		if (page_Move_Right)
		{
			if (this->posx[2] == 0)
			{
				return;
			}
			for (int i = 0; i < 3; i++)
			{
				this->posx[i] -= 16;
			}
		}
		//���̃y�[�W������
		else
		{
			if (this->posx[0] == 0)
			{
				return;
			}
			for (int i = 0; i < 3; i++)
			{
				this->posx[i] += 16;
			}
		}
		
	}

	//----------------------------------------------------------------------------------
	//�y�[�W�؂�ւ����\���𔻒f
	bool Object::Can_I_Change_the_Page()
	{
		return (this->timeCnt > 80);
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