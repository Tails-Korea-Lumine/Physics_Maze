#include "Game_Time.h"
#include <Windows.h>
#include <time.h>
#include <timeapi.h>
#pragma	comment(lib,"winmm")	//	マルチメディア拡張機能を使用するために必要
#include <iostream>


void Game_Time::Update()
{
	//現在時間を取る
	unsigned int now_Time = timeGetTime();
	//保存されている以前時間からの経過時間を1000で割ってデルタタイムを計算する
	this->delta_Time = (now_Time - this->prev_Time) / 1000.0f;
	//現在時間を以前時間に上書きする
	this->prev_Time = now_Time;
}

float Game_Time::Delta_Time() const
{
	return this->delta_Time;
}

Game_Time::Game_Time()
{
	this->prev_Time = timeGetTime();
	this->delta_Time = 0.0f;
}