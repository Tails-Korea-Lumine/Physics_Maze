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
		//�f�[�^�̃[���N���A
		//ZeroMemory(this->arr, sizeof(this->arr));
		//this->sizeX = 0;
		this->sizeY = 0;
		this->sizeX = 0;
		this->chipSize = 100.0f;
		for (int i = 0; i < _countof(this->chipName); i++)
		{
			this->chipName[i] = "";
		}
		this->col_Poligons.clear();
		//�O���t�@�C������̓ǂݍ���
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
		//�ʂ��Ƃɕʂ̏����l��^����
		switch(fenceNumber)
		{
		case 0:
		case 8:
			this->Map_Rotate( ML::QT(0.0f));
			break;
		case 9:
		case 1:
			this->Map_Rotate(ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(90)));
			break;
		case 10:
		case 2:
			this->Map_Rotate(ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(180)));
			break;
		case 11:
		case 3:
			this->Map_Rotate(ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(-90)));
			break;
		case 4:
			this->Map_Rotate(ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(90)));
			break;
		case 5:
			this->Map_Rotate(ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(90)) * ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(90)));
			break;
		case 6:
			this->Map_Rotate(ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(90)) * ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(180)));
			break;
		case 7:
			this->Map_Rotate(ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(90)) * ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(-90)));
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
		//this->sizeX = 0;
		this->sizeY = 0;
		this->sizeX = 0;
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
			for (int x = 0; x < this->sizeX; x++)
			{
				
				//���̓����_�����O���Ȃ�
				if (this->arr[y][x].What_Type_Is_this_Box() == BoxType::Road)
				{
					continue;
				}
					 
				//�`��
				D3DXMatrixAffineTransformation(&matW, this->chipSize, NULL, &this->map_QT, &this->arr[y][x].Get_Pos());

				DG::EffectState().param.matWorld = matW;
				DG::Mesh_Draw(this->chipName[(int)this->arr[y][x].What_Type_Is_this_Box()]);
				
			}
		}
	}

	//-----------------------------------------------------------------------------------
	//�ǉ����\�b�h		
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
		fin >> this->sizeX >> this->sizeY;
		//�}�b�v�z��f�[�^�̓ǂ݂���
		for (int y = 0; y < this->sizeY; y++)
		{
			for (int x = 0; x < sizeX; x++)
			{
				//�`�b�v�ԍ�(�{�b�N�X�^�C�v)�ǂݍ���
				int chip;
				fin >> chip;
				//�}�b�v��������ɂ��������ʒu�Z�o
				ML::Vec3 pos = ML::Vec3(x * this->chipSize + this->chipSize / 2, y*this->chipSize + this->chipSize / 2, this->chipSize + this->chipSize / 2);

				if (this->fenceNumber < 8)
				{
					pos += ge->Map_center - ML::Vec3((this->mapSize*this->chipSize / 2), -(this->mapSize*this->chipSize / 2), -((this->mapSize - 2)*this->chipSize / 2));
				}
				else
				{
					pos += ge->Map_center - ML::Vec3(((this->mapSize + 2)*this->chipSize / 2), (this->mapSize*this->chipSize / 2), -((this->mapSize - 2)*this->chipSize / 2));
				}
				//�����蔻��p��`
				ML::Box3D base = ML::Box3D(-this->chipSize / 2, -this->chipSize / 2, -this->chipSize / 2, this->chipSize, this->chipSize, this->chipSize);				

				//�z��ɓo�^
				this->arr[y][x] = Bbox(BoxType(chip), pos, base, this->map_QT);
				
			}
		}
		fin.close();
		return true;
	}
	//-----------------------------------------------------------------------
	void Object::Map_Check_Hit(std::vector<ML::Vec3>& all_Points, const ML::Vec3& pos, const float& r, const ML::Vec3& speed)
	{
		//���d�Փ˂܂œK�p����ver0.3(2018/04/16)

		//�ڐG�O�p�`�𔻒�O�ɃN���A����
		this->col_Poligons.clear();

		//����X�^�[�g
		for (int y = 0; y < this->sizeY; y++)
		{
			for (int x = 0; x < this->sizeX; x++)
			{
				//for (int x = 0; x < this->sizeX; x++)
				{
					//���͔z��̌��ɐς߂Ă������̂Ŕ��������炻�̌�͏���������break
					if (this->arr[y][x].What_Type_Is_this_Box() == BoxType::Road)
					{
						continue;
					}
					//��苗���ȓ��̂��̂������������
					ML::Vec3 d = this->arr[y][x].Get_Pos() - pos;
					//d�͐�Βl�̋���					
					//��苗���ȏゾ�����画�肹�����ɍ��ڂ�
					if (d.Length() > this->chipSize)
					{
						continue;
					}
													
					//this->collision_Tri = this->col.Hit_Check(Mass, pos, r, this->map_QT); //(ver0.2�Ŏg��������)
					//std::vector<After_Collision> poligon 
					this->arr[y][x].Get_Collision_Poligon(&this->col_Poligons, all_Points, pos, r, speed);
					

					//�S�̏Փˌ��ʂɕۑ�����
					for (auto& c : this->col_Poligons)
					{
						ge->collision_Result.push_back(c);
					}
				}
			}
		}
	}

	//------------------------------------------------------------------------
	//��]������
	void Object::Map_Rotate(const ML::QT& qt)
	{
		//�S�̂̉�]�l�X�V
		this->UpDate_Quartanion(qt);

		for (int y = 0; y < this->sizeY; y++)
		{
			for (int x = 0; x < this->sizeX; x++)
			{
				//��]�s�񐶐�
				ML::Mat4x4 matR;
				D3DXMatrixAffineTransformation(&matR, this->chipSize / 100.0f, &ge->Map_center, &qt, NULL);
				//�{�b�N�X�Ɍʂœn��
				this->arr[y][x].Rotate_Box(&matR, qt);
				
			}
		}
	}

	//-------------------------------------------------------------------------------------------
	//�N�H�[�^�j�I�����X�V����֐�
	void Object::UpDate_Quartanion(const ML::QT& qt)
	{
		this->map_QT *= qt;
	}
	//-------------------------------------------------------------------------------------
	//�z��\�[�g�y�у{�[�����X�^�[�g�ʒu�ɒu��
	void Object::Array_Sorting()
	{
		//�ꎞ�I�ɃR�s�[����ꏊ
		Bbox temp;
		//�z��̃{�b�N�X�^�C�v�����Ȃ�Ό��ɒu��
		for (int y = 0; y < this->sizeY; y++)
		{
			for (int x = 0; x < this->sizeX; x++)
			{
				//���̃{�b�N�X�͌��ɐς߂�
				if (this->arr[y][x].What_Type_Is_this_Box() == BoxType::Road)
				{
					temp = this->arr[y][x];
					//1��̂��̂ɏ㏑������
					for (int i = x; i < this->sizeX - 1; i++)
					{
						this->arr[y][i] = this->arr[y][i + 1];
					}
					this->arr[y][this->sizeX - 1] = temp;
				}
				else if (this->arr[y][x].What_Type_Is_this_Box() == BoxType::Start)
				{
					//�{�[�������̈ʒu�ɒu��
					ge->GetTask_One_G<Ball::Object>("�{�[��")->Teleportation(this->arr[y][x].Get_Pos());
				}
				//���𔭌������̂�1��̂��̂����ȊO��������
				if (this->arr[y][x].What_Type_Is_this_Box() == BoxType::Road && this->arr[y][x + 1].What_Type_Is_this_Box() != BoxType::Road)
				{
					//�ς߂鏈���̂�蒼��
					y--;
				}
			}
		}
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