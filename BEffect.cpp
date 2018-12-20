//-------------------------------------------------------------------------
//�G�t�F�N�g��b�N���X
//-------------------------------------------------------------------------
#include "BEffect.h"
#include "easing.h"
#include "MyMath.h"
#include "MyPG.h"


void BEffect::Effect_Draw()const
{
	if (this->effect_Life <= 0)
	{
		return;
	}
	//�`��p�s�񐶐�
	ML::Mat4x4 matS, matRT;
	matS.Scaling(this->scale);
	//��]�N�H�[�^�j�I��
	ML::QT qt = this->Calc_Qt();
	D3DXMatrixAffineTransformation(&matRT, 1.0f, NULL, &qt, &this->pos);

	//�����x��ݒ�
	auto bsBuf = DG::EffectState().BS_Get();
	DG::EffectState().BS_Alpha();
	//�G�t�F�N�g���������x�ŕ`��
	DG::EffectState().param.objectColor = ML::Color(this->alpha, 1, 1, 1);
	DG::EffectState().param.matWorld = matS * matRT;
	DG::Mesh_Draw(this->meshName);

	//�����x�����ɖ߂�
	DG::EffectState().BS_Set(bsBuf);
	DG::EffectState().param.objectColor = ML::Color(1, 1, 1, 1);
}

void BEffect::Dec_Eff()
{
	//�c�莞�Ԍ���
	this->effect_Life--;
}

bool BEffect::Is_Alive() const
{
	return this->effect_Life > 0;
}

ML::QT BEffect::Calc_Qt() const
{
	//��]���v�Z
	ML::Vec3 yplus = ML::Vec3(0, 1, 0);
	ML::Vec3 anker;
	MyMath::Get_Normal_to_Vector_Cross(&anker, yplus, this->angle);
	//��]�ʌv�Z
	float cos;
	MyMath::Vector_Dot(&cos, yplus, this->angle);
	//�N�H�[�^�j�I���̐����ƃA�t�B���ϊ�
	return ML::QT(anker, acos(cos));
}


BEffect::BEffect(const ML::Vec3& pos, const ML::Vec3& angle)
{
	this->pos = pos;
	this->angle = angle;
	
}
BEffect::BEffect(const ML::Vec3& pos, const ML::Vec3& target, const ML::Vec3& angle)
{
	this->pos = pos;
	this->target = target;
	this->angle = angle;	
}