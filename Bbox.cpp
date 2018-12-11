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
		this->collision_Base->Marking_On_Unsuable_Poligon(6) ;
		this->collision_Base->Marking_On_Unsuable_Poligon(7) ;
		break;
	case Box_Side::Xminus:
		this->collision_Base->Marking_On_Unsuable_Poligon(2) ;
		this->collision_Base->Marking_On_Unsuable_Poligon(3) ;
		break;
	case Box_Side::Yplus:
		this->collision_Base->Marking_On_Unsuable_Poligon(8) ;
		this->collision_Base->Marking_On_Unsuable_Poligon(9) ;
		break;
	case Box_Side::Yminus:
		this->collision_Base->Marking_On_Unsuable_Poligon(10);
		this->collision_Base->Marking_On_Unsuable_Poligon(11);
		break;
	case Box_Side::Zplus:
		this->collision_Base->Marking_On_Unsuable_Poligon(4) ;
		this->collision_Base->Marking_On_Unsuable_Poligon(5) ;
		break;
	case Box_Side::Zminus:
		this->collision_Base->Marking_On_Unsuable_Poligon(0) ;
		this->collision_Base->Marking_On_Unsuable_Poligon(1) ;
		break;
	}
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
	
}
//引数 : (箱のタイプ,位置,あたり判定矩形,初期回転量,ボックスのID)
Bbox::Bbox(const ML::Vec3& pos, const ML::Vec3& half_Of_Length, const ML::QT& qt, const string id)
	:box_Id(id)
{
	this->chip = BoxType::Clear;
	//this->pos = pos;
	this->collision_Base = new Cube(pos, half_Of_Length, qt);
	//this->boxQT = qt;	
}