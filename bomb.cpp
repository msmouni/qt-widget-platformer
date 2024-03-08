#include "bomb.h"
#include "character.h"

Bomb::Bomb(int id, const QPointF &pos, qreal speed_x, qreal speed_y, qreal power_x, qreal power_y, const QString &res_path) : Weapon(id, pos, power_x, power_y, res_path)
{
    this->setData(0, "Bomb");

    m_explosion_timer = new QTimer(this);
    m_explosion_timer->setSingleShot(true);
    connect(m_explosion_timer, &QTimer::timeout, this, &Bomb::explosion);

    m_drop_timer = new QTimer(this);
    m_drop_timer->setSingleShot(true);
    connect(m_drop_timer, &QTimer::timeout, this, &Bomb::end);

    m_speed_x = speed_x;
    m_speed_y = speed_y;
    m_acc_x = 0;
    m_acc_y = 0;

    m_friction = 0.7;
    m_gravity = 13;

    m_collision_rect = new CollisionRect(sceneBoundingRect(), m_speed_x, m_speed_y, this);
}

void Bomb::start()
{
    m_state = WeaponState::Starting;

    m_animation->setId(static_cast<uint8_t>(m_state));

    m_explosion_timer->start(M_EXPLOSION_TIEMOUT_MS);
}

void Bomb::updateShapes()
{
    // with this model: S_n = S_{n-1} * f + Acc => Sn = Acc *(1 - f^n)/(1-f) => #with f < 1 and n >> 0: Sn = Acc/(1-f)
    m_speed_x *= m_friction; // Friction
    m_speed_x += m_acc_x;

    m_speed_y *= m_friction; // Friction
    m_speed_y += m_acc_y + m_gravity;

    m_bounding_rect = this->shape().boundingRect();

    m_collision_rect->update(sceneBoundingRect(), m_speed_x, m_speed_y);
}

void Bomb::updateWeapon()
{
    if (!isActive())
    {
        QVector<const CollisionRect *> dyn_collision_rects;
        QVector<QRectF> static_collision_rects;

        for (QGraphicsItem *item : m_collision_rect->collidingItems())
        {
            if (item->data(0) == "Enemy" || item->data(0) == "Player")
            {
                Character *chara = static_cast<Character *>(item);
                dyn_collision_rects.append(chara->getCollisionRect());
            }
            else if (item->data(0) == "Bomb")
            {
                Bomb *bomb = static_cast<Bomb *>(item);
                if (!bomb->isActive())
                {
                    dyn_collision_rects.append(bomb->getCollisionRect());
                }
            }
            else if (item->data(0) == "Tile")
            {
                Tile *tile = static_cast<Tile *>(item);

                QRectF tile_bnd_rect = tile->sceneBoundingRect();

                if (tile->isSolid() | (!tile->isEmpty() & ((tile->checkUp() && m_speed_y < 0 && sceneBoundingRect().bottom() >= tile_bnd_rect.bottom()) | (tile->checkDown() && m_speed_y > 0 && sceneBoundingRect().top() <= tile_bnd_rect.top()))))
                {
                    static_collision_rects.append(tile_bnd_rect);
                }
            }
        }

        m_collision_rect->handleCollision(dyn_collision_rects);
        m_collision_rect->handleCollision(static_collision_rects);

        m_speed_x = m_collision_rect->getSpeedX();
        m_speed_y = m_collision_rect->getSpeedY();
        QRectF res = m_collision_rect->getEntityRect().translated(m_speed_x, m_speed_y);

        this->setPos(res.topLeft() - boundingRect().topLeft());
    }
}

const CollisionRect *Bomb::getCollisionRect() const
{
    return m_collision_rect;
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
