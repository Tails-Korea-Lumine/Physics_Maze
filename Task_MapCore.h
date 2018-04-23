#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�}�b�v�̃R�A
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include "Collision.h"
#include "Bbox.h"

namespace Map_Core
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�}�b�v");	//�O���[�v��
	const  string  defName("Core");	//�^�X�N��
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
		//�Ăяo�����b�V���̖��O(�o���A�[�͕\�����Ȃ��̂�1����
		string meshName;
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
		//�^�񒆂̃L���[�u�ƊO�ɔ�΂���Ȃ��悤�ɂ���O��
		Bbox core, barrier[6];
		//�o���A�̏����ʒu
		ML::Vec3 b_ini_pos[6];
		//�}�b�v�̑傫���A��Փx�ŕς��
		int mapSize;
		//�{�[���ɓn�����	
		std::vector< After_Collision> col_Poligons;		
		//��]��
		ML::QT map_QT;		

	public:
		//���\�b�h
		void Rotate_Core_and_Barrier();
		void Core_Check_Hit(ML::Vec3 pos, float r, ML::Vec3 speed);//���ƃ}�b�v�̂����蔻��
		//�ق��̃v���O�����ɂ����蔻�肪�I������|���S����n���֐�
		std::vector<After_Collision> Get_Collision_Poligon();
	};
}