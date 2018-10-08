//-------------------------------------------------------------------
//�L���[�u�O�ǂ̃}�b�v
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_MapSide.h"
#include  "Task_Ball.h"
#include  "Task_Game.h"
#include  "Task_CameraMan.h"

#define JUDGE_DEGREE_EIGHTYFIVE 85

namespace  Map3d
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->seTeleportIn = "SETeleIn";
		DM::Sound_CreateSE(this->seTeleportIn, "./data/sound/TeleportIn.wav");
		this->seTeleportOut = "SETeleOut";
		DM::Sound_CreateSE(this->seTeleportOut, "./data/sound/TeleportOut.wav");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{		
		DM::Sound_Erase(this->seTeleportOut);
		DM::Sound_Erase(this->seTeleportIn);
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
		this->rendering_Judge = _CMATH_::cosf(ML::ToRadian(JUDGE_DEGREE_EIGHTYFIVE));

		
		//�f�[�^�̃[���N���A
		//ZeroMemory(this->arr, sizeof(this->arr));
		this->sizeX = 0;
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
			this->Map_Rotate( ML::QT(0.0f));
			this->Normal_Side = ML::Vec3(0, 1, 0);
			break;
		case 1:
			this->Map_Rotate(ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(90)));
			this->Normal_Side = ML::Vec3(0, 0, 1);
			break;
		case 2:
			this->Map_Rotate( ML::QT(ML::Vec3(0, 0, 1), ML::ToRadian(90)));
			this->Normal_Side = ML::Vec3(-1, 0, 0);
			break;
		case 3:
			this->Map_Rotate(ML::QT(ML::Vec3(0, 0, 1), ML::ToRadian(-90)));
			this->Normal_Side = ML::Vec3(1, 0, 0);
			break;
		case 4:
			this->Map_Rotate( ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(-90)));
			this->Normal_Side = ML::Vec3(0, 0, -1);
			break;
		case 5:
			this->Map_Rotate( ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(180)));
			this->Normal_Side = ML::Vec3(0, -1, 0);
			break;
		}

		//�X�^�[�g�ʒu�Ƀ{�[����u��
		this->Init_Positioning_Ball();

		//for�������炷���߃W���O�z��݂����Ȃ������ɂ���
		for (auto& j : this->arr)
		{
			j.erase(remove_if(j.begin(), j.end(), [](Bbox& b) {return (int)b.What_Type_Is_this_Box() <= (int)BoxType::Road ? true : false; })
				, j.end()
			);
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
		//�����_�����O���邩���Ɋm�F
		if (this->Is_Need_Render() == false)
		{
			//�K�v�Ȃ��Ƃ��͂��̂܂܏����I��
			return;
		}

		ML::Mat4x4 matS;
		matS.Scaling(this->chipSize);
		
		for (auto& z : this->arr)
		{
			for (auto& x : z)
			{
				//���̓����_�����O���Ȃ�
				/*if (this->arr[z][x].What_Type_Is_this_Box() == BoxType::Road)
				{
					continue;
				}*/
					 
				ML::Mat4x4 matT;
				matT.Translation(x.Get_Pos());

				ML::Mat4x4 matR;
				matR.RotationQuaternion(this->map_QT);
				//D3DXMatrixAffineTransformation(&matR, this->chipSize / 100.0f, NULL, &qtW, NULL);
				////matR.Inverse();

				//if (this->map_Rotation.Length() != 0)
				//{
				//	ML::Mat4x4 matMove;
				//	D3DXMatrixAffineTransformation(&matMove, this->chipSize / 100.0f, NULL, &qtM, NULL);
				//	matR *= matMove;
				//}
				//matR.Inverse();

				DG::EffectState().param.matWorld = matS * matR * matT;
				DG::Mesh_Draw(this->chipName[(int)x.What_Type_Is_this_Box()]);
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
			this->chipName[c] = "Map" + to_string(this->sideNumber) + "Chip" + std::to_string(c);
			DG::Mesh_CreateFromSOBFile(this->chipName[c], chipFilePath);
		}
		//�}�b�v�z��T�C�Y�̓ǂݍ���
		fin >> this->sizeX >> this->sizeZ;
		//�}�b�v�z��f�[�^�̓ǂ݂���
		
		for (int z = 0; z < this->sizeZ; z++)
		{
			std::vector<Bbox> t;
			t.clear();
			for (int x = 0; x < this->sizeX; x++)
			{
				int chip;
				fin >> chip;
				ML::Vec3 pos = ML::Vec3(x*this->chipSize + this->chipSize / 2, this->chipSize + this->chipSize / 2, (this->sizeZ - 1 - z) * this->chipSize + this->chipSize / 2);
				pos += ge->Map_center - ML::Vec3((this->mapSize*this->chipSize / 2), -((this->mapSize - 2)*this->chipSize / 2), (this->mapSize*this->chipSize / 2));

				ML::Box3D base = ML::Box3D(-this->chipSize / 2, -this->chipSize / 2, -this->chipSize / 2, this->chipSize, this->chipSize, this->chipSize);				
				ML::Mat4x4 matR;
				ML::Vec3 objectPos;
				//�������邱�ƈȊO�ɉ���������������K�v���������
				switch (BoxType(chip))
				{				
				//�e���|�[�g
				case BoxType::Teleportaion:		

					ge->TM.Update_Door_Position(this->sideNumber, pos);
					break;
				//�X�C�b�`�͂����蔻��͈͂�������	
				case BoxType::LightSwitch:
					base = ML::Box3D(base.x / 10, base.y / 10, base.z / 10, base.w / 10, base.h / 10, base.d / 10);
					break;
				}

				
				//this->arr[z][x] = Bbox(BoxType(chip), pos, base, this->map_QT);
				t.push_back(Bbox(BoxType(chip), pos, base, this->map_QT));
			}
			this->arr.push_back(t);
		}
		fin.close();

		
		
		return true;
	}
	//---------------------------------------------------------------------------------------
	//�{�[�����X�^�[�g�ʒu�ɒu��
	void Object::Init_Positioning_Ball()
	{
		//�X�^�[�g�ʒu���z��̒��ɂ��邩���m�F���ă{�[���������ړ�������
		for (auto& z : this->arr)
		{
			for (auto& x : z)
			{
				if (x.What_Type_Is_this_Box() == BoxType::Start)
				{
					auto ball = ge->GetTask_One_G<Ball::Object>("�{�[��");

					ball->Teleportation(x.Get_Pos());
				}
			}
		}
	}
	//-----------------------------------------------------------------------
	bool Object::Map_Check_Hit(std::vector<ML::Vec3>& all_Points, const ML::Vec3& pos, const float& r, const ML::Vec3& speed)
	{
		//���d�Փ˂܂œK�p����ver0.3(2018/04/16)

		//�ڐG�O�p�`�𔻒�O�ɃN���A����
		this->col_Poligons.clear();
		//std::vector<After_Collision> poligon;
		bool ball_on_This_Side = false;
		//����X�^�[�g
		
		for (auto& z : this->arr)
		{
			for (auto& x : z)
			{
				//��苗���ȓ��̂��̂������������
				ML::Vec3 d = x.Get_Pos() - pos;

				if (d.Length() > this->chipSize)
				{
					continue;
				}

				//d�͐�Βl�̋���(distance)					
				ball_on_This_Side = true;
				//boxType�ʂɏ����𕪂���
				switch (x.What_Type_Is_this_Box())
				{
				//���͔���Ȃ�
				case BoxType::Road:
					continue;
					break;
				//�S�[�����̓N���A����
				case BoxType::Goal:
					if (x.Get_Collision_Bool(all_Points,pos, r, speed))
					{
						ML::Vec3 distance = x.Get_Pos() - pos;
						auto ball = ge->GetTask_One_G<Ball::Object>("�{�[��");
						ball->Teleportation(pos + (distance*0.01f));
							
						ge->game.lock()->Game_Clear();							
					}
					break;
				//���̓e���|�[�g
				case BoxType::Teleportaion:						
					if (x.Get_Collision_Bool(all_Points, pos, r, speed))
					{						
						auto ball = ge->GetTask_One_G<Ball::Object>("�{�[��");
						ML::Vec3 exitpos;
						if (ge->TM.Find_Exit(this->sideNumber,&exitpos))
						{								
							ball->Teleportation(exitpos);
							auto eff = ge->eff_Manager.lock();
							//�e���|�[�g�C���̃G�t�F�N�g
							eff->Add_Effect(pos, x.Get_Pos(), ML::Vec3(0, 0, 0), BEffect::effType::Teleportin);
							//�e���|�[�g�C���̉���炷
							DM::Sound_Play(this->res->seTeleportIn, false);
							//�e���|�[�g�A�E�g�̃G�t�F�N�g
							eff->Add_Effect(exitpos, this->Normal_Side, BEffect::effType::TeleportOut);
							//�e���|�[�g�A�E�g�̉���炷
							DM::Sound_Play(this->res->seTeleportOut, false);
						}
					}
					break;
				case BoxType::LightSwitch:
					if (x.Get_Collision_Bool(all_Points, pos, r, speed))
					{
						//�J�����}���Ƀ��C�g��3�b�ԃI�t���閽�߂𑗂�
						ge->GetTask_One_G<CameraMan::Object>("�J�����}��")->Turnoff_the_Light();
					}
					break;
				//�ǂ͂����̂����蔻��
				case BoxType::Wall:
					x.Get_Collision_Poligon(&this->col_Poligons, all_Points, pos, r, speed);
					break;
				}
				//this->collision_Tri = this->col.Hit_Check(Mass, pos, r, this->map_QT); //(ver0.2�Ŏg��������)				

				//�S�̏Փˌ��ʂɕۑ�����
				for (auto& c : this->col_Poligons)
				{
					ge->collision_Result.push_back(c);
				}
			}
			
		}
		
		return ball_on_This_Side;
	}

	//------------------------------------------------------------------------
	//��]������
	void Object::Map_Rotate(const ML::QT& qt)
	{
		//�N�H�[�^�j�I���X�V
		this->UpDate_Quartanion(qt);
		for (auto& z : this->arr)
		{
			for (auto& x : z)
			{

				ML::Vec3 pos = x.Get_Pos();
				//pos += ge->Map_center - ML::Vec3((this->mapSize*this->chipSize / 2), -((this->mapSize-2)*this->chipSize / 2), (this->mapSize*this->chipSize / 2));
				//D3DXMatrixAffineTransformation(&matR, this->chipSize / 100.0f, &ML::Vec3(1050,50,1050), &qtW, NULL);
				//matR.Inverse();

				/*if (this->map_Rotation.Length() != 0)
				{
				ML::Mat4x4 matMove;
				D3DXMatrixAffineTransformation(&matMove, this->chipSize / 100.0f, &ge->Map_center, &qtM, NULL);
				matR *= matMove;
				}*/
				ML::Mat4x4 matR;
				D3DXMatrixAffineTransformation(&matR, this->chipSize / 100.0f, &ge->Map_center, &qt, NULL);
				//ML::Vec3 temp = matR.TransformCoord(this->arr[z][y][x].Get_Pos());
				pos = matR.TransformCoord(pos);

				//matR.Inverse();
				x.Rotate_Box(pos, this->map_QT);
				//�e���|�[�g���̈ʒu�X�V
				if (x.What_Type_Is_this_Box() == BoxType::Teleportaion)
				{
					ge->TM.Update_Door_Position(this->sideNumber, pos);
				}
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
		MyMath::Vector_Dot(&c, sv.Normalize(), this->Normal_Side);

		//���ϒl��-90 < cos < 90�̊Ԃ̓����_�����O�����Ȃ�
		return c > this->rendering_Judge? false : true;
	}
	//-----------------------------------------------------------------------------------------
	//�}�b�v�̖@���x�N�g����Ԃ����\�b�h
	void Object::Get_Normal_Side(ML::Vec3* result)const
	{
		*result = this->Normal_Side;
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