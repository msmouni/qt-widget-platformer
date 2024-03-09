#include "bomb.h"

Bomb::Bomb(int id, const QPointF &pos, qreal speed_x, qreal speed_y, qreal power_x, qreal power_y, const QString &res_path) : Weapon(id, pos, power_x, power_y, res_path)
{
    this->setData(1, "Bomb");

    m_explosion_timer = new QTimer(this);
    m_explosion_timer->setSingleShot(true);
    connect(m_explosion_timer, &QTimer::timeout, this, &Bomb::explosion);

    m_drop_timer = new QTimer(this);
    m_drop_timer->setSingleShot(true);
    connect(m_drop_timer, &QTimer::timeout, this, &Bomb::end);

    m_dynamics = new EntityDynamics(this, speed_x, speed_y, 0.7);
}

void Bomb::start()
{
    m_state = WeaponState::Starting;

    m_animation->setId(static_cast<uint8_t>(m_state));

    m_explosion_timer->start(M_EXPLOSION_TIEMOUT_MS);
}

void Bomb::updateKinematics()
{
    m_bounding_rect = this->shape().boundingRect();

    m_dynamics->updateKinematics();
}

void Bomb::updateWeapon()
{
    if (!isActive())
    {
        m_dynamics->updateDynamics();
        this->setPos(m_dynamics->getEntityPos());
    }
}

const CollisionRect *Bomb::getCollisionRect() const
{
    return m_dynamics->getCollisionRect();
}

void Bomb::explosion()
{
    m_state = WeaponState::Active;

    m_animation->setId(static_cast<uint8_t>(m_state));

    m_explosion_timer->stop();
    m_explosion_timer->deleteLater();

    m_drop_timer->start(M_DROP_TIEMOUT_MS);
}

void Bomb::end()
{
    m_drop_timer->stop();
    m_drop_timer->deleteLater();

    m_animation->stop();

    emit terminate(this);
}
