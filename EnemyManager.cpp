#include <imgui.h>
#include "EnemyManager.h"
#include "Collision.h"
#include "EnemySlime.h"

// �X�V����
void EnemyManager::Update(float elapsedTime)
{
	for (Enemy* enemy : enemies)
	{
		enemy->Update(elapsedTime);
	}

	// �j������
	// ��enemies�͈̔�for������erase()����ƕs����������Ă��܂����߁A
	// �@�X�V�������I�������ɔj�����X�g�ɐς܂ꂽ�I�u�W�F�N�g���폜����B
	for (Enemy* enemy : removes)
	{
		// std::vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜���Ȃ���΂Ȃ�Ȃ�
		std::vector<Enemy*>::iterator it = std::find(enemies.begin(), enemies.end(), enemy);
		if (it != enemies.end())
		{
			enemies.erase(it);
		}

		// �폜
		delete enemy;
	}
	// �j�����X�g���N���A
	removes.clear();

	// �G���m�̏Փˏ���
	CollisionEnemyVsEnemies();
}

// �`�揈��
void EnemyManager::Render(ID3D11DeviceContext* context, Shader* shader)
{
	for (Enemy* enemy : enemies)
	{
		enemy->Render(context, shader);
	}
}

// �G�l�~�[�o�^
void EnemyManager::Register(Enemy* enemy)
{
	enemies.emplace_back(enemy);
}

// �G�l�~�[�S�폜
void EnemyManager::Clear()
{
	for (Enemy* enemy : enemies)
	{
		delete enemy;
	}
	enemies.clear();
}

// �G�l�~�[�폜
void EnemyManager::Remove(Enemy* enemy)
{
	// �j�����X�g�ɒǉ�
	removes.insert(enemy);
}

// �G�l�~�[���m�̏Փˏ���
void EnemyManager::CollisionEnemyVsEnemies()
{
	size_t enemyCount = enemies.size();
	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemyA = enemies.at(i);
		for (int j = i + 1; j < enemyCount; ++j)
		{
			Enemy* enemyB = enemies.at(j);

			DirectX::XMFLOAT3 outPositionA, outPositionB;
			if (Collision::IntersectCylinderVsCylinderEx(
				enemyA->GetPosition(),
				enemyA->GetRadius(),
				enemyA->GetHeight(),
				enemyB->GetPosition(),
				enemyB->GetRadius(),
				enemyB->GetHeight(),
				outPositionB,
				outPositionA))
			{
				enemyA->SetPosition(outPositionA);
				enemyB->SetPosition(outPositionB);
			}
		}
	}
}