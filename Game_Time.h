#pragma once
//#include <timeapi.h>


//デルタタイムを計算するクラス


class Game_Time
{
private:
	unsigned int prev_Time;
	float delta_Time;


public:
	//現在時間をもらってデルタタイム計算
	void Update();
	//ゲッター
	//デルタタイム
	float Delta_Time() const;

	//コンストラクタ
	Game_Time();
	
};