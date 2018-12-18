#include "Teleport_Out.h"
#include "MyPG.h"
#include "Dm2008_Ver1_3.h"


void Teleport_Out::Effect_Update()
{
	//�t���[���������A�傫���Ȃ�
	this->alpha -= 0.008f;

	this->scale.y += 100.0f;

	//�����x��0�ȉ��ɗ����Ȃ��悤�ɂ���
	this->alpha = max(this->alpha, 0.0f);
}

void Teleport_Out::Effect_Draw() const
{
	if (this->effect_Life <= 0)
	{
		return;
	}
	ML::Mat4x4 matS, matRT;
	matS.Scaling(this->scale);
	/*ML::Mat4x4 matT;
	matT.Translation(this->pos);*/

	ML::Vec3 yplus = ML::Vec3(0, 1, 0);
	ML::Vec3 anker;
	MyMath::Get_Normal_to_Vector_Cross(&anker, yplus, this->angle);
	float cos;
	MyMath::Vector_Dot(&cos, yplus, this->angle);
	ML::QT qt = ML::QT(anker, acos(cos));
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

Teleport_Out::Teleport_Out(const ML::Vec3& pos, const ML::Vec3& angle, const string& mesh_name, const string& sound_Name)
	:BEffect(pos,angle)
{
	this->meshName = mesh_name;
	this->sound_Name = sound_Name;	
	this->alpha = 0.8f;
	this->effect_Life = 80;
	this->scale = ML::Vec3(200, 100, 200);

	//�T�E���h�G�t�F�N�g�Đ�
	if (this->sound_Name != "")
	{
		DM::Sound_Play(this->sound_Name, false);
	}
}


