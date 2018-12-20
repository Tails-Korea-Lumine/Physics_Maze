#include "Unstable_Wall.h"
#include "MyPG.h"
#include "Task_Ball.h"

bool Unstable_Wall::Collision_Action(std::vector<Collision_Data>* result, Shape3D* ball) 
{
	//���C�t���c���Ă���Ƃ��̂ݏ���������
	if (this->life <= 0)
	{
		return false;
	}
	//�����蔻�茋�ʂ�result�ɕۑ�
	if (!this->collision_Base->Hit(result, ball))
	{
		//������Ȃ������������[���x�N�g�����U���g��pushback���ďI������
		result->push_back(Collision_Data());
		this->hit_Flag = false;
		return false;
	}	
	//�V�������茋�ʂɂ�ID�����Ă���
	for (auto& r : *result)
	{
		if (r.collision_Id == "")
		{
			r.collision_Id = this->box_Id;
		}
	}
	//�{�[�����x�̃X�J���[�ʕ����C�t������
	this->Decrease_Life(ge->GetTask_One_G<Ball::Object>("�{�[��")->Get_Speed().Length());
	//�G�t�F�N�g����
	this->Be_Effect(result->at(result->size() - 1).normal);
	return true;
}

void Unstable_Wall::Be_Effect(const ML::Vec3& normal)
{
	auto eff = ge->eff_Manager.lock();
	if (this->life <= 0)
	{
		//���ď�����G�t�F�N�g����
		eff->Add_Effect(this->Get_Pos(), normal, BEffect::effType::Crashed);
	}

	if (this->hit_Flag == false)
	{
		//���������G�t�F�N�g����
		eff->Add_Effect(this->Get_Pos(), normal, BEffect::effType::Breaking);
		this->hit_Flag = true;
	}
	
}

void Unstable_Wall::Rendering() const
{
	//���C�t���c���Ă��Ȃ��Ȃ烌���_�����O���Ȃ�
	if (this->life <= 0.0f)
	{
		return;
	}
	//�s�񐶐�
	ML::Mat4x4 matRT, matS;
	//�X�P�[�����O
	matS.Scaling(this->collision_Base->Get_Length()*2.0f);
	//�A�t�B���ϊ�
	D3DXMatrixAffineTransformation(&matRT, 1.0f, NULL, &this->collision_Base->Get_Quaternion(), &this->collision_Base->Get_Center());
	//���[���h�s��ɏ㏑��
	DG::EffectState().param.matWorld = matS * matRT;
	//�����_�����O
	DG::Mesh_Draw(this->mesh_Name);
}

void Unstable_Wall::Decrease_Life(const float& dec)
{
	this->life -= dec;
}

Unstable_Wall::Unstable_Wall(const ML::Vec3& pos, const ML::Vec3& half_Of_Length, const ML::QT& qt, const string& id, const string& mesh_Name)
	:Bbox(pos,half_Of_Length,qt,id,mesh_Name)
{
	this->chip = Bbox::BoxType::Unstable_Wall;
	this->life = 60.0f;
	this->hit_Flag = false;
}