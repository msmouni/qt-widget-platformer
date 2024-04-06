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

    m_remaining_timer_time = 0;

    m_dynamics = new EntityDynamics(this, speed_x, speed_y, 0.7);
}

void Bomb::start()
{
    m_state = WeaponState::Starting;

    m_animation->setId(static_cast<uint8_t>(m_state));

    m_progress_bar = new ProgressBar(this, QRectF(20, 30, 50, 4), 1.0, Qt::green);
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
        qreal speed_x_prev = m_dynamics->getSpeedX();
        qreal speed_y_prev = m_dynamics->getSpeedY();

        m_dynamics->updateDynamics();
        qreal speed_x = m_dynamics->getSpeedX();
        qreal speed_y = m_dynamics->getSpeedY();

        if (abs(speed_y - speed_y_prev) > 1)
        {
            m_dynamics->setSpeedY((speed_y - speed_y_prev));
        }

        this->setPos(m_dynamics->getEntityPos());
    }

    if (m_state == WeaponState::Starting)
    {
        qreal progress = (qreal)m_explosion_timer->remainingTime() / (qreal)M_EXPLOSION_TIEMOUT_MS;
        m_progress_bar->setProgress(progress);

        m_progress_bar->setBarColor(QColor(fmin((-2 * progress + 2) * 255, 255), fmin((2 * progress) * 255, 255), 0));
    }
}

const CollisionRect *Bomb::getCollisionRect() const
{
    return m_dynamics->getCollisionRect();
}

void Bomb::pause()
{
    if (m_state == WeaponState::Starting)
    {
        m_remaining_timer_time = m_explosion_timer->isActive() ? m_explosion_timer->remainingTime() : 0;
        m_explosion_timer->stop();
    }
    else if (m_state == WeaponState::Active)
    {
        m_remaining_timer_time = m_drop_timer->isActive() ? m_drop_timer->remainingTime() : 0;
        m_drop_timer->stop();
    }

    Weapon::pause();
}

void Bomb::resume()
{
    if (m_state == WeaponState::Starting)
    {
        m_explosion_timer->start(m_remaining_timer_time);
    }
    else if (m_state == WeaponState::Active)
    {
        m_drop_timer->start(m_remaining_timer_time);
    }

    Weapon::resume();
}

void Bomb::explosion()
{
    m_state = WeaponState::Active;

    delete m_progress_bar;

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
