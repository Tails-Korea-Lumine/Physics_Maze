#pragma once
#pragma warning(disable:4996)
#include "GameEngine_Ver3_7.h"
//瞬間移動を管理するクラス

//ドアは1面あたり1個以下が存在して、全数は偶数で存在する
//0<->3
//1<->4
//2<->5
//のようにつながっている

class Teleporation_Manager
{
private:
	//瞬間移動の扉の座標
	ML::Vec3 doorPos[6];

public:
	//座標を初期化
	void Init_Door_Position(int sidenum, const ML::Vec3& pos);
	//
};