#include "Bbox.h"
#include "Cube.h"
#include "MyMath.h"

#define TRIANGLE_ON_CUBE 12
#define VERTEX_ON_CUBE 8


void Bbox::Rotate_Box(ML::Mat4x4* mat, const ML::QT& q)
{
	//回転量と位置を更新
	this->collision_Base->Rotation(mat, q);
}


//あたり判定に必要ない三角形を表示しておく
void Bbox::Marking_On_Unusable_Side(const Box_Side& side)
{
	//引数の方向に相当する番号のところをマーキング
	switch (side)
	{
	case Box_Side::Xplus:
		this->unusable_Triagle[6] = true;
		this->unusable_Triagle[7] = true;
		break;
	case Box_Side::Xminus:
		this->unusable_Triagle[2] = true;
		this->unusable_Triagle[3] = true;
		break;
	case Box_Side::Yplus:
		this->unusable_Triagle[8] = true;
		this->unusable_Triagle[9] = true;
		break;
	case Box_Side::Yminus:
		this->unusable_Triagle[10] = true;
		this->unusable_Triagle[11] = true;
		break;
	case Box_Side::Zplus:
		this->unusable_Triagle[4] = true;
		this->unusable_Triagle[5] = true;
		break;
	case Box_Side::Zminus:
		this->unusable_Triagle[0] = true;
		this->unusable_Triagle[1] = true;
		break;
	}
}

bool Bbox::Get_Marking_Unsuable_Side(const int& i)
{
	//範囲外ならfalseを返す
	if (i > TRIANGLE_ON_CUBE || i < 0)
	{
		return false;
	}

	//判定がいらない三角形ならtrueを返す
	return this->unusable_Triagle[i];
}

Bbox::BoxType Bbox::What_Type_Is_this_Box() const
{
	return this->chip;
}

ML::Vec3 Bbox::Get_Pos() const
{
	return this->collision_Base->Get_Center();
}

string Bbox::Get_Id() const
{
	return this->box_Id;
}

//引数なしコンストラクタ(ゼロクリア)
Bbox::Bbox()
{
	this->chip = BoxType::Clear;
	//this->pos = ML::Vec3(0, 0, 0);
	this->collision_Base = nullptr;
	//this->boxQT = ML::QT(0.0f);
	for (int i = 0; i < TRIANGLE_ON_CUBE; i++)
	{
		this->unusable_Triagle[i] = true;
	}
}
//引数 : (箱のタイプ,位置,あたり判定矩形,初期回転量,ボックスのID)
Bbox::Bbox(const ML::Vec3& pos, const ML::Vec3& half_Of_Length, const ML::QT& qt, const string id)
	:box_Id(id)
{
	this->chip = BoxType::Clear;
	//this->pos = pos;
	this->collision_Base = new Cube(pos, half_Of_Length, qt);
	//this->boxQT = qt;
	for (int i = 0; i < TRIANGLE_ON_CUBE; i++)
	{
		this->unusable_Triagle[i] = false;
	}
}