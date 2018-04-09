#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�}�b�v
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include  "Collision.h"

struct mapData
{
	int chip;//�}�X�̃}�b�v�`�b�v
	ML::Vec3 pos;//���S�_�̍��W
	ML::Box3D collision_Base;//���̔���͈�
};

namespace Map3d
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�}�b�v");	//�O���[�v��
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
	
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		//�ϐ�
		mapData arr[30][30][30];
		int sizeZ, sizeY, sizeX;
		float chipSize;
		ML::Box3D hitBase;//�}�b�v�S�̂̔���͈�
		string chipName[10];
		After_Collision collision_Tri;//�{�[���̂ق��ɓn�����

		ML::Vec3 map_Rotation;//�}�b�v�̉�]��
		ML::QT map_QT;
		Collision col;
		

	public:
		//���\�b�h
		bool Map_Load(string f_);
		bool Map_Check_Hit(const ML::Box3D& hit);
		void Map_Check_Hit(ML::Vec3 pos, float r);//���ƃ}�b�v�̂����蔻��		
		void Map_Rotate();//Update�ŌĂԊ֐�
		

		After_Collision is_Collision();
	};
}