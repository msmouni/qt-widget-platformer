#include "dynamic.h"

EntityDynamics::EntityDynamics(QGraphicsItem *parent, qreal init_speed_x, qreal init_speed_y, qreal friction, bool change_mvmt_dir, qreal speed_max_x, qreal speed_max_y)
{
    m_speed_x = init_speed_x;
    m_speed_y = init_speed_y;
    //    m_speed_max_x = speed_max_x;
    //    m_speed_max_y = speed_max_y;

    m_friction = friction;

    //    // IF the acceleration needs to be smaller, take it as arguments
    //    m_accel_max_x = m_speed_max_x + m_speed_max_x*m_friction; // Speed_X_{k} = Speed_X_{k -1} * friction + Accel_X_{k} / Speed_X_{k} = S_MAX & Speed_X_{k - 1} = -S_MAX
    //    m_accel_max_y = m_speed_max_y + m_speed_max_x*m_friction + M_GRAVITY; // Speed_Y_{k} = Speed_Y_{k -1} * friction + Accel_Y_{k} + GRAVITY / Speed_Y_{k} = -S_MAX & Speed_Y_{k - 1} = S_MAX

    //    qDebug()<<m_speed_max_x<<m_speed_max_y<<m_accel_max_x<<m_accel_max_y;
    m_acc_x = 0;
    m_acc_y = 0;

    setMaxAbsSpeedX(speed_max_x);
    setMaxAbsSpeedY(speed_max_y);

    m_parent = parent;

    m_collision_rect = new CollisionRect(parent, m_speed_x, m_speed_y);

    m_entity_pos = m_collision_rect->getEntityPos() - m_parent->boundingRect().topLeft();

    if (change_mvmt_dir)
    {
        m_direction = EntityDirection::MovingRight;
    }
    else
    {
        m_direction = EntityDirection::NoDirection;
    }
}

void EntityDynamics::updateKinematics()
{
    // with this model: S_n = S_{n-1} * f + Acc => Sn = Acc *(1 - f^n)/(1-f) => #with f < 1 and n >> 0: Sn = Acc/(1-f)
    m_speed_x *= m_friction; // Friction
    m_speed_x += m_acc_x;
    setSpeedX(m_speed_x);

    m_speed_y *= m_friction; // Friction
    m_speed_y += m_acc_y + M_GRAVITY;
    setSpeedY(m_speed_y);

    updateDirection();

    m_collision_rect->update();
}

void EntityDynamics::updateDynamics()
{
    m_collision_rect->handleCollision();

    QPointF scene_adjustmnt = m_parent->boundingRect().topLeft();

    m_entity_pos = m_collision_rect->getEntityPos() - scene_adjustmnt;
}

void EntityDynamics::setCollisionMargin(QMarginsF margin)
{
    m_collision_rect->setMargin(margin);
}

const CollisionRect *EntityDynamics::getCollisionRect() const
{
    return m_collision_rect;
}

qreal EntityDynamics::getSpeedX() const
{
    return m_speed_x;
}

qreal EntityDynamics::getSpeedY() const
{
    return m_speed_y;
}

void EntityDynamics::setSpeedX(qreal speed_x)
{
    m_speed_x = fmin(fmax(speed_x, -m_speed_max_x), m_speed_max_x);
}

void EntityDynamics::setSpeedY(qreal speed_y)
{
    m_speed_y = fmin(fmax(speed_y, -m_speed_max_y), m_speed_max_y);
}

qreal EntityDynamics::getAccelX() const
{
    return m_acc_x;
}

qreal EntityDynamics::getAccelY() const
{
    return m_acc_y;
}

void EntityDynamics::setAccelX(qreal accel_x)
{
    // NOTE: ACCELERATION IS CONSTRAINED BY SPEED IN THE MODEL WE USE
    // NOTE: Use min/max after seperating mvmt_accel + jump_accel (enemy/player): max_accel > max_speed*0.5 +gravity = 28
    m_acc_x = accel_x; // fmin(fmax(accel_x, getMinAccel()), getMaxAccel());
}

