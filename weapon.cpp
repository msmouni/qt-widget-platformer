#include "weapon.h"
#include <QPainter>

Weapon::Weapon(int id, const QPointF &pos, qreal power_x, qreal power_y, const QString &res_path) : m_id(id), m_power_x(power_x), m_power_y(power_y)
{

    setData(0, "Weapon");

    m_state = WeaponState::Idle;

    QHash<uint8_t, QString> animations_ids;
    animations_ids.insert((uint8_t)WeaponState::Idle, res_path + "/Idle");
    animations_ids.insert((uint8_t)WeaponState::Starting, res_path + "/Starting");
    animations_ids.insert((uint8_t)WeaponState::Active, res_path + "/Active");

    m_animation = new SpriteAnimation(animations_ids, 50);

    m_animation->setId(static_cast<uint8_t>(m_state));

    this->setPos(pos);

    setPixmap(m_animation->getPixmap());

    connect(m_animation, SIGNAL(updatePixmap()), this, SLOT(updateView()));
}

QRectF Weapon::boundingRect() const
{
    return m_bounding_rect;
}

void Weapon::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(m_bounding_rect, pixmap(), this->shape().boundingRect());

    // Set the pen and brush
    QPen pen(Qt::red);
    pen.setWidth(2);
    painter->setPen(pen);

    painter->drawRect(this->boundingRect());

    painter->drawPath(this->shape());
}

void Weapon::start()
{
    m_state = WeaponState::Starting;

    m_animation->setId(static_cast<uint8_t>(m_state));
}

void Weapon::activate()
{
    m_state = WeaponState::Active;

    m_animation->setId(static_cast<uint8_t>(m_state));
}

void Weapon::desactivate()
{
    m_state = WeaponState::Idle;

    m_animation->setId(static_cast<uint8_t>(m_state));
}

bool Weapon::isActive() const
{
    return m_state == WeaponState::Active;
}

qreal Weapon::getPowerX() const
{
    return m_power_x;
}

qreal Weapon::getPowerY() const
{
    return m_power_y;
}

int Weapon::getId()
{
    return m_id;
}

void Weapon::updateView()
{
    setPixmap(m_animation->getPixmap());
}
