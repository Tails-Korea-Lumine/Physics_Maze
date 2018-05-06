#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//���ʉ��
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include  "Task_Title.h"

namespace Result
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("���ʉ��");	//�O���[�v��
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
		string result;
		string thisTime;
		string yourScore;
		string Number_Image;
		string presS;
		string bgmName;
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
		static  Object::SP  Create(bool flagGameEnginePushBack_,int playTime, Difficult_Range di);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize(int playTime, Difficult_Range di);
		bool  B_Finalize();
		bool  Initialize(int playTime, Difficult_Range di);	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate();		//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF();	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
		void  Render3D_L0();
		//�ύX������������������������������������������������������
	
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		//�ϐ�
		int score;
		int timeCnt;
		ML::Box2D src_Number[10];
		//�^�X�N���ł܂ł̃J�E���^�ƃt���O
		int countdown;
		bool countdownFlag;
	public:
		//���\�b�h
		void Draw_Score();
		bool Is_Count_Down();
	};
}