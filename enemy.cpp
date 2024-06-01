#include "enemy.h"

Enemy::Enemy(const QPointF &pos, const QString &res_path, const Platform &platform, const QRectF &player_rect) : Character(pos, res_path, platform), m_path_finder(platform), m_player_rect(player_rect)
{
    setData(0, "Enemy");
    m_type = CharacterType::Enemy;

    m_animation->addAnimationState((uint8_t)CharacterState::Attack, res_path + "/Attack");

    connect(&m_path_finder, SIGNAL(pathFindingRes(QVector<QPoint>)), this, SLOT(setPathFindingResult(QVector<QPoint>)));
}

void Enemy::updateKinematics()
{
    if (!isHit() && !isAttacking())
{
    followPath();
    }
    else if (isAttacking())
    {
        // NOTE: When not colling followPath when Attacking or hit -> acceleration is not updated, so the enemy keeps the last value
        m_path_tiles.clear();
        m_dynamics->setAccelX(0);
        m_dynamics->setAccelY(0);
    }

    Character::updateKinematics();
}

void Enemy::gameUpdate()
{
    findPath();

    m_dynamics->setCollisionMargin(isAttacking() ? m_dynamics->getDirection() == EntityDirection::MovingRight ? QMarginsF(0, 0, -15, 0) : QMarginsF(-15, 0, 0, 0) : QMarginsF(0, 0, 0, 0));
    if (sceneBoundingRect().marginsAdded(QMarginsF(3, 0, 3, 0)).intersects(m_player_rect))
    {
        m_attacking = true;
    }

    updateCharacter();
}

void Enemy::setPathFindingResult(QVector<QPoint> path)
{
    QMutexLocker ml(&m_path_mutex);
    m_path_tiles = path;
}

void Enemy::findPath()
{
    QPointF pos = sceneBoundingRect().topLeft();
    QPoint tile_below = m_platform.getTileIdx(pos);
    tile_below.setY(tile_below.y() + 1);

    bool on_ground = m_platform.isInMap(tile_below) && (m_platform.getTileType(tile_below) == TileType::Solid || m_platform.getTileType(tile_below) == TileType::JumpThrough);

    if (on_ground)
    {
        m_path_finder.findPath(pos, m_player_rect.topLeft());
    }
}

void Enemy::checkPathNodeReached()
{
    if (m_path_tiles.length() == 1)
    {
        QPointF next_pos = m_platform.getPosInTile(m_path_tiles[0], this->boundingRect());
        QPointF dist = next_pos - sceneBoundingRect().topLeft();

        if (sqrt(QPointF::dotProduct(dist, dist)) < M_PATH_FOLLOW_PRECISION)
        {
            m_path_tiles.pop_front();
        }
    }
    else if (m_path_tiles.length() > 1)
    {
        QPointF prev_pos = m_platform.getPosInTile(m_path_tiles[0], this->boundingRect());
        QPointF next_pos = m_platform.getPosInTile(m_path_tiles[1], this->boundingRect());

        QPointF dir = next_pos - prev_pos;
        QPointF dist = next_pos - sceneBoundingRect().topLeft();

        if (QPointF::dotProduct(dir, dist) < 0 || sqrt(QPointF::dotProduct(dist, dist)) < M_PATH_FOLLOW_PRECISION)
        {
            m_path_tiles.pop_front();
        }
    }
}

void Enemy::followPath()
{
    QMutexLocker ml(&m_path_mutex);

    checkPathNodeReached();

    if (!m_path_tiles.isEmpty())
    {
        QPointF next_pos;
        if (m_path_tiles.length() == 1)
        {
            next_pos = m_platform.getPosInTile(m_path_tiles[0], this->boundingRect());
        }
        else
        {
            next_pos = m_platform.getPosInTile(m_path_tiles[1], this->boundingRect());
        }

        QPointF speed = next_pos - sceneBoundingRect().topLeft();

        qreal accel_x = speed.x() - m_dynamics->getSpeedX() * m_dynamics->getFriction();

        m_dynamics->setAccelX(fmin(fmax(accel_x, m_dynamics->getMinAccel()), m_dynamics->getMaxAccel()));

        if (speed.y() < 0)
        {
            jump();
        }
        else if (m_path_tiles.length() == 1 || m_path_tiles.length() > 1 && m_path_tiles[1].y() >= m_path_tiles[0].y())
        {
            stopJump();
        }
    }
    else
    {
        m_dynamics->setAccelX(0);
        m_dynamics->setAccelY(0);
    }
}
