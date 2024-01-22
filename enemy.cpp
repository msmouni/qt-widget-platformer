#include "enemy.h"

Enemy::Enemy(const QRectF &rect, const QString &res_path, const Platform &platform, const QRectF &player_rect) : Character(rect, res_path, platform), m_path_finder(platform), m_player_rect(player_rect)
{
    m_type = CharacterType::Enemy;

    m_path_iter = M_NB_PATH_ITER;

    connect(&m_path_finder, SIGNAL(pathFindingRes(QVector<QPoint>)), this, SLOT(setPathFindingResult(QVector<QPoint>)));
}

void Enemy::gameUpdate()
{
    if (m_path_tiles.isEmpty() && isOnGround())
    {
        m_path_finder.findPath(scenePos(), m_player_rect.center());
        m_path_iter = M_NB_PATH_ITER;
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

    // TODO: CHANGE METHOD LATER ({USE ACTIONS: LEFT - RIGHT - JUMP - FALL} + {dot_product(next_pos - path[i], path[i] - current_pos) >=0 => reached current target})

    if (!m_path_tiles.isEmpty())
    {
        QPointF currentPos(this->scenePos().x(), this->sceneBoundingRect().bottom());
        QPointF next_tile_pos(m_platform.getTileBottomCenter(m_path_tiles.first()));

        m_speed_x = (next_tile_pos.x() - currentPos.x()) / m_path_iter;
        m_speed_y = (next_tile_pos.y() - currentPos.y()) / m_path_iter;

        m_path_iter -= 1;

        if (m_path_iter == 0)
        {
            m_path_iter = M_NB_PATH_ITER;
            m_path_tiles.pop_front();
        }

        m_speed_y -= (m_acc_y + m_gravity);
        m_speed_y /= m_friction; // Friction

        m_speed_x -= m_acc_x;
        m_speed_x /= m_friction; // Friction
    }
}
