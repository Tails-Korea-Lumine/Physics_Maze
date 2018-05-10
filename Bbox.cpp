#include "Bbox.h"


void Bbox::Rotate_Box(ML::Vec3 pos, ML::QT q)
{
	this->pos = pos;
	this->boxQT = q;
}

void Bbox::Get_Collision_Poligon(std::vector<After_Collision>* result, ML::Vec3 pos, float r, ML::Vec3 speed)
{
	//当たらなかった時だけゼロベクトルリザルトをpushbackする
	if (!this->col.Hit_Check(result, this->collision_Base.OffsetCopy(this->pos), pos, r, speed, this->boxQT))
	{
		result->push_back(After_Collision());
	}
}

BoxType Bbox::What_Type_Is_this_Box()
{
	return this->chip;
}

ML::Vec3 Bbox::Get_Pos()
{
	return this->pos;
}

Bbox::Bbox()
{
	this->chip = BoxType::Clear;
	this->pos = ML::Vec3(0, 0, 0);
	this->collision_Base = ML::Box3D(0, 0, 0, 0, 0, 0);
	this->boxQT = ML::QT(0.0f);
}

Bbox::Bbox(BoxType chip, ML::Vec3 pos, ML::Box3D base, ML::QT qt)
{
	this->chip = chip;
	this->pos = pos;
	this->collision_Base = base;
	this->boxQT = qt;
}

//クリア判定
bool Bbox::Player_was_Clear_the_Game(ML::Vec3 pos, float r, ML::Vec3 speed)
{
	//あたり判定の結果をもらう
	std::vector<After_Collision> check;
	this->col.Hit_Check(&check, this->collision_Base.OffsetCopy(this->pos), pos, r, speed, this->boxQT);
	//結果のサイズが０ではないならクリア
	if (check.size() != 0)
	{
		return true;
	}
	return false;
}