#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include "Task_cursor.h"

enum Difficult_Range
{
	Easy = -1,
	Normal = 1,
	Hard = 3,
};

enum Tutorial_Column
{
	OutLine = -1,
	Control = 1,
	Obstacle = 3,
};

namespace Title
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�^�C�g��");	//�O���[�v��
	const  string  defName("NoName");	//�^�X�N��
	//-------------------------------------------------------------------
	class  Resource
	{
		bool  Initialize();
		bool  Finalize();
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
		//���L����ϐ��͂����ɒǉ�����
		string Title_Name_Image[12];
		string press_Any_Key_Image;
		string Difficalt_Image[3];
		string Column_Image[3];
		string start_Image;
		string tutorial_Image;
		string select_Guide_Image;
		string BG_ImageName;
		string bgm_Title;
	};
	//-------------------------------------------------------------------
	class  Object : public  BTask
	{
	//�ύX�s����������������������������������������������������
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//�������� ������true�Ń^�X�N�V�X�e���֎����o�^
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate();		//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF();	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
		void  Render3D_L0();
		//�ύX������������������������������������������������������
	public:
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		//�ϐ�
		//�^�C�g�������̈ړ�����ѕ`����W
		ML::Vec2 Title_Name[12];

		bool select_now;
		int timeCnt;
		int moving_Menu;
		unsigned int moving_Title_Name;
		POINT next_Task_Index;
		nowMenu n;

		//���\�b�h
		bool Press_Any_Key();//�X�e�B�b�N�ȊO�̃{�^���������̂��𔻕�
		void UpDate_Title_Name();
		void Draw_Title_Name();
		void Draw_PAK();
		void Draw_Menu();
		void Draw_Dif_Col(nowMenu now);

		//���̃^�X�N�����߂�֐�
		void I_Select(POINT select);
	};
}