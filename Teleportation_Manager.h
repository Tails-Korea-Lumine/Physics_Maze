#pragma once
#pragma warning(disable:4996)
#include "GameEngine_Ver3_7.h"
//�u�Ԉړ����Ǘ�����N���X

//�h�A��1�ʂ�����1�ȉ������݂��āA�S���͋����ő��݂���
//0<->3
//1<->4
//2<->5
//�̂悤�ɂȂ����Ă���

class Teleporation_Manager
{
private:
	//�u�Ԉړ��̔��̍��W
	ML::Vec3 doorPos[6];

public:
	//���W��������
	void Init_Door_Position(int sidenum, const ML::Vec3& pos);
	//
};