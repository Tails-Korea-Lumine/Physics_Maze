//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Game.h"
#include  "Task_title.h"
#include  "Task_MapSide.h"
#include  "Task_MapCore.h"
#include  "Task_Ball.h"
#include  "Task_Player.h"
#include  "Task_MapFence.h"
#include  "Task_Map_Manager.h"
#include  "Task_Result.h"
#include  "Task_UI.h"

namespace  Game
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->bgmName = "stageBgm";
		DM::Sound_CreateStream(this->bgmName, "./data/sound/stage.wav");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DM::Sound_Erase(this->bgmName);
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize(Difficult_Range di)
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		this->countdown = 0;
		this->countdownFlag = false;
		this->timeCnt = 0;
		this->nowdi = di;

		ge->gameClearFlag = false;

		//�J�����̐ݒ�
		ge->camera[0] = MyPG::Camera::Create(
			ML::Vec3(1000.0f, 0.0f, 1000.0f),				//	�^�[�Q�b�g�ʒu
			ML::Vec3(1000.0f, 1400.0f, -1000.0f),			//	�J�����ʒu
			ML::Vec3(0.0f, 1.0f, 0.0f),					//	�J�����̏�����x�N�g��
			ML::ToRadian(35), 10.0f, 4000.0f,	//	����p�E���싗��
			(float)ge->screenWidth / (float)ge->screenHeight);		//	��ʔ䗦		
		DG::EffectState().param.bgColor = ML::Color(0.5f, 0.0f, 0.4f, 0.0f);
		//���C�e�B���O�L����
		DG::EffectState().param.lightsEnable = true;
		//�����̋�����ݒ肷��
		DG::EffectState().param.lightAmbient = ML::Color(1, 0.3f, 0.3f, 0.3f);
		//���s�����̐ݒ�
		DG::EffectState().param.light[0].enable = true;
		DG::EffectState().param.light[0].kind = DG_::Light::Directional;//�����̎��
		DG::EffectState().param.light[0].direction = ML::Vec3(1, -1, 1).Normalize();//�Ǝ˕���
		DG::EffectState().param.light[0].color = ML::Color(1, 0.89f, 0.89f, 0.89f);//�F�Ƌ���

		//�f�o�b�O�p�̕�������
		//DG::Font_Create("FontA", "HGS�޼��M", 12, 16);

		//bgm�Đ�
		DM::Sound_Play(this->res->bgmName, true);

		//���[���h��]�ʏ�����
		ge->World_Rotation = ML::QT();

		//���^�X�N�̐���
		//�}�b�v�̒��S�n
		ge->Map_center = ML::Vec3(1050, 50, 1050);
		//����̌��ʂ��[���N���A
		ge->collision_Result.clear();
		//�{�[���̐���
		auto ball = Ball::Object::Create(true);
		//���S�̌��Ȃ�L���[�u
		auto core = Map_Core::Object::Create(true , di);
		//�}�b�v����
		for (int i = 0; i < 6; i++)
		{
			auto map = Map3d::Object::Create(true, i,di);
		}
		//�t�F���X����
		for (int f = 0; f < 12; f++)
		{
			auto fence = MapFence::Object::Create(true, f,di);
		}		

		//�}�b�v�}�l�[�W������
		auto manager = Map_Manager::Object::Create(true);
		
		//UI�̐���
		auto ui = UI::Object::Create(true);

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

		//DG::Font_Erase("FontA");

		DM::Sound_Stop(this->res->bgmName);
		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			//�������p���^�X�N�̐���
			
			auto nextTask = Result::Object::Create(true,this->timeCnt,this->nowdi);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		//auto in = DI::GPad_GetState("P1");

		if (this->IS_Cleared())
		{
			ge->GetTask_One_G<UI::Object>("UI")->Start_WipeIn();
			this->countdownFlag = true;
		}

		//�J�E���g�_�E��
		if (this->Is_Count_Down())
		{
			this->countdown++;
		}
		//1�b��Ƀ^�X�N����
		if (this->countdown > 60)
		{
			this->Kill();
		}
		//���Ԃ��X�V
		this->timeCnt++;
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		/*auto ball = ge->GetTask_One_G<Ball::Object>("�{�[��");
		if (ball == nullptr)
		{
			return;
		}
		char buf[1024];
		sprintf(buf, "pos : %4.3f , %4.3f , %4.3f \n"
			"speed : %4.3f , %4.3f , %4.3f \n"
			"WorldR : %4.3f , %4.3f , %4.3f",
			 ball->Get_Pos().x, ball->Get_Pos().y, ball->Get_Pos().z, ball->Get_Speed().x, ball->Get_Speed().y, ball->Get_Speed().z,
			ge->World_Rotation.x, ge->World_Rotation.y, ge->World_Rotation.z);

		ML::Box2D moji(100, 0, 600, 600);
		DG::Font_Draw("FontA", moji, buf, ML::Color(1, 1, 0, 1));*/
	}

	void  Object::Render3D_L0()
	{
		
	}

	//--------------------------------------------------------------------------------------
	//�ǉ����\�b�h
	//�J�E���g�_�E���t���O��Ԃ��֐�
	bool Object::Is_Count_Down()
	{
		return this->countdownFlag;
	}

	//------------------------------------------------------------------------------------
	//�N���A�t���O�𗧂Ă�
	void Object::Game_Clear()
	{
		ge->gameClearFlag = true;
	}

	//---------------------------------------------------------------------------------------
	//�N���A�����̂����m�F
	bool Object::IS_Cleared()
	{
		return ge->gameClearFlag;
	}
	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_, Difficult_Range di)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
			}
			if (!ob->B_Initialize(di)) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize(Difficult_Range di)
	{
		return  this->Initialize(di);
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