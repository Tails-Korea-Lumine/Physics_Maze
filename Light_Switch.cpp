#include "Light_Switch.h"
#include "MyPG.h"
#include "Task_CameraMan.h"

bool Light_Switch::Collision_Action(std::vector<Collision_Data>* result, Shape3D* ball) 
{	
	//�����蔻�茋�ʂ�result�ɕۑ�
	if (!this->collision_Base->Hit(result, ball))
	{
		//������Ȃ���������false��Ԃ�
		return false;
	}
	//�J�����}���Ƀ��C�g��3�b�ԃI�t���閽�߂𑗂�
	ge->GetTask_One_G<CameraMan::Object>("�J�����}��")->Turnoff_the_Light();
	//����͓������������������͂��Ȃ��̂��]�܂����̂�false��Ԃ�
	return false;
}

void Light_Switch::Rendering() const
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

Light_Switch::Light_Switch(const ML::Vec3& pos, const ML::Vec3& half_Of_Length, const ML::QT& qt, const string& id, const string& mesh_Name)
	:Bbox(pos, half_Of_Length, qt, id,mesh_Name)
{
	this->chip = Bbox::BoxType::LightSwitch;	
}