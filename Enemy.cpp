#include "Enemy.h"
#include "EnemyManager.h"
#include "Graphics/Graphics.h"

// �j��
void Enemy::Destroy()
{
	EnemyManager::Instance().Remove(this);
}
