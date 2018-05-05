//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Result.h"
#include  "Task_Title.h"
#include  "Task_UI.h"

namespace  Result
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->thisTime = "thisTime";
		this->yourScore = "yourScore";
		this->result = "result";
		this->Number_Image = "resultNumber";
		this->presS = "pressS";

		this->bgmName = "ResultBgmName";

		DG::Image_Create(this->thisTime, "./data/image/thisTime.png");
		DG::Image_Create(this->yourScore, "./data/image/yourScore.png");
		DG::Image_Create(this->result, "./data/image/result.png");
		DG::Image_Create(this->Number_Image, "./data/image/Number.png");
		DG::Image_Create(this->presS, "./data/image/pressS.png");

		DM::Sound_CreateStream(this->bgmName, "./data/sound/ending.wav");

		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->thisTime);
		DG::Image_Erase(this->yourScore);
		DG::Image_Erase(this->result);
		DG::Image_Erase(this->Number_Image);

		DM::Sound_Erase(this->bgmName);
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize(int playTime)
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		this->countdown = 0;
		this->countdownFlag = false;
		//bgm�Đ�
		DM::Sound_Play(this->res->bgmName, true);

		//�_���v�Z�� : 1000-�v���[����(�b�P��)
		this->score = 1000-(playTime/60);
		this->timeCnt = 0;
		for (int i = 0; i < 10; i++)
		{
			this->src_Number[i].x = (140 * i);
			this->src_Number[i].y = 0;
			this->src_Number[i].w = 140;
			this->src_Number[i].h = 140;
		}

		//�J�����̐ݒ�
		
		DG::EffectState().param.bgColor = ML::Color(1, 0, 0, 0);
		//���^�X�N�̐���

		UI::Object::Create(true);

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->KillAll_G("UI");

		DM::Sound_Stop(this->res->bgmName);

		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			//�������p���^�X�N�̐���
			auto NextTask = Title::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto in = DI::GPad_GetState("P1");
		//�J�E���g�_�E���t���O�𗧂Ă�
		if (in.ST.down)
		{
			this->countdownFlag = true;
			ge->GetTask_One_G<UI::Object>("UI")->Start_WipeIn();
			this->timeCnt = 300;
		}
		//�t���O�������Ă���ԂɃJ�E���g�_�E������
		if (this->Is_Count_Down())
		{
			this->countdown++;
		}
		//1�b��Ƀ^�X�N����
		if (this->countdown > 60)
		{
			this->Kill();
		}
		

		this->timeCnt++;
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//result
		ML::Box2D draw_Result(30, 30, 400, 100);
		ML::Box2D src_Result(0, 0, 500, 140);
		DG::Image_Draw(this->res->result, draw_Result, src_Result);
		//this time
		if (this->timeCnt > 60)
		{
			ML::Box2D draw_ThisTime(500, 70, 700, 140);
			ML::Box2D src_ThisTime(0, 0, 700, 140);
			DG::Image_Draw(this->res->thisTime, draw_ThisTime, src_ThisTime);
		}
		//your score
		if (this->timeCnt > 120)
		{
			ML::Box2D draw_YourScore(450, 300, 770, 140);
			ML::Box2D src_YourScore(0, 0, 770, 140);
			DG::Image_Draw(this->res->yourScore, draw_YourScore, src_YourScore);
		}
		//�X�R�A�[�\��
		if (this->timeCnt > 180)
		{
			this->Draw_Score();
		}
		//press S to return title
		if (this->timeCnt > 240)
		{
			ML::Box2D draw_pressS(200, 650, 700, 70);
			ML::Box2D src_pressS(0, 0, 1400, 140);
			DG::Image_Draw(this->res->presS, draw_pressS, src_pressS);
		}

	}

	void  Object::Render3D_L0()
	{
		
	}

	//------------------------------------------------------------------------------------
	//�ǉ����\�b�h
	void Object::Draw_Score()
	{
		int score1000 = 0;
		int score100 = 0;
		int score10 = 0;
		int score1 = 0;
		//�_�����[�Ȃ�O�_�ŕ\������
		if (this->score > 0)
		{
			score1000 = this->score / 1000;
			score100 = (this->score / 100) % 10;
			score10 = (this->score / 10) % 10;
			score1 = this->score % 10;
		}


		ML::Box2D draw1000(400, 450, 140, 140);
		ML::Box2D draw100(520, 450, 140, 140);
		ML::Box2D draw10(640, 450, 140, 140);
		ML::Box2D draw1(760, 450, 140, 140);

		//�`��
		DG::Image_Draw(this->res->Number_Image, draw1000, this->src_Number[score1000]);
		DG::Image_Draw(this->res->Number_Image, draw100, this->src_Number[score100]);
		DG::Image_Draw(this->res->Number_Image, draw10, this->src_Number[score10]);
		DG::Image_Draw(this->res->Number_Image, draw1, this->src_Number[score1]);
	}

	//----------------------------------------------------------------------------------
	//�J�E���^�_�E���t���O��Ԃ��֐�
	bool Object::Is_Count_Down()
	{
		return this->countdownFlag;
	}
	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_, int playTime)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
			}
			if (!ob->B_Initialize(playTime)) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize(int playTime)
	{
		return  this->Initialize(playTime);
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