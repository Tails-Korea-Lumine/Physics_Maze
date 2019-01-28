//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Tutorial.h"
#include  "Task_Title.h"
#include "Task_UI.h"
#include "Task_Effect_Manager.h"
#include "Task_Physics_Manager.h"
#include "Task_MapCore.h"
#include "Task_MapFence.h"
#include "Task_MapSide.h"
#include "Task_Ball.h"
#include "Task_CameraMan.h"
#include "easing.h"

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

		this->guide_Img[0] = "page_Guide";
		this->guide_Img[1] = "press_S";

		DG::Image_Create(this->guide_Img[0], "./data/image/pageGuide.png");
		DG::Image_Create(this->guide_Img[1], "./data/image/pressS.png");

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
		for (int i = 0; i < 2; i++)
		{
			DG::Image_Erase(this->guide_Img[i]);
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

		this->countDown = 0;
		this->countDown_Flag = false;

		this->timeCnt = 990;//0~80�ł͂Ȃ����l�ŏ�����
		this->page_Change_Flag = false;
		this->page_Change_Speed = (int)ge->screenWidth / 80;

		this->vol = 1000;
		//�}�b�v�̒��S�n
		ge->Map_center = ML::Vec3(1050, 50, 1050);
		//����̌��ʂ��[���N���A
		ge->collision_Result.clear();
		//�O������e���[�g���A�����e��ǂݍ��ޏ���
		switch(this->column)
		{
		case Tutorial_Column::OutLine:
			//������
			break;
		case Tutorial_Column::Control:
			//������
			break;
		case Tutorial_Column::Obstacle:
			//������
			break;
		}

		DM::Sound_Play(this->res->bgmName, true);
			
		DG::EffectState().param.bgColor = ML::Color(1, 0, 0, 0);
		//���^�X�N�̐���
		//ui
		auto ui = UI::Object::Create(true);
		//�J����
		auto camera = CameraMan::Object::Create(true);
		//�{�[��
		auto ball = Ball::Object::Create(true);
		//���S�L���[�u
		auto core = Map_Core::Object::Create(true,Difficult_Range::TUTORIAL);
		//��
		for (int i = 0; i < 6; i++)
		{
			auto side = Map_Side::Object::Create(true, i, Difficult_Range::TUTORIAL);
		}
		//�p
		for (int f = 0; f < 12; f++)
		{
			auto fence = MapFence::Object::Create(true, f, Difficult_Range::TUTORIAL);
		}
		//�}�l�[�W��
		auto ma = Physics_Manager::Object::Create(true);
		//�G�t�F�N�g
		auto effect = EffectManager::Object::Create(true);

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���		
		ge->KillAll_G("�}�b�v");
		ge->KillAll_G("�{�[��");
		ge->KillAll_G("UI");
		ge->KillAll_G("�J�����}��");
		ge->KillAll_G("�G�t�F�N�g");
		ge->KillAll_G("�����^��");

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
		//�J�E���g�_�E��
		if (this->Is_Count_Down())
		{
			this->countDown++;
		}
		//�J�E���g�_�E�����I������玩��������
		if (this->Count_Down_Over())
		{
			this->Kill();
		}
		//���ԏ㏸
		this->timeCnt += ge->g_Time.Delta_Time();
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{

	}

	void  Object::Render3D_L0()
	{
		
	}

	//-----------------------------------------------------------------------------------
	//�y�[�W�؂�ւ�
	void Object::Page_Chage(bool page_Move_Right)
	{		
		if (this->Can_I_Change_the_Page())
		{
			return;
		}

		//�E�̃y�[�W������
		if (page_Move_Right)
		{
			if (this->posx[2] <= 0)
			{
				return;
			}
			for (int i = 0; i < 3; i++)
			{
				this->posx[i] -= this->page_Change_Speed;
			}
		}
		//���̃y�[�W������
		else
		{
			if (this->posx[0] >= 0)
			{
				return;
			}
			for (int i = 0; i < 3; i++)
			{
				this->posx[i] += this->page_Change_Speed;
			}
		}
		
	}

	//-----------------------------------------------------------------------------
	//�������ł܂ł̃J�E���g�_�E���J�n���m�F
	bool Object::Is_Count_Down() const
	{
		return this->countDown_Flag;
	}

	//-------------------------------------------------------------------------------
	//�J�E���g�_�E�����I������̂����m�F
	bool Object::Count_Down_Over() const
	{
		return (this->countDown > 60);
	}
	//----------------------------------------------------------------------------------
	//�y�[�W�؂�ւ����\���𔻒f
	bool Object::Can_I_Change_the_Page() const
	{
		return (this->timeCnt > 80);
	}
	//--------------------------------------------------------------------------------
	//BGM fade out
	void Object::BGM_Fade_Out()
	{
		this->vol -= 5;
		DM::Sound_Volume(this->res->bgmName, this->vol);
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