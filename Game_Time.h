#pragma once
//#include <timeapi.h>


//�f���^�^�C�����v�Z����N���X


class Game_Time
{
private:
	unsigned int prev_Time;
	float delta_Time;


public:
	//���ݎ��Ԃ�������ăf���^�^�C���v�Z
	void Update();
	//�Q�b�^�[
	//�f���^�^�C��
	float Delta_Time() const;

	//�R���X�g���N�^
	Game_Time();
	
};