#include "Enemy.h"
#include "EnemyManager.h"
#include "Graphics/Graphics.h"

// ”jŠü
void Enemy::Destroy()
{
	EnemyManager::Instance().Remove(this);
}
