#include "enemy.h"

Enemy::Enemy(const QRectF &rect, const QString &res_path, const QRectF &player_rect) : Character(rect, res_path), m_player_rect(player_rect)
{
    m_type = CharacterType::Enemy;
}

void Enemy::gameUpdate(const Platform *platform)
{
    updateCharacter(platform);
}
