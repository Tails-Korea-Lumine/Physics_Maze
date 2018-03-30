//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Map3d.h"
#include  "Task_Ball.h"

namespace  Map3d
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
	bool  Object::Initialize()
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();


		//�f�[�^�̃[���N���A
		ZeroMemory(this->arr, sizeof(this->arr));
		this->sizeX = 0;
		this->sizeY = 0;
		this->sizeZ = 0;
		this->hitBase = ML::Box3D(0, 0, 0, 0, 0, 0);
		this->chipSize = 100.0f;
		for (int i = 0; i < _countof(this->chipName); i++)
		{
			this->chipName[i] = "";
		}

		this->collision_Tri = { false,ML::Vec3(0,0,0) };
		//���f�[�^������
		this->Map_Load("./data/StageData/Map00.txt");

		this->render3D_Priority[0] = 1.0f;

		this->controllor_Volume = 0.0f;
		
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
		auto in1 = DI::GPad_GetState("P1");

		this->controllor_Volume = in1.LStick.volume;

		//���[���h��]�ʂɉ����ĉ�]
		this->Map_Rotate();
		//�����蔻��͖���}�b�v�̂ق��ōs��
		//�{�[���̏����C��
		auto ball = ge->GetTask_One_G<Ball::Object>("�{�[��");
		
		//�{�[���ƃ}�b�v�̂����蔻��
		this->Map_Check_Hit(ball->pos, ball->r);
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		
	}

	void  Object::Render3D_L0()
	{
		//����]�s��
		ML::Mat4x4 matRX, matRY, matRZ;

		//���[���h��]�ʂ���s��ɔ��f
		matRX.RotationX(this->controllor_Volume);
		matRY.RotationY(this->controllor_Volume);
		matRZ.RotationZ(this->controllor_Volume);

		ML::Mat4x4 matS;
		matS.Scaling(this->chipSize / 100.0f);
		for (int y = 0; y < this->sizeY; y++)
		{
			for (int z = 0; z < this->sizeZ; z++)
			{
				for (int x = 0; x < this->sizeX; x++)
				{
					int cn = this->arr[z][y][x].chip;
					if (cn == 0)
					{
						continue;
					}
					 
					ML::Mat4x4 matT;
					matT.Translation(this->arr[z][y][x].pos);
					ML::Mat4x4 matW;
					matW = matS *  matRY  * matT;
					DG::EffectState().param.matWorld = matW;
					DG::Mesh_Draw(this->chipName[cn]);
				}
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
			this->chipName[c] = "MapChip" + std::to_string(c);
			DG::Mesh_CreateFromSOBFile(this->chipName[c], chipFilePath);
		}
		//�}�b�v�z��T�C�Y�̓ǂݍ���
		fin >> this->sizeX >> this->sizeY >> this->sizeZ;
		this->hitBase = ML::Box3D(0, 0, 0,
			this->sizeX*(int)this->chipSize,
			this->sizeY*(int)this->chipSize,
			this->sizeZ*(int)this->chipSize);
		//�}�b�v�z��f�[�^�̓ǂ݂�����
		for (int y = 0; y < this->sizeY; y++)
		{
			for (int z = this->sizeZ - 1; z >= 0; z--)
			{
				for (int x = 0; x < this->sizeX; x++)
				{
					fin >> this->arr[z][y][x].chip;
					this->arr[z][y][x].pos = ML::Vec3(x*this->chipSize + this->chipSize / 2, y*this->chipSize + this->chipSize / 2, z*this->chipSize + this->chipSize / 2);					
					this->arr[z][y][x].collision_Base = ML::Box3D(-50, -50, -50, 100, 100, 100);
				}
			}
		}
		fin.close();
		return true;
	}

	//�����蔻��
	bool Object::Map_Check_Hit(const ML::Box3D& hit)
	{
		struct Box3D_2point
		{
			int fx, fy, fz;//�l�����������̓_
			int bx, by, bz;//�l���傫�����̓_
		};

		Box3D_2point r =
		{
			hit.x , hit.y , hit.z,
			hit.x + hit.w , hit.y + hit.h , hit.z + hit.d
		};

		Box3D_2point m =
		{
			this->hitBase.x , this->hitBase.y, this->hitBase.z,
			this->hitBase.x + this->hitBase.w , this->hitBase.y + this->hitBase.h, this->hitBase.z + this->hitBase.d
		};

		//�L�����N�^�̋�`���}�b�v�͈͓̔��Ɋۂߍ���
		if (r.fx < m.fx) { r.fx = m.fx; }
		if (r.fy < m.fy) { r.fy = m.fy; }
		if (r.fz < m.fz) { r.fz = m.fz; }
		if (r.bx > m.bx) { r.bx = m.bx; }
		if (r.by > m.by) { r.by = m.by; }
		if (r.bz > m.bz) { r.bz = m.bz; }

		//�L�����N�^���}�b�v�̊O�Ɋ��S�ɏo�Ă����画��I��
		if (r.bx <= r.fx) { return false; }
		if (r.by <= r.fy) { return false; }
		if (r.bz <= r.fz) { return false; }

		//���[�v�͈͐ݒ�
		int sx, sy, sz, ex, ey, ez;
		sx = r.fx / (int)this->chipSize;
		sy = r.fy / (int)this->chipSize;
		sz = r.fz / (int)this->chipSize;
		ex = (r.bx - 1) / (int)this->chipSize;
		ey = (r.bx - 1) / (int)this->chipSize;
		ez = (r.bz - 1) / (int)this->chipSize;

		//�ڐG����
		for (int z = sz; z <= ez; z++)
		{
			for (int y = sy; y <= ey; y++)
			{
				for (int x = sx; x <= ex; x++)
				{
					if (this->arr[z][y][x].chip > 0)
					{
						return true;
					}
				}
			}
		}
		return false;//�ڐG���Ă�����̂��Ȃ�
	}
	//���ƃ}�b�v�̂����蔻��
	void Object::Map_Check_Hit(ML::Vec3 pos, float r)
	{
		//���͈̔�
		//�}�b�v�̓ǂݍ��݂��m��������ɍĒ�����������(2018/03/28)
		int sx, sy, sz;
		int ex, ey, ez;

		sx = (pos.x / 100) -1;
		sy = (pos.y / 100) -1;
		sz = (pos.z / 100) -1;
		ex = sx + 2;
		ey = sy + 2;
		ez = sz + 2;

		if (sx < 0 || sy < 0 || sz < 0)
		{
			return;
		}
		if (ex > this->sizeX || ey > this->sizeY || ez > this->sizeZ)
		{
			return;
		}
		
		//����X�^�[�g
		for (int y = sy; y <= ey; y++)
		{
			for (int z = sz; z <= ez; z++)
			{
				for (int x = sx; x <= ex; x++)
				{
					//���͔��肵�Ȃ�
					if (this->arr[z][y][x].chip == 0)
					{
						continue;
					}
					//���肷��}�X���C������
					ML::Box3D Mass = this->arr[z][y][x].collision_Base.OffsetCopy(this->arr[z][y][x].pos);
					this->collision_Tri = this->col.Hit_Check(Mass, pos, r);
					//����œ��������珈�����~�߂�
					if (this->is_Collision().collision_Flag)
					{
						return;
					}
				}
			}
		}
	}
	

	//-----------------------------------------------------------------------
	//�������Ă��邩��Ԃ��֐�
	After_Collision Object::is_Collision()
	{
		return this->collision_Tri;
	}

	//------------------------------------------------------------------------
	//��]������
	void Object::Map_Rotate()
	{
		//����]�s��
		ML::Mat4x4 matRX, matRY, matRZ;

		//���[���h��]�ʂ���s��ɔ��f
		matRX.RotationX(ge->World_Rotation.x);
		matRY.RotationY(ge->World_Rotation.y);
		matRZ.RotationZ(ge->World_Rotation.z);

		for (int y = 0; y < this->sizeY; y++)
		{
			for (int z = 0; z < this->sizeZ; z++)
			{
				for (int x = 0; x < this->sizeX; x++)
				{
					this->arr[z][y][x].pos -= ML::Vec3(1000, 0, 1000);
					this->arr[z][y][x].pos = matRX.TransformCoord(this->arr[z][y][x].pos);
					this->arr[z][y][x].pos = matRY.TransformCoord(this->arr[z][y][x].pos);
					this->arr[z][y][x].pos = matRZ.TransformCoord(this->arr[z][y][x].pos);
					this->arr[z][y][x].pos += ML::Vec3(1000, 0, 1000);
				}
			}
		}
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