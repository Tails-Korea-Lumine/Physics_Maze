#include "Game_Time.h"
#include <Windows.h>
#include <time.h>
#include <timeapi.h>
#pragma	comment(lib,"winmm")	//	�}���`���f�B�A�g���@�\���g�p���邽�߂ɕK�v
#include <iostream>


void Game_Time::Update()
{
	//���ݎ��Ԃ����
	unsigned int now_Time = timeGetTime();
	//�ۑ�����Ă���ȑO���Ԃ���̌o�ߎ��Ԃ�1000�Ŋ����ăf���^�^�C�����v�Z����
	this->delta_Time = (now_Time - this->prev_Time) / 1000.0f;
	//���ݎ��Ԃ��ȑO���Ԃɏ㏑������
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