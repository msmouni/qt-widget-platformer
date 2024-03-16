#include "bomb.h"

Bomb::Bomb(int id, const QRectF &scene_rect, const QRectF &active_rect, qreal speed_x,qreal speed_y,qreal power_x, qreal power_y, const Platform &platform, const QString &res_path): Weapon(id, scene_rect, active_rect, power_x, power_y, platform, res_path)
{
    m_explosion_timer = new QTimer;
    connect(m_explosion_timer, &QTimer::timeout, this, &Bomb::explosion);

    m_drop_timer = new QTimer;
    connect(m_drop_timer, &QTimer::timeout, this, &Bomb::end);

    m_speed_x = speed_x;
    m_speed_y = speed_y;
    m_acc_x = 0;
    m_acc_y = 0;

    m_friction = 0.5;
    m_gravity = 13;
}

void Bomb::start()
{
    m_state=WeaponState::Starting;

    m_animation->setId(static_cast<uint8_t>(m_state));

    m_explosion_timer->start(M_EXPLOSION_TIEMOUT_MS);
}

void Bomb::updateWeapon()
{
    if (!isActive()){
        // with this model: S_n = S_{n-1} * f + Acc => Sn = Acc *(1 - f^n)/(1-f) => #with f < 1 and n >> 0: Sn = Acc/(1-f)
        m_speed_x *= m_friction; // Friction
        m_speed_x += m_acc_x;

        m_speed_y *= m_friction; // Friction
        m_speed_y += m_acc_y + m_gravity;

        QRectF res = m_platform.handleCollision(sceneBoundingRect(), m_speed_x, m_speed_y);

        this->setPos(res.center());
    }
}

void Bomb::explosion()
{
    m_state=WeaponState::Active;

    m_animation->setId(static_cast<uint8_t>(m_state));

    m_state = WeaponState::Active;

    m_drop_timer->start(M_DROP_TIEMOUT_MS);
}

void Bomb::end()
{
    emit terminate(this);
}
