#include "Bbox.h"


void Bbox::Rotate_Box(const ML::Vec3& pos, const ML::QT& q)
{
	//回転量と位置を更新
	this->pos = pos;
	this->boxQT = q;
}

void Bbox::Get_Collision_Poligon(std::vector<After_Collision>* result, const ML::Vec3& pos, const float& r, const ML::Vec3& speed)
{
	//あたり判定結果をresultに保存
	if (!this->col.Hit_Check(result, this->collision_Base.OffsetCopy(this->pos), pos, r, speed, this->boxQT))
	{
		//当たらなかった時だけゼロベクトルリザルトをpushbackする
		result->push_back(After_Collision());
	}
}

BoxType Bbox::What_Type_Is_this_Box() const
{
	return this->chip;
}

ML::Vec3 Bbox::Get_Pos() const
{
	return this->pos;
}

//引数なしコンストラクタ(ゼロクリア)
Bbox::Bbox()
{
	this->chip = BoxType::Clear;
	this->pos = ML::Vec3(0, 0, 0);
	this->collision_Base = ML::Box3D(0, 0, 0, 0, 0, 0);
	this->boxQT = ML::QT(0.0f);
}
//引数 : (箱のタイプ,位置,あたり判定矩形,初期回転量)
Bbox::Bbox(BoxType chip, ML::Vec3 pos, ML::Box3D base, ML::QT qt)
{
	this->chip = chip;
	this->pos = pos;
	this->collision_Base = base;
	this->boxQT = qt;
}

//クリア判定
bool Bbox::Player_was_Clear_the_Game(const ML::Vec3& pos, const float& r, const ML::Vec3 speed)
{
	return this->Get_Collision_Bool(pos, r, speed);
}

//テレポートさせるかを判定
bool Bbox::Player_was_Hit_the_Door(const ML::Vec3& pos, const float& r, const ML::Vec3 speed)
{
	return this->Get_Collision_Bool(pos, r, speed);
}

//ライトをオフするかを判定
bool Bbox::Player_Turnoff_the_Switch(const ML::Vec3& pos, const float& r, const ML::Vec3 speed)
{
	return this->Get_Collision_Bool(pos, r, speed);
}

//あたってるかあたってないか論理的な結果だけを求める処理
bool Bbox::Get_Collision_Bool(const ML::Vec3& pos, const float& r, const ML::Vec3& speed)
{
	//あたり判定の結果をもらう
	std::vector<After_Collision> check;
	this->col.Hit_Check(&check, this->collision_Base.OffsetCopy(this->pos), pos, r, speed, this->boxQT);
	//結果のサイズが０ではないならあたっている
	if (check.size() != 0)
	{
		return true;
	}
	return false;
}