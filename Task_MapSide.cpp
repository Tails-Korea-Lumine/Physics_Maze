//-------------------------------------------------------------------
//�L���[�u�O�ǂ̃}�b�v
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_MapSide.h"
#include  "Task_Ball.h"
#include  "Task_Game.h"
#include  "Task_CameraMan.h"
#include "Teleportation.h"
#include "Goal.h"
#include "Wall.h"
#include "Light_Switch.h"
#include "Unstable_Wall.h"


namespace  Map_Side
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
		this->render3D_Priority[0] = 0.3f;
		this->gimicCnt = 0;		
		this->sideNumber = sideNum;
		this->mapSize = 8;//��{��8X8
		this->rendering_Judge = _CMATH_::cosf(ML::ToRadian(70));
		//this->render3D_Priority[0] = 1.0f;
		//�f�[�^�̃[���N���A
		//ZeroMemory(this->arr, sizeof(this->arr));
		this->sizeX = 0;
		this->sizeZ = 0;
		this->chipSize = 100.0f;

		for (size_t z = 0; z < 8; z++)
		{
			for (size_t x = 0; x < 8; x++)
			{
				this->arr[z][x] = nullptr;
			}
		}		
		//�O���t�@�C������̓ǂݍ���
		switch (di)
		{
		case Difficult_Range::Easy:
			this->Map_Load("./data/map/Easy/map" + to_string(sideNum) + ".txt");
			break;
		case Difficult_Range::Normal:
			this->Map_Load("./data/map/Normal/map" + to_string(sideNum) + ".txt");
			break;
		case Difficult_Range::Hard:
			this->Map_Load("./data/map/Hard/map" + to_string(sideNum) + ".txt");
			break;
		}
		//�ʂ��Ƃɕʂ̏����l��^����
		switch (sideNumber)
		{
		case 0:
			this->Map_Rotate(ML::QT(0.0f));
			this->Normal_Side = ML::Vec3(0, 1, 0);
			break;
		case 1:
			this->Map_Rotate(ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(90)));
			this->Normal_Side = ML::Vec3(0, 0, 1);
			break;
		case 2:
			this->Map_Rotate(ML::QT(ML::Vec3(0, 0, 1), ML::ToRadian(90)));
			this->Normal_Side = ML::Vec3(-1, 0, 0);
			break;
		case 3:
			this->Map_Rotate(ML::QT(ML::Vec3(0, 0, 1), ML::ToRadian(-90)));
			this->Normal_Side = ML::Vec3(1, 0, 0);
			break;
		case 4:
			this->Map_Rotate(ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(-90)));
			this->Normal_Side = ML::Vec3(0, 0, -1);
			break;
		case 5:
			this->Map_Rotate(ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(180)));
			this->Normal_Side = ML::Vec3(0, -1, 0);
			break;
		}
		
		this->Check_Unusable_Side();
		//this->Array_Sorting();
		//this->Insert_Id_To_Ball();
		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���		
		for (size_t z = 0; z < this->sizeZ; z++)
		{
			for (size_t x = 0; x < this->sizeX; x++)
			{
				if (this->arr[z][x] != nullptr)
				{
					delete this->arr[z][x];
				}
			}
		}
		this->sizeX = 0;
		this->sizeZ = 0;
		

		
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
		//�����_�����O���邩���Ɋm�F
		if (this->Is_Need_Render() == false)
		{
			//�K�v�Ȃ��Ƃ��͂��̂܂܏����I��
			return;
		}			
		
		size_t z, x;
		for (z = 0; z < this->sizeZ; z++)
		{
			for (x = 0; x < this->sizeX; x++)
			{
				if (this->arr[z][x] == nullptr || this->Is_Need_Render(z, x) == false)
				{
					continue;
				}
				//�e�}�X�̃����_�����O����
				const float rb = 37.45f;
				const float gb = 52.98f;
				const float bb = 11.34;

				float r = abs( cosf(rb * (this->sideNumber + 1)));
				float g = abs( cosf(gb * (this->sideNumber + 1)));
				float b = abs( cosf(bb * (this->sideNumber + 1)));
				ML::Color color = ML::Color(1, r, g, b);
				if (this->arr[z][x]->What_Type_Is_this_Box() == Bbox::BoxType::Wall || this->arr[z][x]->What_Type_Is_this_Box() == Bbox::BoxType::Unstable_Wall)
				{
					DG::EffectState().param.objectColor = color;
				}
				this->arr[z][x]->Rendering();
				DG::EffectState().param.objectColor = ML::Color(1, 1, 1, 1);
			}
		}
		
	}

	//-----------------------------------------------------------------------------------
	//�ǉ����\�b�h		
	//�O���t�@�C������̃}�b�v���[�h
	bool Object::Map_Load(string f_)
	{
		//�����������b�V������ۑ����Ă����ꏊ
		string chipName[10];
		ML::Mat4x4 matR;
		//�O���t�@�C������ǂݍ���
		ifstream fin(f_);
		if (!fin)
		{
			return false;
		}
		//�����_�����O���郁�b�V���̌�
		int chipNum;
		fin >> chipNum;
		//�`�b�v�t�@�C�����ǂݍ��݂ƁA���b�V���̃��[�h
		for (int c = 1; c <= chipNum; c++)
		{
			string chipFileName, chipFilePath;
			fin >> chipFileName;
			chipFilePath = "./data/mesh/" + chipFileName;
			chipName[c] = "Map" + to_string(this->sideNumber) + "Chip" + std::to_string(c);
			DG::Mesh_CreateFromSOBFile(chipName[c], chipFilePath);
		}
		//�}�b�v�z��T�C�Y�̓ǂݍ���
		fin >> this->sizeX >> this->sizeZ;
		//�}�b�v�z��f�[�^�̓ǂ݂���
		
		for (size_t z = 0; z < this->sizeZ; z++)
		{
			for (size_t x = 0; x < this->sizeX; x++)
			{
				//�`�b�v�ԍ�(�{�b�N�X�^�C�v)�ǂݍ���
				int chip;
				fin >> chip;
				//�}�b�v���S�_����ɂ��������ʒu�Z�o
				ML::Vec3 pos = ML::Vec3(x*this->chipSize + this->chipSize / 2, this->chipSize + this->chipSize / 2, (this->sizeZ - 1 - z)*this->chipSize + this->chipSize / 2);
				pos += ge->Map_center - ML::Vec3((this->mapSize*this->chipSize / 2), -((this->mapSize - 2)*this->chipSize / 2), (this->mapSize*this->chipSize / 2));
				//�����蔻��p��`
				ML::Vec3 base = ML::Vec3(this->chipSize/2.0f, this->chipSize/2.0f, this->chipSize/2.0f);
				//�{�b�N�X��ID����
				string id = to_string(this->sideNumber) + to_string(z) + to_string(x);
				
				//�������邱�ƈȊO�ɉ���������������K�v���������
				switch (Bbox::BoxType(chip))
				{
				//�e���|�[�g
				case Bbox::BoxType::Teleportation:
					//�z��ɓo�^
					this->arr[z][x] = new Teleportation(pos, base, this->map_QT, id, chipName[chip], this->sideNumber);
					break;
				//�X�C�b�`�͂����蔻��͈͂�������	
				case Bbox::BoxType::LightSwitch:
					base /= 10.0f;
					//�z��ɓo�^
					this->arr[z][x] = new Light_Switch(pos, base, this->map_QT, id, chipName[chip]);
					break;
				//�{�[�����X�^�[�g�ʒu�ɒu��
				case Bbox::BoxType::Start:					
					D3DXMatrixAffineTransformation(&matR, 1.0f, &ge->Map_center, &ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(-90)), NULL);
					pos = matR.TransformCoord(pos);
					ge->GetTask_One_G<Ball::Object>("�{�[��")->Teleportation(pos);
					//�z��o�^�͂��Ȃ�
					break;
				//��{�̕Ǐ�Q��
				case Bbox::BoxType::Wall:
					//�z��ɓo�^
					this->arr[z][x] = new Wall(pos, base, this->map_QT, id, "MapCore");
					break;
				//�S�[���ʒu
				case Bbox::BoxType::Goal:
					base /= 10.0f;					
					//�z��ɓo�^
					this->arr[z][x] = new Goal(pos, base, this->map_QT, id, chipName[chip]);
					break;
				//�����
				case Bbox::BoxType::Unstable_Wall:
					this->arr[z][x] = new Unstable_Wall(pos, base, this->map_QT, id, chipName[chip]);
					break;
				default:
					continue;
					break;
				}
								
				
			}
			
		}
		fin.close();
		return true;
	}
	//-----------------------------------------------------------------------
	bool Object::Map_Check_Hit(Shape3D* ball)
	{		
		//�{�[�������̖ʂɂ��邩���m�F���邽�߂̃t���O	
		bool ball_on_This_Side = false;
		std::vector<Collision_Data> col_Poligons;

		//����X�^�[�g		
		for (size_t z = 0; z < this->sizeZ; z++)
		{
			for (size_t x = 0; x < this->sizeX; x++)
			{
				if (this->arr[z][x] == nullptr)
				{
					continue;
				}
				//�ڐG�O�p�`�𔻒�O�ɃN���A����
				col_Poligons.clear();				
				//��苗���ȓ��̂��̂������������
				ML::Vec3 d = this->arr[z][x]->Get_Pos() - ball->Get_Center();
				//d�͐�Βl�̋���(distance)				
				//��苗���ȏゾ�����画�肹�����ɍ��ڂ�
				if (d.Length() > this->chipSize)
				{
					continue;
				}
				ball_on_This_Side = true;

				//����J�n�y�уM�~�b�N����
				if (!this->arr[z][x]->Collision_Action(&col_Poligons, ball))
				{
					//�������ĂȂ��}�X�Ȃ玟�̃}�X�Ɉڍs
					continue;
				}
				//����̌��ʂ͕ۑ�����
				for (auto& c : col_Poligons)
				{
					ge->collision_Result.push_back(c);
				}
			}
			
		}
		//�߂�l�̓{�[�������̖ʂɂ��邩���m�F�����t���O
		return ball_on_This_Side;
	}

	//------------------------------------------------------------------------
	//��]������
	void Object::Map_Rotate(const ML::QT& qt)
	{
		//�}�b�v�S�̂̉�]�l�X�V
		this->UpDate_Quartanion(qt);

		for (size_t z = 0; z < this->sizeZ; z++)
		{
			for (size_t x = 0; x < this->sizeX; x++)
			{
				if (this->arr[z][x] == nullptr)
				{
					continue;
				}
				//��]�s�񐶐�
				ML::Mat4x4 matR;
				D3DXMatrixAffineTransformation(&matR, 1.0f, &ge->Map_center, &qt, NULL);				

				//�{�b�N�X�Ɍʂœn��
				this->arr[z][x]->Rotate_Box(&matR, qt);
			}
		}
		
	}

	//-------------------------------------------------------------------------------------------
	//�N�H�[�^�j�I�����X�V����֐�
	void Object::UpDate_Quartanion(const ML::QT& qt)
	{
		this->map_QT *= qt;

		//�@���x�N�g������]�ʂɉ����ē�������]���s��
		ML::Mat4x4 matR;
		D3DXMatrixAffineTransformation(&matR, 1.0f, NULL, &qt, NULL);

		this->Normal_Side = matR.TransformNormal(this->Normal_Side);
	}

	//------------------------------------------------------------------------------------------
	//�����_�����O���邩���m�F����
	bool Object::Is_Need_Render()
	{
		//�J�����Ƃ̓��ϒl��ۑ�����ꏊ
		float c = 0.0f;
		//�����x�N�g��
		ML::Vec3 sv = ge->camera[0]->target - ge->camera[0]->pos;
		MyMath::Vector_Dot(&c, sv, this->Normal_Side);

		//���ϒl��-90 < cos < 90�̊Ԃ̓����_�����O�����Ȃ�
		return c <= this->rendering_Judge;
	}
	//---------------------------------------------------------------------------------------------
	//�����_�����O���邩���Ȃ������m�F���邽�߂̃��\�b�h(�}�X��)
	bool Object::Is_Need_Render(const unsigned int& z ,const unsigned int& x)
	{
		ML::Vec3 d_Cmc = (ge->Map_center - ge->camera[0]->pos);
		const float judge = d_Cmc.Length() + (this->chipSize * this->mapSize / 3.0f);

		//���肷�鋗�����}�X�ƃJ�����Ƃ̋����������Ȃ烌���_�����O���Ȃ�
		ML::Vec3 d_Cf0 = this->arr[z][x]->Get_Pos() - ge->camera[0]->pos;

		//�}���������ŕԂ�
		return d_Cf0.Length() < judge;

	}
	//-----------------------------------------------------------------------------------------
	//�}�b�v�̖@���x�N�g����Ԃ����\�b�h
	ML::Vec3 Object::Get_Normal_Side()const
	{
		return this->Normal_Side;
	}
	//�A�����Ă��Ďg���Ȃ��ʂ�T��
	void Object::Check_Unusable_Side()
	{
		//�ŏ��l�A�ő�l�𒴂��Ȃ��悤�ɂ���
		auto Inside_Range = [this](const size_t& cx, const size_t& cz)
		{return cx >= 0 && cx < this->sizeX && cz >= 0 && cz < this->sizeZ; };

		//���������������ǂȂ̂����f����
		auto Judge = [](const Bbox* b1, const Bbox* b2)
		{
			if (b1 != nullptr && b2 != nullptr)
			{
				auto b1_Type = b1->What_Type_Is_this_Box();
				auto b2_Type = b2->What_Type_Is_this_Box();
				return (b1_Type == Bbox::BoxType::Wall) && (b2_Type == Bbox::BoxType::Wall);
			}
			else
			{
				return false;
			}
		};
		//�e�{�b�N�X�ɘA������{�b�N�X�����邩���m�F
		for (size_t z = 0; z < this->sizeZ; z++)
		{
			for (size_t x = 0; x < this->sizeX; x++)
			{
				//���ʊJ�n
				//��
				if (Inside_Range(x, z-1) && Judge(this->arr[z - 1][x], this->arr[z][x]))
				{
					//�����|���S����\�����Ă���
					this->arr[z][x]->Marking_On_Unusable_Side(Box_Side::Zplus);
				}
				//��O
				if (Inside_Range(x, z + 1) && Judge(this->arr[z + 1][x], this->arr[z][x]))
				{
					//�����|���S����\�����Ă���
					this->arr[z][x]->Marking_On_Unusable_Side(Box_Side::Zminus);
				}
				//��
				if (Inside_Range(x - 1, z) && Judge(this->arr[z][x - 1], this->arr[z][x]))
				{
					//�����|���S����\�����Ă���
					this->arr[z][x]->Marking_On_Unusable_Side(Box_Side::Xminus);
				}
				//�E
				if (Inside_Range(x + 1, z) && Judge(this->arr[z][x+1], this->arr[z][x]))
				{
					//�����|���S����\�����Ă���
					this->arr[z][x]->Marking_On_Unusable_Side(Box_Side::Xplus);
				}
			}
		}
	}

	//-------------------------------------------------------------------------------------
	//�z��\�[�g�y�у{�[�����X�^�[�g�ʒu�ɒu��
	//void Object::Array_Sorting()
	//{
	//	//2�����z��Ȃ̂ŁAsizeZ���\�[�g��������
	//	for (size_t z = 0; z < this->sizeZ; z++)
	//	{
	//		//stl remove_if�ō폜(�����f�[�^�ɏ㏑������)�Ƃ�������炢�Ȃ���
	//		//�f�[�^��O�ɐς߂�
	//		auto remove_Point = remove_if(&this->arr[z][0], &this->arr[z][this->sizeX], [](const Bbox& b) {return b.What_Type_Is_this_Box() == BoxType::Road; });

	//		//�����f�[�^�ɏ㏑������
	//		for (; remove_Point != &this->arr[z][this->sizeX]; remove_Point++)
	//		{
	//			*remove_Point = Bbox();
	//		}
	//	}
	//}
	//�{�[���^�X�N�̃t���O��ID��g�ݍ��߂�
	//void Object::Insert_Id_To_Ball()
	//{
	//	//ID�o�^�̂��߂Ƀ{�[���^�X�N��������Ă���
	//	auto ball = ge->GetTask_One_G<Ball::Object>("�{�[��");

	//	for (size_t z = 0; z < this->sizeZ; z++)
	//	{
	//		for (size_t x = 0; x < this->sizeX; x++)
	//		{
	//			auto now_Type = this->arr[z][x].What_Type_Is_this_Box();
	//			//���͔z��̌��ɐς�ł������̂Ō���������break
	//			if (now_Type == BoxType::Road || now_Type == BoxType::Clear)
	//			{
	//				break;
	//			}
	//			//����ȊO�̓t���O�o�^
	//			ball->Set_Id_And_Flag(this->arr[z][x].Get_Id());
	//		}
	//	}
	//}
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