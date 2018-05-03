#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�}�b�v
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include  "Collision.h"
#include "Bbox.h"
#include "Task_title.h"



namespace Map3d
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�}�b�v");	//�O���[�v��
	const  string  defName("Side");	//�^�X�N��
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
	};
	//-------------------------------------------------------------------
	class  Object : public BTask
	{
	//�ύX�s����������������������������������������������������
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//�������� ������true�Ń^�X�N�V�X�e���֎����o�^
		static  Object::SP  Create(bool flagGameEnginePushBack_,int sideNum, Difficult_Range di);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize(int sideNum, Difficult_Range di);
		bool  B_Finalize();
		bool  Initialize(int sideNum, Difficult_Range di);	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate();		//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF();	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
		void  Render3D_L0();
		//�ύX������������������������������������������������������
	
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����	
		//�ϐ�
		Bbox arr[16][1][16];
		int sizeZ, sizeY, sizeX;
		int mapSize;
		int sideNumber;
		float chipSize;
		string chipName[10];		
		//�{�[���̂ق��ɓn�����
		std::vector< After_Collision> col_Poligons;
		//��]��
		ML::QT map_QT;
		ML::QT frame_QT;
		
	public:
		//���\�b�h
		bool Map_Load(string f_);
		void Map_Check_Hit(const ML::Vec3& pos, const float& r, const ML::Vec3& speed);//���ƃ}�b�v�̂����蔻��
		void Map_Rotate();
		
		std::vector<After_Collision> Get_Collision_Poligon();

		//�N�H�[�^�j�I�����X�V����֐�
		void UpDate_Quartanion(ML::QT qt);
		
	};
}