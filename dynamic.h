#ifndef ENTITYDYNAMICS_H
#define ENTITYDYNAMICS_H

#include "collision.h"

enum class EntityDirection
{
    NoDirection,
    MovingLeft,
    MovingRight,
};

class EntityDynamics
{
public:
    EntityDynamics(QGraphicsItem *parent = nullptr, qreal init_speed_x = 0, qreal init_speed_y = 0, qreal friction = 0.5, bool change_mvmt_dir = false, qreal speed_max_x = M_DEFAULT_SPEED_MAX, qreal speed_max_y = M_DEFAULT_SPEED_MAX, qreal weight = 50);

    void updateKinematics();
    void updateDynamics();

    void setCollisionMargin(QMarginsF margin);

    CollisionRect *getCollisionRect();

    qreal getSpeedX() const;
    qreal getSpeedY() const;
    void setSpeedX(qreal speed_x);
    void setSpeedY(qreal speed_y);

    qreal getAccelX() const;
    qreal getAccelY() const;
    void setAccelX(qreal accel_x);
    void setAccelY(qreal accel_y);

    qreal getFriction() const;
    qreal getGravity() const;
    qreal getMaxAbsSpeedX() const;
    qreal getMaxAbsSpeedY() const;
    void setMaxAbsSpeedX(qreal speed_max_x);
    void setMaxAbsSpeedY(qreal speed_max_y);
    //    qreal getMinSpeed() const;
    qreal getMaxAbsAccelX() const;
    qreal getMaxAbsAccelY() const;
    //    qreal getMinAccel() const;

    void setWeight(qreal weight);

    const EntityDirection &getDirection() const;

    QPointF getEntityPos() const;

    bool isBottomCollision();

private:
    //    const qreal M_SPEED_MAX = 30;
    //    const qreal M_ACCEL_MAX = 15;
    constexpr static const qreal M_DEFAULT_SPEED_MAX = 30;
    const qreal M_GRAVITY = 13;

    QGraphicsItem *m_parent;

    QPointF m_entity_pos;

    qreal m_speed_max_x;
    qreal m_speed_max_y;
    qreal m_accel_max_x;
    qreal m_accel_max_y;
    qreal m_speed_x;
    qreal m_speed_y;
    qreal m_acc_x;
    qreal m_acc_y;

    qreal m_weight;

    EntityDirection m_direction;

    qreal m_friction;

    CollisionRect *m_collision_rect;

    void updateDirection();
};

#endif // ENTITYDYNAMICS_H
