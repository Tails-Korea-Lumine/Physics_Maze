#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�{�[��
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include  "Gravity.h"

namespace Ball
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�{�[��");	//�O���[�v��
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

		//���݈ʒu(���S�_)
		ML::Vec3 pos;
		//�ړ���
		ML::Vec3 speed;
		//�t���[�����̈ړ��ʕω���S���v�Z�����x�N�g��
		ML::Vec3 moveVec;
		//�����a
		float r;
		//�d��
		float m;
		//�ڐG���Ă��邩�𔻒肷��t���O
		bool collision_Flag;
		//�d��
		Gravity G;

	public:
		//���\�b�h
		//��O�̃t���[���ŏՓ˂�������Ԃ�
		bool Is_Collision();
		//���̎��̃{�[���̏���Ԃ��֐�
		//�ʒu
		ML::Vec3 Get_Pos();
		//�����a
		float Get_Radious();
		//���x
		ML::Vec3 Get_Speed();
		//�����蔻��ɂ������]���y�шړ�
		void Move_Ball();
		//�ʒu�␳�p��]�֐� ���� : (�}�b�v�̃t���[����]��)
		void Fix_Position_for_Rotate(const ML::QT& qt);
		//�e���|�[�g(�w�肵���ꏊ�ɑ��ړ�����)���� : (�ړI�n)
		void Teleportation(const ML::Vec3& objectPos);
		//�R�A�Ƃ������ĂȂ����R�A�̕��Ɉ���������
		void Graviation_Pull(const ML::Vec3& force);
	};
}