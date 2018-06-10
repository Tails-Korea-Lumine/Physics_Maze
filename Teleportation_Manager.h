#pragma once
#pragma warning(disable:4996)
#include "myLib.h"
//瞬間移動を管理するクラス

//ドアは1面あたり1個以下が存在して、全の数は偶数で存在する
//0<->3
//1<->4
//2<->5
//のようにつながっている

class Teleportation_Manager
{
private:
	//瞬間移動の扉の座標
	ML::Vec3 doorPos[6];
	//各扉の使用されたかを判別するフラグ
	bool be_Used[6];
	//時間判別用カウンタ
	int timeCnt[6];

public:
	//座標を更新
	void Update_Door_Position(const int& sidenum, const ML::Vec3& pos);
	//出口を探す関数 引数 : (当たり判定が起きた面の番号)
	bool Find_Exit(const int& sidenum, ML::Vec3* exitpos);
	//フレームに1ずつカウンタを上げる関数
	void Increse_Counter();

	//コンストラクタ・デストラクタ
	//ゼロクリア
	Teleportation_Manager();	

	~Teleportation_Manager() 
	{}
};