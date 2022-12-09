#include "EnemySlime.h"

// �R���X�g���N�^
EnemySlime::EnemySlime()
{
	model = new Model("Data/Model/Slime/Slime.mdl");

	// ���f�����傫���̂ŃX�P�[�����O
	scale.x = scale.y = scale.z = 0.01f;

	// ���A�����ݒ�
	radius = 0.5f;
	height = 1.0f;
}

// �f�X�g���N�^
EnemySlime::~EnemySlime()
{
	delete model;
}

// �X�V����
void EnemySlime::Update(float elapsedTime)
{
	// ���͏����X�V
	UpdateVelocity(elapsedTime);

	// ���G���ԍX�V
	UpdateInvincibleTimer(elapsedTime);

	// �I�u�W�F�N�g�s����X�V
	UpdateTransform();

	// ���f���s��X�V
	model->UpdateTransform(transform);
}

// �`�揈��
void EnemySlime::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}

// ���S�������ɌĂ΂��
void EnemySlime::OnDead()
{
	// ���g��j��
	Destroy();
}