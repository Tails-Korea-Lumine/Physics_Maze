//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_MapCore.h"
#include  "Task_Ball.h"

namespace  Map_Core
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->meshName = "MapCore";
		//���̃��b�V��
		DG::Mesh_CreateFromSOBFile(this->meshName, "./data/mesh/box3.sob");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{		
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
		this->mapSize = 8;//��{��8X8��Փx�ɂ���đ��������(2018/04/21)		
		this->map_QT = ML::QT(0.0f);
		
		//�R�A�̏�����
		this->core = Bbox(BoxType::Core, ge->Map_center, ML::Box3D(-50 * this->mapSize, -50 * this->mapSize, -50 * this->mapSize, 100 * this->mapSize, 100 * this->mapSize, 100 * this->mapSize), this->map_QT);
		//�o���A�̏�����
		ML::Box3D XZ, XY, YZ;
		XZ = ML::Box3D(-100 * (this->mapSize -2), -50 , -100 * (this->mapSize -2), 100 * (this->mapSize + 4), 100, 100 * (this->mapSize + 4));
		XY = ML::Box3D(-100 * (this->mapSize - 2), -100 * (this->mapSize - 2), -50 , 100 * (this->mapSize + 4), 100 * (this->mapSize + 4), 100);
		YZ = ML::Box3D(-50 , -100 * (this->mapSize - 2), -100 * (this->mapSize - 2), 100, 100 * (this->mapSize + 4), 100 * (this->mapSize + 4));

		this->b_ini_pos[0] = ge->Map_center + ML::Vec3(0, 50 * (this->mapSize + 4), 0);
		this->b_ini_pos[1] = ge->Map_center + ML::Vec3(0, 0, 50 * (this->mapSize + 4));
		this->b_ini_pos[2] = ge->Map_center + ML::Vec3(-50 * (this->mapSize + 4), 0, 0);
		this->b_ini_pos[3] = ge->Map_center + ML::Vec3(50 * (this->mapSize + 4), 0, 0);
		this->b_ini_pos[4] = ge->Map_center + ML::Vec3(0, 0, -50 * (this->mapSize + 4));
		this->b_ini_pos[5] = ge->Map_center + ML::Vec3(0, -50 * (this->mapSize + 4), 0);

		this->barrier[0] = Bbox(BoxType::Barrier, b_ini_pos[0] , XZ, this->map_QT);
		this->barrier[1] = Bbox(BoxType::Barrier, b_ini_pos[1] , XY, this->map_QT);
		this->barrier[2] = Bbox(BoxType::Barrier, b_ini_pos[2] , YZ, this->map_QT);
		this->barrier[3] = Bbox(BoxType::Barrier, b_ini_pos[3] , YZ, this->map_QT);
		this->barrier[4] = Bbox(BoxType::Barrier, b_ini_pos[4] , XY, this->map_QT);
		this->barrier[5] = Bbox(BoxType::Barrier, b_ini_pos[5] , XZ, this->map_QT);

		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���


		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			//�������p���^�X�N�̐���
			//auto nextTask = Game::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto in1 = DI::GPad_GetState("P1");
		//�X�e�B�b�N���|���ꂽ�ʂ��X�V
		if (in1.B1.on)
		{
			//�X�e�B�b�N�œ���
			if (in1.LStick.axis.x != 0)
			{
				ML::QT temp_qt(ML::Vec3(0, 0, 1), ML::ToRadian(-in1.LStick.axis.x));
				this->map_QT *= temp_qt;
			}
		}
		//������Ă��Ȃ�����Y����]��X����]
		else
		{

			if (in1.LStick.axis.y != 0)
			{
				ML::QT temp_qt(ML::Vec3(1, 0, 0), ML::ToRadian(-in1.LStick.axis.y));
				this->map_QT *= temp_qt;
			}

			if (in1.LStick.axis.x != 0)
			{
				ML::QT temp_qt(ML::Vec3(0, 1, 0), ML::ToRadian(-in1.LStick.axis.x));
				this->map_QT *= temp_qt;
			}
		}

		//���[���h��]�ʂɔ��f
		ge->World_Rotation = this->map_QT;


		//��]
		this->Rotate_Core_and_Barrier();


		//�����蔻��͖���}�b�v�̂ق��ōs��
		//�{�[���̏����C��
		auto ball = ge->GetTask_One_G<Ball::Object>("�{�[��");

		this->Core_Check_Hit(ball->pos, ball->r, ball->speed);

	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{

	}

	void  Object::Render3D_L0()
	{
		//�����_�����O�����A�o���A�̓����_�����O�����Ȃ�
		ML::Mat4x4 matS, matR, matT;

		matS.Scaling(this->mapSize);
		matR.RotationQuaternion(this->map_QT);
		/*for (int i = 0; i < 6; i++)
		{
			matT.Translation(this->barrier[i].Get_Pos());

			DG::EffectState().param.matWorld = matS * matR * matT;

			DG::Mesh_Draw(this->res->meshName);
		}*/
		matT.Translation(this->core.Get_Pos());

		DG::EffectState().param.matWorld = matS * matR * matT;

		DG::Mesh_Draw(this->res->meshName);

	}

	//-----------------------------------------------------------------------------------
	//�ǉ����\�b�h

	//��]
	void Object::Rotate_Core_and_Barrier()
	{
		ML::Mat4x4 matR;
		//matR���A�t�B���ϊ��s��ɍ��
		D3DXMatrixAffineTransformation(&matR, 1.0f, &ge->Map_center, &this->map_QT,NULL);
		
		//��]�J�n
		ML::Vec3 temp = matR.TransformCoord(ge->Map_center);
		this->core.Rotate_Box(temp, this->map_QT);
		for (int b = 0; b < 6; b++)
		{
			ML::Vec3 btemp = matR.TransformCoord(this->b_ini_pos[b]);
			this->barrier[b].Rotate_Box(btemp, this->map_QT);
		}
	}

	//------------------------------------------------------------------------------------
	//�����蔻��

	void Object::Core_Check_Hit(ML::Vec3 pos, float r, ML::Vec3 speed)
	{
		//�ڐG�O�p�`�𔻒�O�ɃN���A����	
		this->col_Poligons.clear();

		//����֐�
		After_Collision poligon0 = this->core.Get_Collision_Poligon(pos, r, speed);


		//����̌��ʂ�ۑ�
		this->col_Poligons.push_back(poligon0);
		for (int b = 0; b < 6; b++)
		{
			this->col_Poligons.push_back(this->barrier[b].Get_Collision_Poligon(pos,r,speed));
		}
	}

	//-----------------------------------------------------------------------------------
	//�ق��̃v���O�����ɂ����蔻�肪�I������|���S����n���֐�
	std::vector<After_Collision> Object::Get_Collision_Poligon()
	{
		return this->col_Poligons;
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