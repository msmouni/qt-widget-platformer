#include "enemy.h"

Enemy::Enemy(const QRectF &rect, const QString &res_path, const Platform &platform, const QRectF &player_rect) : Character(rect, res_path, platform), m_player_rect(player_rect)
{
    m_type = CharacterType::Enemy;
}

void Enemy::gameUpdate()
{
    updateCharacter();
}
