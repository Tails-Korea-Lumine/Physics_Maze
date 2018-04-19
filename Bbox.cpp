#include "Bbox.h"

//外部ファイルからのマップロード
bool Bbox::Map_Load(string f_)
{
	//外部ファイルから読み込み
	ifstream fin(f_);
	if (!fin)
	{
		return false;
	}
	int chipNum;
	fin >> chipNum;
	//チップファイル名読み込みと、メッシュのロード
	for (int c = 1; c <= chipNum; c++)
	{
		string chipFileName, chipFilePath;
		fin >> chipFileName;
		chipFilePath = "./data/mesh/" + chipFileName;
		this->chipName[c] = "MapChip" + std::to_string(c);
		DG::Mesh_CreateFromSOBFile(this->chipName[c], chipFilePath);
	}
	//マップ配列サイズの読み込み
	fin >> this->sizeX >> this->sizeY >> this->sizeZ;
	this->hitBase = ML::Box3D(0, 0, 0,
		this->sizeX*(int)this->chipSize,
		this->sizeY*(int)this->chipSize,
		this->sizeZ*(int)this->chipSize);
	//マップ配列データの読みおこみ
	for (int y = 0; y < this->sizeY; y++)
	{
		for (int z = this->sizeZ - 1; z >= 0; z--)
		{
			for (int x = 0; x < this->sizeX; x++)
			{
				fin >> this->arr[z][y][x].chip;
				this->arr[z][y][x].pos = ML::Vec3(x*this->chipSize + this->chipSize / 2, y*this->chipSize + this->chipSize / 2, z*this->chipSize + this->chipSize / 2);
				this->arr[z][y][x].collision_Base = ML::Box3D(-this->chipSize / 2, -this->chipSize / 2, -this->chipSize / 2, this->chipSize, this->chipSize, this->chipSize);
			}
		}
	}
	fin.close();
	return true;
}

//あたり判定
bool Bbox::Map_Check_Hit(const ML::Box3D& hit)
{
	struct Box3D_2point
	{
		int fx, fy, fz;//値が小さい側の点
		int bx, by, bz;//値が大きい側の点
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

	//キャラクタの矩形をマップの範囲内に丸め込む
	if (r.fx < m.fx) { r.fx = m.fx; }
	if (r.fy < m.fy) { r.fy = m.fy; }
	if (r.fz < m.fz) { r.fz = m.fz; }
	if (r.bx > m.bx) { r.bx = m.bx; }
	if (r.by > m.by) { r.by = m.by; }
	if (r.bz > m.bz) { r.bz = m.bz; }

	//キャラクタがマップの外に完全に出ていたら判定終了
	if (r.bx <= r.fx) { return false; }
	if (r.by <= r.fy) { return false; }
	if (r.bz <= r.fz) { return false; }

	//ループ範囲設定
	int sx, sy, sz, ex, ey, ez;
	sx = r.fx / (int)this->chipSize;
	sy = r.fy / (int)this->chipSize;
	sz = r.fz / (int)this->chipSize;
	ex = (r.bx - 1) / (int)this->chipSize;
	ey = (r.bx - 1) / (int)this->chipSize;
	ez = (r.bz - 1) / (int)this->chipSize;

	//接触判定
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
	return false;//接触しているものがない
}
//球とマップのあたり判定
void Bbox::Map_Check_Hit(ML::Vec3 pos, float r, ML::Vec3 speed)
{
	//多重衝突まで適用したver0.3(2018/04/16)

	//接触三角形を判定前にクリアする
	After_Collision collision_False;
	collision_False.collision_Flag = false;
	collision_False.normal = ML::Vec3(0, 0, 0);

	this->collision_Tri = collision_False;

	this->col_Poligons.clear();

	//判定スタート
	for (int y = 0; y <= this->sizeY; y++)
	{
		for (int z = 0; z <= this->sizeZ; z++)
		{
			for (int x = 0; x <= this->sizeX; x++)
			{
				//道は判定しない
				if (this->arr[z][y][x].chip == 0)
				{
					continue;
				}
				//一定距離以内のものだけ判定をする
				ML::Vec3 d = this->arr[z][y][x].pos - pos;
				if (d.Length() < 0)
				{
					d *= -1;
				}
				if (d.Length() > 200)
				{
					continue;
				}
				//判定するマスを修得する
				ML::Box3D Mass = this->arr[z][y][x].collision_Base.OffsetCopy(this->arr[z][y][x].pos);
				//this->collision_Tri = this->col.Hit_Check(Mass, pos, r, this->map_QT); //(ver0.2で使った処理)
				After_Collision poligon = this->col.Hit_Check(Mass, pos, r, speed, this->map_QT);

				this->col_Poligons.push_back(poligon);

				//ver0.2で使った処理
				//判定で当たったら処理を止める
				/*if (this->is_Collision().collision_Flag)
				{
				return;
				}*/
			}
		}
	}
}

//------------------------------------------------------------------------
//回転させる
void Bbox::Map_Rotate()
{
	//軸回転クォータニオン
	ML::QT qtM, qtW;

	ML::Mat4x4 matR;


	ML::Mat4x4 matS;
	matS.Scaling(this->chipSize / 100.0f);
	for (int y = 0; y < this->sizeY; y++)
	{
		for (int z = 0; z < this->sizeZ; z++)
		{
			for (int x = 0; x < this->sizeX; x++)
			{


				ML::Vec3 temp = ML::Vec3(x*this->chipSize + this->chipSize / 2, y*this->chipSize + this->chipSize / 2, z*this->chipSize + this->chipSize / 2);

				//D3DXMatrixAffineTransformation(&matR, this->chipSize / 100.0f, &ML::Vec3(1050,50,1050), &qtW, NULL);
				//matR.Inverse();

				/*if (this->map_Rotation.Length() != 0)
				{
				ML::Mat4x4 matMove;
				D3DXMatrixAffineTransformation(&matMove, this->chipSize / 100.0f, &ML::Vec3(1050, 50, 1050), &qtM, NULL);
				matR *= matMove;
				}*/

				D3DXMatrixAffineTransformation(&matR, this->chipSize / 100.0f, &ML::Vec3(1050, 50, 1050), &this->map_QT, NULL);

				/*ML::Mat4x4 mattmp;
				mattmp.RotationQuaternion(this->map_QT);*/
				matR.Inverse();
				this->arr[z][y][x].pos = matR.TransformCoord(temp);
			}
		}
	}
}

Bbox::Bbox()
{
	//データのゼロクリア
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
	this->map_QT = ML::QT(0.0f);
}