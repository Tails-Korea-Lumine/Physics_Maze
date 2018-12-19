#include "Goal.h"
#include "MyPG.h"

bool Goal::Collision_Action(std::vector<Collision_Data>* result, Shape3D* ball) 
{	
	//�����蔻�茋�ʂ�result�ɕۑ�
	if (!this->collision_Base->Hit(result, ball))
	{
		//������Ȃ���������false��Ԃ�
		return false;
	}

	//�Q�[���^�X�N�ɃN���A������������
	ge->game.lock()->Game_Clear();
	return true;
}

void Goal::Rendering() const
{
	//�s�񐶐�
	ML::Mat4x4 matW;	
	//�A�t�B���ϊ�
	D3DXMatrixAffineTransformation(&matW, 100.0f, NULL, &this->collision_Base->Get_Quaternion(), &this->collision_Base->Get_Center());
	//���[���h�s��ɏ㏑��
	DG::EffectState().param.matWorld = matW;
	//�����_�����O
	DG::Mesh_Draw(this->mesh_Name);
}

Goal::Goal(const ML::Vec3& pos, const ML::Vec3& half_Of_Length, const ML::QT& qt, const string& id, const string& mesh_Name)
	:Bbox(pos, half_Of_Length, qt, id,mesh_Name)
{
	this->chip = Bbox::BoxType::Goal;
}