#pragma once
#pragma warning(disable : 4996)
#include "myLib.h"

//-------------------------------------------------------------------------
//�G�t�F�N�g��b�N���X
//-------------------------------------------------------------------------
//�O��Ŏg�p���Ă�����̍�i�ɂ͎g�p���Ȃ��f�[�^���܂܂�Ă���


class BEffect 
{	

public:
	//�ϐ�
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
	enum effType
	{
		EFF_CLEAR = -1,		
		TeleportOut =0,
		Teleportin,		
		Game_Clear,
	};
protected:
	//effType playing_EffectHandle;
	int effect_Life;
	
	ML::Vec3 pos;//�G�t�F�N�g�Đ��ʒu
	ML::Vec3 target;//�^�[�Q�b�g�ʒu(����ꍇ�������炤)
	ML::Vec3 angle;//�G�t�F�N�g�Đ��p�x
	ML::Vec3 scale;//�G�t�F�N�g�̑傫��
	ML::Vec3 moveVec;//�G�t�F�N�g�̈ړ����x
	float alpha;//�����x

	string meshName;//���b�V���̖��O
	string sound_Name;//�T�E���h�G�t�F�N�g�̖��O

public:
	//���\�b�h
	//�������Ń��b�V����ǂݍ��ފ֐�
	//void LoadEffect(const effType& handle);	

	//���z�֐�
	virtual void Effect_Update() = 0;
	virtual void Effect_Draw() const = 0;

	//���\�b�h
	//�G�t�F�N�g�̎c�莞�Ԃ�����������
	void Dec_Eff();
	//�G�t�F�N�g�������Ă��邩���m�F����
	bool Is_Alive() const;	
	
	//�G�t�F�N�g�Z�b�e�B���O
	//void Set_Effect(const ML::Vec3& pos, const ML::Vec3& angle);
	//void Set_Effect(const ML::Vec3& pos, const ML::Vec3& target, const ML::Vec3& angle);

	//��������
	//�[���N���A
	BEffect() :
		pos(0, 0, 0),
		target(0,0,0),
		angle(0, 0, 0),
		scale(0,0,0),
		moveVec(0,0,0),
		//playing_EffectHandle(EFF_CLEAR),
		effect_Life(0),
		alpha(1)
	{
		
	}
	//�R���X�g���N�^
	//�����ꏊ�����ōs���G�t�F�N�g
	//���� : (�����ʒu�A��������)
	BEffect(const ML::Vec3& pos, const ML::Vec3& angle);
	//�ړI�n�܂ňړ����Ȃ��珈������G�t�F�N�g
	//���� : (�����ʒu�A�ړI�n�A��������)
	BEffect(const ML::Vec3& pos, const ML::Vec3& target, const ML::Vec3& angle);

	virtual ~BEffect(){}
};