void EntityDynamics::setAccelY(qreal accel_y)
{
    m_acc_y = accel_y; // fmin(fmax(accel_y, getMinAccel()), getMaxAccel());
}

qreal EntityDynamics::getFriction() const
{
    return m_friction;
}

qreal EntityDynamics::getGravity() const
{
    return M_GRAVITY;
}

qreal EntityDynamics::getMaxAbsSpeedX() const
{
    return m_speed_max_x;
}

qreal EntityDynamics::getMaxAbsSpeedY() const
{
    return m_speed_max_y;
}

qreal EntityDynamics::getMaxAbsAccelX() const
{
    return m_accel_max_x;
}

qreal EntityDynamics::getMaxAbsAccelY() const
{
    return m_accel_max_y;
}

void EntityDynamics::setMaxAbsSpeedX(qreal speed_max_x)
{
    m_speed_max_x = speed_max_x;

    // IF the acceleration needs to be smaller, take it as arguments
    m_accel_max_x = m_speed_max_x + m_speed_max_x * m_friction; // Speed_X_{k} = Speed_X_{k -1} * friction + Accel_X_{k} / Speed_X_{k} = S_MAX & Speed_X_{k - 1} = -S_MAX
}

void EntityDynamics::setMaxAbsSpeedY(qreal speed_max_y)
{
    m_speed_max_y = speed_max_y;

    // IF the acceleration needs to be smaller, take it as arguments
    m_accel_max_y = m_speed_max_y + m_speed_max_y * m_friction + M_GRAVITY; // Speed_Y_{k} = Speed_Y_{k -1} * friction + Accel_Y_{k} + GRAVITY / Speed_Y_{k} = -S_MAX & Speed_Y_{k - 1} = S_MAX
}

const EntityDirection &EntityDynamics::getDirection() const
{
    return m_direction;
}

QPointF EntityDynamics::getEntityPos() const
{
    return m_entity_pos;
}

bool EntityDynamics::isBottomCollision()
{
    return m_collision_rect->isBottomCollision();
}

bool EntityDynamics::isFrontCollision()
{
    switch (m_direction)
    {
    case EntityDirection::MovingLeft:
        return m_collision_rect->isLeftCollision();
    case EntityDirection::MovingRight:
        return m_collision_rect->isRightCollision();
    case EntityDirection::NoDirection:
        return m_collision_rect->isLeftCollision() || m_collision_rect->isRightCollision();
    default:
        break;
    }
}

void EntityDynamics::hit(QPointF hit_pos, qreal power_x, qreal power_y)
{
    QPointF diff_pos = m_parent->sceneBoundingRect().center() - hit_pos;
    qreal dir_x = 1;
    qreal dir_y = -1;

    if (diff_pos.x() < 0)
    {
        dir_x = -1;
    }

    if (diff_pos.y() > 0)
    {
        dir_y = 1;
    }

    setSpeedX(getSpeedX() + power_x * dir_x);

    setSpeedY(getSpeedY() + power_y * dir_y);
}

void EntityDynamics::updateDirection()
{
    if (m_direction != EntityDirection::NoDirection)
    {
        if (m_speed_x > 1)
        {
            m_parent->setTransform(QTransform().scale(1, 1));
            m_direction = EntityDirection::MovingRight;
        }
        else if (m_speed_x < -1 || m_direction == EntityDirection::MovingLeft)
        {
            // Width change
            QRectF parent_bnd_rect = m_parent->boundingRect();
            m_parent->setTransform(QTransform().scale(-1, 1).translate(-2 * parent_bnd_rect.topLeft().x() - parent_bnd_rect.width(), 0));
            m_direction = EntityDirection::MovingLeft;
        }
    }
}
