#include "dynamic.h"

EntityDynamics::EntityDynamics(QGraphicsItem *parent, qreal init_speed_x, qreal init_speed_y, qreal friction, bool change_mvmt_dir)
{
    m_speed_x = init_speed_x;
    m_speed_y = init_speed_y;
    m_acc_x = 0;
    m_acc_y = 0;

    m_friction = friction;

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

    m_speed_y *= m_friction; // Friction
    m_speed_y += m_acc_y + M_GRAVITY;

    updateDirection();

    m_collision_rect->update();
}

void EntityDynamics::updateDynamics()
{
    m_collision_rect->handleCollision();

    QPointF scene_adjustmnt = m_parent->boundingRect().topLeft();

    m_entity_pos = m_collision_rect->getEntityPos() - scene_adjustmnt;
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
    m_speed_x = speed_x;
}

void EntityDynamics::setSpeedY(qreal speed_y)
{
    m_speed_y = speed_y;
}

qreal EntityDynamics::getAccelX()
{
    return m_acc_x;
}

qreal EntityDynamics::getAccelY()
{
    return m_acc_y;
}

void EntityDynamics::setAccelX(qreal accel_x)
{
    m_acc_x = accel_x;
}

void EntityDynamics::setAccelY(qreal accel_y)
{
    m_acc_y = accel_y;
}

qreal EntityDynamics::getFriction()
{
    return m_friction;
}

qreal EntityDynamics::getGravity()
{
    return M_GRAVITY;
}

const EntityDirection &EntityDynamics::getDirection() const
{
    return m_direction;
}

QPointF EntityDynamics::getEntityPos()
{
    return m_entity_pos;
}

bool EntityDynamics::isBottomCollision()
{
    return m_collision_rect->isBottomCollision();
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
