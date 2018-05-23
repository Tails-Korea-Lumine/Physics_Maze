#pragma once
#pragma warning(disable : 4996)

//---------------------------------------------------------------------------------
//�G�t�F�N�g�`��N���X
//---------------------------------------------------------------------------------
#include "BEffect.h"


class Effect : public BEffect
{
public:
	//���Ŋ֐�
	void Finalize();


	//�����E����
	Effect() {}

	~Effect() {}

	//�������֐�
	void Load_Eff(ML::Vec3 pos, ML::Vec3 angle, effType handle);
	void Load_Eff(ML::Vec3 pos,ML::Vec3 target, ML::Vec3 angle, effType handle);

	//�`�惁�\�b�h
	//�}�l�[�W���[�ŌĂԂ̂͂���炾��
	//�`��
	void Playing_Effect(effType ef);
	//�A�b�v�f�[�g
	void UpDate_Effect(effType ef);

private:
	//---------------------------------------------------------------------------------------------
	//�G�t�F�N�g�n���h��
	//0�ԁF�L�����N�^����
	//1�ԁF�A�C�e������
	//2�ԁF�A�C�e������
	//3�ԁF�G�l�~�[���n
	//4�ԁF�G�l�~�[�ƏՓ�
	//5�ԁF�ǂƏՓ�
	//6�ԁF�����ې���
	//7�ԁF�����ۏ���
	//8�ԁF�����ۂƏՓ�
	//9�ԁF�N���A�G�t�F�N�g
	//---------------------------------------------------------------------------------------------
	//�G�ڌĂ΂Ȃ��Ă��悢�֐�
	//�`��
	void Playing_EF0();
	void Playing_EF1();
	void Playing_EF2();
	void Playing_EF3();
	void Playing_EF4();
	void Playing_EF5();
	void Playing_EF6();
	void Playing_EF7();
	void Playing_EF8();
	void Playing_EF9();
	//�A�b�v�f�[�g
	void UpDate_EF0();
	void UpDate_EF1();
	void UpDate_EF2();
	void UpDate_EF3();
	void UpDate_EF4();
	void UpDate_EF5();
	void UpDate_EF6();
	void UpDate_EF7();
	void UpDate_EF8();
	void UpDate_EF9();
	
};