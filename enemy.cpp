#include "enemy.h"

Enemy::Enemy(const QPointF &pos, const QString &res_path, const Platform &platform, const QRectF &player_rect) : Character(pos, res_path, platform), m_path_finder(platform), m_player_rect(player_rect)
{
    setData(0, "Enemy");
    m_type = CharacterType::Enemy;

    m_animation->addAnimationState((uint8_t)CharacterState::Attack, res_path + "/Attack");

    //    m_animation->stop();

    m_dynamics->setMaxAbsSpeedX(20);
    m_dynamics->setMaxAbsSpeedY(20);
    m_jump_timeout_ms = ((qreal)(m_platform.getTileSize().height() * M_MAX_TILES_JUMP)) / m_dynamics->getMaxAbsSpeedY() * 50; // NOTE: m_update_timeout_ms = 50;

    connect(&m_path_finder, SIGNAL(pathFindingRes(QVector<QPoint>)), this, SLOT(setPathFindingResult(QVector<QPoint>)));
}

void Enemy::updateKinematics()
{
    if (!isHit() && !isAttacking())
    {
        followPath();
    }

    // NOTE: When not colling followPath when Attacking or hit -> acceleration is not updated, so the enemy keeps the last value
    else if (isAttacking())
    {
        m_path_tiles.clear();
        m_dynamics->setAccelX(0);
        m_dynamics->setAccelY(0);
        //        m_dynamics->setSpeedX(0);
        //        m_dynamics->setSpeedY(0);
    }

    Character::updateKinematics();
}

void Enemy::gameUpdate()
{
    findPath();

    if (isAttacking())
    {
        //        qDebug()<<"Collision margin";
        m_dynamics->setCollisionMargin(QMarginsF(-15, 0, -15, 0));
    }
    else
    {
        m_dynamics->setCollisionMargin(QMarginsF(0, 0, 0, 0));
    }

    if (sceneBoundingRect().marginsAdded(QMarginsF(3, 0, 3, 0)).intersects(m_player_rect))
    {
        //        qDebug()<<"Attack";
        m_attacking = true;
        //        m_attack_timer.start(m_animation->getIdDuration(static_cast<uint8_t>(CharacterState::Attack)));
        //        m_state =CharacterState::Attack;
        //        //        updateAnimation();
        //        m_animation->setId(static_cast<uint8_t>(m_state));
    }

    updateCharacter();
}

void Enemy::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (!m_path_tiles.isEmpty())
    {
        QPainterPath path;

        path.moveTo(mapFromScene(m_platform.getPosInTile(m_path_tiles[0], this->boundingRect())));

        for (int i = 1; i < m_path_tiles.size(); ++i)
        {
            path.lineTo(mapFromScene(m_platform.getPosInTile(m_path_tiles[i], this->boundingRect())));
        }

        QPen pen(Qt::red);
        pen.setWidth(2); // 20);
        painter->setPen(pen);

        painter->drawPath(path);
    }

    Character::paint(painter, option, widget);
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

        m_dynamics->setAccelX(accel_x); // fmin(fmax(accel_x, m_dynamics->getMinAccel()), m_dynamics->getMaxAccel()));

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

// void Enemy::updateState()
//{
//     Character::updateState();

////    bool attack=abs(m_player_rect.bottom() - sceneBoundingRect().bottom())<1 && m_player_rect.right() - sceneBoundingRect().right();

//    // TODO: change it to pursued
////    if (abs(m_player_rect.left() - sceneBoundingRect().left())<boundingRect().width()*1.5){
//    if (sceneBoundingRect().marginsAdded(QMarginsF(3,0,3,0)).intersects(m_player_rect)){
//        m_state =CharacterState::Attack;
////        updateAnimation();
//        m_animation->setId(static_cast<uint8_t>(m_state));
//    }
//}
