#pragma once
#pragma warning(disable : 4996)
#include "BEffect.h"

//---------------------------------------------------------------------------------
//�G�t�F�N�g�N���X
//---------------------------------------------------------------------------------


class Effect : private BEffect
{
private:
	//---------------------------------------------------------------------------------------------
	//�G�t�F�N�g�n���h��
	//0�ԁF�L�����N�^����
	//1�ԁF�e���|�[�g�A�E�g
	//2�ԁF�e���|�[�g�C��
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


public:

	//�����E����
	Effect() {}

	~Effect() {}

	//�������֐�
	//���̏�őS����������G�t�F�N�g
	void Load_Eff(ML::Vec3 pos, ML::Vec3 angle, effType handle);
	//�ړI�n�܂ňړ����Ȃ��珈������G�t�F�N�g
	void Load_Eff(ML::Vec3 pos,ML::Vec3 target, ML::Vec3 angle, effType handle);
	

	//���\�b�h	
	//�`��
	void Playing_Effect(effType ef);
	//�A�b�v�f�[�g
	void UpDate_Effect(effType ef);	
	//�_�~�[�f�[�^����
	void Set_Dummy();
	//�G�t�F�N�g�̎c�莞�Ԃ�����������
	void Dec_Eff();
	//�G�t�F�N�g�������Ă��邩���m�F����
	bool Is_Alive();
	//�G�t�F�N�g�^�C�v�����炤
	effType Get_Type();
	//remove�ō폜���锻�莮
	static bool Eff_Judge(Effect* e);


	//���Ŋ֐�
	void Finalize();
};