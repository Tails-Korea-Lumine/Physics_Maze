#include "Breaking.h"
#include "MyPG.h"


void Breaking::Effect_Update()
{
	if (this->effect_Life <= 0)
	{
		return;
	}

	//XZ������100�܂ő傫���Ȃ�
	this->scale.x += 20.0f;
	this->scale.z += 20.0f;
	this->scale.x = min(this->scale.x, 100.0f);
	this->scale.z = min(this->scale.z, 100.0f);

	//�ړ����x�ƕ����𐶐�
	ML::Vec3 speed = ML::Vec3(0.0f, sinf(this->effect_Life) * 17.0f, 0.0f);
	//��]�N�H�[�^�j�I���𐶐�����
	ML::QT qt = this->Calc_Qt();
	//��]�s�񐶐�
	ML::Mat4x4 mat;
	mat.RotationQuaternion(qt);
	//���x�����̕���]����
	speed = mat.TransformCoord(speed);

	this->pos += speed;
	//�����x�͂͂�����Ȃ����u�Ԃ��瓧���ɂȂ��Ă���
	this->alpha = sinf(this->effect_Life);
}

//void Breaking::Effect_Draw() const
//{
//	if (this->effect_Life <= 0)
//	{
//		return;
//	}
//
//	//�`��p�s�񐶐�
//	ML::Mat4x4 matS, matRT;
//	matS.Scaling(this->scale);
//	//��]�N�H�[�^�j�I��
//	ML::QT qt = this->Calc_Qt();
//	D3DXMatrixAffineTransformation(&matRT, 1.0f, NULL, &qt, &this->pos);
//
//	//�����x��ݒ�
//	auto bsBuf = DG::EffectState().BS_Get();
//	DG::EffectState().BS_Alpha();
//	//�G�t�F�N�g���������x�ŕ`��
//	DG::EffectState().param.objectColor = ML::Color(this->alpha, 1, 1, 1);
//	DG::EffectState().param.matWorld = matS * matRT;
//	DG::Mesh_Draw(this->meshName);
//
//	//�����x�����ɖ߂�
//	DG::EffectState().BS_Set(bsBuf);
//	DG::EffectState().param.objectColor = ML::Color(1, 1, 1, 1);
//}

Breaking::Breaking(const ML::Vec3& pos, const ML::Vec3& angle, const string& mesh_name, const string& sound_Name)
	:BEffect(pos, angle)
{
	this->meshName = mesh_name;
	this->sound_Name = sound_Name;
	this->alpha = 1.0f;
	this->effect_Life = 0.5f;
	this->scale = ML::Vec3(50, 50, 50);

	if (this->sound_Name != "")
	{
		DM::Sound_Play(this->sound_Name, false);
	}
}