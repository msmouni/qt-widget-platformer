#ifndef ENTITYDYNAMICS_H
#define ENTITYDYNAMICS_H

#include "collision.h"

enum class EntityDirection
{
    NoDirection,
    MovingLeft,
    MovingRight,
};
/*
NOTES:

    Kinematics:
        Definition: Kinematics is the study of motion of objects without considering the forces causing the motion or the properties of the objects (like mass or friction).
        Focus: It describes motion in terms of displacement, velocity, acceleration, and time, without delving into the reasons why the motion occurs.
        Equations: The primary equations used in kinematics are the equations of motion, such as the kinematic equations for uniformly accelerated motion.
        Questions Addressed: Kinematics answers questions like "How far did the object move?", "What was its speed?", or "How fast was it accelerating?".

    Dynamics:
        Definition: Dynamics is the study of motion of objects in relation to the forces acting upon them and the resulting motion changes (acceleration).
        Focus: It considers the forces, masses, and accelerations of objects to predict or explain motion.
        Equations: Newton's laws of motion are the fundamental equations used in dynamics. They relate the net force acting on an object to its acceleration.
        Questions Addressed: Dynamics answers questions like "What forces are acting on the object?", "What is the resulting acceleration?", or "What will happen to the motion if a certain force is applied?".

*/
class EntityDynamics
{
public:
    EntityDynamics(QGraphicsItem *parent = nullptr, qreal init_speed_x = 0, qreal init_speed_y = 0,  qreal weight = 50, qreal friction = 0.5, bool change_mvmt_dir = false, qreal speed_max_x = M_DEFAULT_SPEED_MAX, qreal speed_max_y = M_DEFAULT_SPEED_MAX);

    void updateKinematics();
    void updateDynamics();

    void setCollisionMargin(QMarginsF margin);

    const CollisionRect *getCollisionRect() const;

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

    EntityDirection m_direction;

    qreal m_friction;

    CollisionRect *m_collision_rect;

    void updateDirection();
};

#endif // ENTITYDYNAMICS_H
