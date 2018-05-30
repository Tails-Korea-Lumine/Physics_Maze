//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_cursor.h"
#include "Task_Title.h"
#include "Task_UI.h"

namespace  Cursor
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imageName = "Cursor";
		DG::Image_Create(this->imageName, "./data/image/cursor.png");

		this->seMoveName = "seCursorMove";
		DM::Sound_CreateSE(this->seMoveName, "./data/sound/Cursor.wav");
		this->seSelectName = "seCursorSelect";
		DM::Sound_CreateSE(this->seSelectName, "./data/sound/CursorSelct.wav");
		this->seCancelName = "seCursorCancel";
		DM::Sound_CreateSE(this->seCancelName, "./data/sound/CursorCencel.wav");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
		DM::Sound_Erase(this->seMoveName);
		DM::Sound_Erase(this->seCancelName);
		DM::Sound_Erase(this->seSelectName);
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
		this->pos = ML::Vec2(300, -32);
		this->select_pos.x = 0;
		this->select_pos.y = -1;
		this->countdown = 0;
		this->countdownFlag = false;
		this->now = Start_Tutorial;

		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���

		auto title = ge->GetTask_One_G <Title::Object>("�^�C�g��");
		if (title == nullptr)
		{
			return false;
		}
		title->I_Select(this->select_pos);


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
		//�J�E���g�_�E���t���O�������Ă��Ȃ��ꍇ�̂ݑI���Ǝ������������s��
		if (!this->Is_Count_Down())
		{
			//�J�[�\���̖ړI�n��ݒ�
			this->destination.y = (float)this->Move_Cursor(this->select_pos);
			//�J�[�\���̑I������
			if (in1.B2.down || in1.ST.down)
			{
				DM::Sound_Play(this->res->seSelectName,false);
				if (this->now == Start_Tutorial)
				{
					this->now = nowMenu(this->select_pos.y);
					this->select_pos.x = this->select_pos.y;
				}
				else
				{
					//UI�^�X�N�ɉ�ʉB���𗊂�
					ge->GetTask_One_G<UI::Object>("UI")->Start_WipeIn();
					this->countdownFlag = true;
				}
			}
			//�J�[�\���̎���������
			if (in1.B1.down)
			{
				this->now = Start_Tutorial;
				this->select_pos.x = 0;
				DM::Sound_Play(this->res->seCancelName, false);
			}
		}
		//�t���O����������J�E���g�_�E���J�n
		else
		{
			this->countdown++;
			this->destination.y = -32;
		}
		//1�b��Ƀ^�X�N����
		if (this->Count_Down_Over())
		{
			this->Kill();
		}
		//25�����ړ�
		this->pos.y += (this->destination.y - this->pos.y) *0.25f;
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw(-16, -16, 32, 32);
		ML::Box2D src(0, 0, 100, 100);

		draw.Offset(this->pos);
		DG::Image_Draw(this->res->imageName, draw, src);
	}

	void  Object::Render3D_L0()
	{
		
	}

	//------------------------------------------------------------------------------------
	//�ǉ����\�b�h
	int Object::Move_Cursor(POINT& select)
	{
		auto in1 = DI::GPad_GetState("P1");
		//�ړ�
		if (in1.LStick.U.down || in1.HU.down)
		{
			//SE�Đ�
			DM::Sound_Play(this->res->seMoveName, false);
			select.y -= 2;
		}
		if (in1.LStick.D.down || in1.HD.down)
		{
			//SE�Đ�
			DM::Sound_Play(this->res->seMoveName, false);
			select.y += 2;
		}
		//���݃��j���|�ɉ�����ړ��͈͐ݒ�
		if (this->now == Start_Tutorial)
		{
			if (select.y < -1)
			{
				select.y = -1;
			}
			else if (this->select_pos.y > 1)
			{
				select.y = 1;
			}
		}
		else
		{
			if (select.y < -1)
			{
				select.y = -1;
			}
			else if (this->select_pos.y > 3)
			{
				select.y = 3;
			}
		}
		
		return 408 + (select.y * 43);
	}

	//---------------------------------------------------------------------------------
	//���݃��j���[��Ԃ��֐�
	nowMenu Object::Get_Now_Menu()
	{
		return this->now;
	}

	//---------------------------------------------------------------------------------
	//�J�E���g�_�E���t���O��Ԃ��֐�
	bool Object::Is_Count_Down()
	{
		return this->countdownFlag;
	}
	//--------------------------------------------------------------------------
	//��ʐ؂�ւ����I������̂����m�F
	bool Object::Count_Down_Over()
	{
		return (this->countdown > 130);
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