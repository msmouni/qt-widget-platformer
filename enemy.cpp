#include "enemy.h"

Enemy::Enemy(const QRectF &rect, const QString &res_path, const Platform &platform, const QRectF &player_rect) : Character(rect, res_path, platform), m_player_rect(player_rect)
{
    m_type = CharacterType::Enemy;

    connect(&m_path_finder, SIGNAL(pathFindingRes(QVector<QPoint>)), this, SLOT(setPathFindingResult(QVector<QPoint>)));
}

void Enemy::gameUpdate()
{
    if (isOnGround())
    {
        m_path_finder.findPath(scenePos(), m_player_rect.center());
    }

    followPath();
    updateCharacter();
}

void Enemy::setPathFindingResult(QVector<QPoint> path)
{
    QMutexLocker ml(&m_path_mutex);
    m_path_tiles = path;
}

void Enemy::followPath()
{
    QMutexLocker ml(&m_path_mutex);
}
