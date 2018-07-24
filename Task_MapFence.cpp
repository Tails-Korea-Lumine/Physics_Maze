//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include "Task_MapFence.h"
#include  "Task_Ball.h"

namespace  MapFence
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		
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
	bool  Object::Initialize(int sideNum, Difficult_Range di)
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();


		//���f�[�^������		
		this->fenceNumber = sideNum;
		this->mapSize = 8;//��{��8X8��Փx�ɂ���đ��������(2018/04/21)
		this->render3D_Priority[0] = 1.0f;
		//�ʂ��Ƃɕʂ̏����l��^����
		switch(fenceNumber)
		{
		case 0:
		case 8:
			this->map_QT = ML::QT(0.0f);
			break;
		case 9:
		case 1:
			this->map_QT = ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(90));
			break;
		case 10:
		case 2:
			this->map_QT = ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(180));
			break;
		case 11:
		case 3:
			this->map_QT = ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(-90));
			break;
		case 4:
			this->map_QT = ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(90));
			break;
		case 5:
			this->map_QT = ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(90));
			this->map_QT *= ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(90));
			break;
		case 6:
			this->map_QT = ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(90));
			this->map_QT *= ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(180));
			break;
		case 7:
			this->map_QT = ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(90));
			this->map_QT *= ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(-90));
			break;
		}
		//�f�[�^�̃[���N���A
		ZeroMemory(this->arr, sizeof(this->arr));
		this->sizeX = 0;
		this->sizeY = 0;
		this->sizeZ = 0;
		this->chipSize = 100.0f;
		for (int i = 0; i < _countof(this->chipName); i++)
		{
			this->chipName[i] = "";
		}
		this->col_Poligons.clear();
		switch (di)
		{
		case Difficult_Range::Easy:
			this->Map_Load("./data/fence/Easy/line" + to_string(sideNum) + ".txt");
			break;
		case Difficult_Range::Normal:
			this->Map_Load("./data/fence/Normal/line" + to_string(sideNum) + ".txt");
			break;
		case Difficult_Range::Hard:
			this->Map_Load("./data/fence/Hard/line" + to_string(sideNum) + ".txt");
			break;
		}
		

		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		for (int i = 0; i < _countof(this->chipName); i++)
		{
			if (this->chipName[i] != "")
			{
				DG::Mesh_Erase(this->chipName[i]);
			}
		}
		this->sizeX = 0;
		this->sizeY = 0;
		this->sizeZ = 0;
		this->col_Poligons.clear();

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
		//�{�[���̏����C��
		//auto ball = ge->GetTask_One_G<Ball::Object>("�{�[��");

		//this->frame_QT = ML::QT(0.0f);
		//auto in1 = DI::GPad_GetState("P1");

		//for (int i = 0; i < 10; i++)
		//{
		//	//�X�e�B�b�N���|���ꂽ�ʂ��X�V
		//	if (in1.B1.on)
		//	{
		//		//�X�e�B�b�N�œ���
		//		if (in1.LStick.axis.x != 0)
		//		{
		//			this->frame_QT = ML::QT(ML::Vec3(0, 0, 1), ML::ToRadian(-in1.LStick.axis.x / 10.0f));
		//			this->map_QT *= this->frame_QT;
		//		}
		//	}
		//	//������Ă��Ȃ�����Y����]��X����]
		//	else
		//	{

		//		if (in1.LStick.axis.y != 0)
		//		{
		//			this->frame_QT = ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(-in1.LStick.axis.y / 10.0f));
		//			this->map_QT *= this->frame_QT;
		//		}

		//		if (in1.LStick.axis.x != 0)
		//		{
		//			this->frame_QT = ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(-in1.LStick.axis.x / 10.0f));
		//			this->map_QT *= this->frame_QT;
		//		}
		//	}

		//	//���[���h��]�ʂɔ��f
		//	//ge->World_Rotation = this->map_QT;		


		//	//��]

		//	this->Map_Rotate();



		//	//�����蔻��͖���}�b�v�̂ق��ōs��	

		//	//�{�[���ƃ}�b�v�̂����蔻��
		//	if (ball != nullptr)
		//	{
		//		this->Map_Check_Hit(ball->Get_Pos(), ball->Get_Radious(), ball->Get_Speed());
		//	}

		//	//�ʒu�␳���d�|����
		//	for (auto p : this->col_Poligons)
		//	{
		//		if (p.collision_Flag)
		//		{
		//			ball->Fix_Position_for_Rotate(this->frame_QT);
		//			break;
		//		}
		//	}
		//}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		
	}

	void  Object::Render3D_L0()
	{
		

		ML::Mat4x4 matW;
		//matS.Scaling(this->chipSize);
		for (int y = 0; y < this->sizeY; y++)
		{
			for (int z = 0; z < this->sizeZ; z++)
			{
				for (int x = 0; x < this->sizeX; x++)
				{
					//���̓����_�����O���Ȃ�
					if (this->arr[z][y][x].What_Type_Is_this_Box() == BoxType::Road)
					{
						continue;
					}
					 
				/*	ML::Mat4x4 matT;
					matT.Translation(this->arr[z][y][x].Get_Pos());

					ML::Mat4x4 matR;
					matR.RotationQuaternion(this->map_QT);*/
					
					D3DXMatrixAffineTransformation(&matW, this->chipSize, NULL, &this->map_QT, &this->arr[z][y][x].Get_Pos());

					DG::EffectState().param.matWorld = matW;
					DG::Mesh_Draw(this->chipName[(int)this->arr[z][y][x].What_Type_Is_this_Box()]);
				}
			}
		}
	}

	//-----------------------------------------------------------------------------------
	//�ǉ����\�b�h	
	//�������Ă��邩��Ԃ��֐�	

	void Object::Get_Collision_Poligon(std::vector<After_Collision>* result) const
	{
		for (auto p : this->col_Poligons)
		{
			result->push_back(p);
		}
	}
	//---------------------------------------------------------------------------------------
	//�O���t�@�C������̃}�b�v���[�h
	bool Object::Map_Load(string f_)
	{
		//�O���t�@�C������ǂݍ���
		ifstream fin(f_);
		if (!fin)
		{
			return false;
		}
		int chipNum;
		fin >> chipNum;
		//�`�b�v�t�@�C�����ǂݍ��݂ƁA���b�V���̃��[�h
		for (int c = 1; c <= chipNum; c++)
		{
			string chipFileName, chipFilePath;
			fin >> chipFileName;
			chipFilePath = "./data/mesh/" + chipFileName;
			this->chipName[c] = "Fence_Chip" + std::to_string(c);
			DG::Mesh_CreateFromSOBFile(this->chipName[c], chipFilePath);
		}
		//�}�b�v�z��T�C�Y�̓ǂݍ���
		fin >> this->sizeX >> this->sizeY >> this->sizeZ;
		//�}�b�v�z��f�[�^�̓ǂ݂���
		for (int y = 0; y < this->sizeY; y++)
		{
			for (int z = this->sizeZ - 1; z >= 0; z--)
			{
				for (int x = 0; x < this->sizeX; x++)
				{
					int chip;
					fin >> chip;
					ML::Vec3 pos = ML::Vec3(x*this->chipSize + this->chipSize / 2, y*this->chipSize + this->chipSize / 2, z*this->chipSize + this->chipSize / 2);
					ML::Box3D base = ML::Box3D(-this->chipSize / 2, -this->chipSize / 2, -this->chipSize / 2, this->chipSize, this->chipSize, this->chipSize);				


					this->arr[z][y][x] = Bbox(BoxType(chip), pos, base, this->map_QT);
				}
			}
		}
		fin.close();
		return true;
	}
	//-----------------------------------------------------------------------
	void Object::Map_Check_Hit(const ML::Vec3& pos, const float& r, const ML::Vec3& speed)
	{
		//���d�Փ˂܂œK�p����ver0.3(2018/04/16)

		//�ڐG�O�p�`�𔻒�O�ɃN���A����
		this->col_Poligons.clear();

		//����X�^�[�g
		for (int y = 0; y < this->sizeY; y++)
		{
			for (int z = 0; z < this->sizeZ; z++)
			{
				for (int x = 0; x < this->sizeX; x++)
				{
					//��苗���ȓ��̂��̂������������
					ML::Vec3 d = this->arr[z][y][x].Get_Pos() - pos;
					//d�͐�Βl�̋���
					if (d.Length() < 0)
					{
						d *= -1;
					}
					//��苗���ȏゾ�����画�肹�����ɍ��ڂ�
					if (d.Length() > this->chipSize)
					{
						continue;
					}
					//���͔��肵�Ȃ�
					if (this->arr[z][y][x].What_Type_Is_this_Box() == BoxType::Road)
					{
						continue;
					}									
					//this->collision_Tri = this->col.Hit_Check(Mass, pos, r, this->map_QT); //(ver0.2�Ŏg��������)
					//std::vector<After_Collision> poligon 
					this->arr[z][y][x].Get_Collision_Poligon(&this->col_Poligons, pos, r, speed);
					

					//ver0.2�Ŏg��������
					//����œ��������珈�����~�߂�
					/*if (this->is_Collision().collision_Flag)
					{
					return;
					}*/
				}
			}
		}
	}

	//------------------------------------------------------------------------
	//��]������
	void Object::Map_Rotate()
	{
			

		for (int y = 0; y < this->sizeY; y++)
		{
			for (int z = 0; z < this->sizeZ; z++)
			{
				for (int x = 0; x < this->sizeX; x++)
				{

					ML::Vec3 pos = ML::Vec3(x*this->chipSize + this->chipSize / 2, y*this->chipSize + this->chipSize / 2, z*this->chipSize + this->chipSize / 2);
					if (this->fenceNumber < 8)
					{
						pos += ge->Map_center - ML::Vec3((this->mapSize*this->chipSize / 2), -(this->mapSize*this->chipSize / 2), -(this->mapSize*this->chipSize / 2));
					}
					else
					{
						pos += ge->Map_center - ML::Vec3(((this->mapSize+2)*this->chipSize / 2), (this->mapSize*this->chipSize / 2), -(this->mapSize*this->chipSize / 2));
					}
					//D3DXMatrixAffineTransformation(&matR, this->chipSize / 100.0f, &ML::Vec3(1050,50,1050), &qtW, NULL);
					//matR.Inverse();

					/*if (this->map_Rotation.Length() != 0)
					{
					ML::Mat4x4 matMove;
					D3DXMatrixAffineTransformation(&matMove, this->chipSize / 100.0f, &ge->Map_center, &qtM, NULL);
					matR *= matMove;
					}*/
					ML::Mat4x4 matR;
					D3DXMatrixAffineTransformation(&matR, this->chipSize / 100.0f, &ge->Map_center, &this->map_QT, NULL);
					//ML::Vec3 temp = matR.TransformCoord(this->arr[z][y][x].Get_Pos());
					pos = matR.TransformCoord(pos);

					//matR.Inverse();
					this->arr[z][y][x].Rotate_Box(pos, this->map_QT);
				}
			}
		}
	}

	//-------------------------------------------------------------------------------------------
	//�N�H�[�^�j�I�����X�V����֐�
	void Object::UpDate_Quartanion(const ML::QT& qt)
	{
		this->map_QT *= qt;
	}
	
	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_, int sideNum, Difficult_Range di)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
			}
			if (!ob->B_Initialize(sideNum,di)) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize(int sideNum, Difficult_Range di)
	{
		return  this->Initialize(sideNum,di);
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