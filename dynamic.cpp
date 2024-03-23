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
    m_speed_x = fmin(fmax(m_speed_x, getMinSpeed()), getMaxSpeed());

    m_speed_y *= m_friction; // Friction
    m_speed_y += m_acc_y + M_GRAVITY;
    m_speed_y = fmin(fmax(m_speed_y, getMinSpeed()), getMaxSpeed());

    //    updateDirection();

    m_collision_rect->update();
}

void EntityDynamics::updateDynamics()
{

    updateDirection();
    m_collision_rect->handleCollision();

    QPointF scene_adjustmnt = m_parent->boundingRect().topLeft();
    if (m_direction == EntityDirection::MovingLeft)
    {
        scene_adjustmnt.setX(-scene_adjustmnt.x());
    }

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
    m_speed_x = speed_x; // fmin(fmax(speed_x, getMinSpeed()), getMaxSpeed());
}

void EntityDynamics::setSpeedY(qreal speed_y)
{
    m_speed_y = speed_y; // fmin(fmax(speed_y, getMinSpeed()), getMaxSpeed());
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

qreal EntityDynamics::getMaxSpeed() const
{
    return M_SPEED_MAX;
}

qreal EntityDynamics::getMinSpeed() const
{
    return -M_SPEED_MAX;
}

qreal EntityDynamics::getMaxAccel() const
{
    return M_ACCEL_MAX;
}

qreal EntityDynamics::getMinAccel() const
{
    return -M_ACCEL_MAX;
}

const EntityDirection &EntityDynamics::getDirection() const
{
    return m_direction;
}

QPointF EntityDynamics::getEntityPos() const
{
    return m_entity_pos;
}

QPointF EntityDynamics::getEntityBoxPos() const
{
    QPointF scene_adjustment = m_parent->boundingRect().topLeft();
    if (m_direction == EntityDirection::MovingLeft)
    {
        scene_adjustment.setX(-m_parent->boundingRect().left());
    }

    return m_parent->pos() + scene_adjustment;
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
            m_parent->setTransform(QTransform().scale(-1, 1).translate(-m_parent->boundingRect().width(), 0)); ////////// Width change ...
            m_direction = EntityDirection::MovingLeft;
        }
    }
}
