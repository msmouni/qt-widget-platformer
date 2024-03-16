#include "weapon.h"



Weapon::Weapon(int id, const QRectF &scene_rect, const QRectF &active_rect, qreal power_x, qreal power_y, const Platform &platform, const QString &res_path):
    m_id(id), m_scene_rect(scene_rect), m_active_rect(active_rect), m_power_x(power_x), m_power_y(power_y), m_platform(platform)
{

    setData(0, "Weapon");

    // pos = center
    m_scene_rect = QRectF(-scene_rect.width() / 2, -scene_rect.height() / 2, scene_rect.width(), scene_rect.height());
    m_active_rect = QRectF(-active_rect.width() / 2, -active_rect.height() / 2, active_rect.width(), active_rect.height());
    this->setPos(scene_rect.center());

    m_state=WeaponState::Idle;

    if (res_path !=nullptr){
        QHash<uint8_t, QString> animations_ids;
        animations_ids.insert((uint8_t) WeaponState::Idle, res_path + "/Idle");
        animations_ids.insert((uint8_t)WeaponState::Starting, res_path + "/Starting");
        animations_ids.insert((uint8_t)WeaponState::Active, res_path + "/Active");

        m_animation = new SpriteAnimation(animations_ids, 50);

        m_animation->setId(static_cast<uint8_t>(m_state));

        setPixmap(m_animation->getPixmap());

        connect(m_animation, SIGNAL(updatePixmap()), this, SLOT(updateView()));

        m_visible=true;
    }else {
        m_visible=false;
    }
}

QRectF Weapon::boundingRect() const
{
    switch (m_state) {
    case WeaponState::Idle:
        return m_scene_rect;
    case WeaponState::Starting:
        return m_scene_rect;
    case WeaponState::Active:
        return m_active_rect;
    default:
        break;
    }
}

void Weapon::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (m_visible){
        painter->drawPixmap(m_scene_rect, pixmap(), m_animation->getRect());

        // Set the pen and brush for the rectangle
        QPen pen(Qt::red);
        pen.setWidth(2);
        painter->setPen(pen);
        painter->drawRect(this->boundingRect());
    }
}

QPainterPath Weapon::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    //    path.addEllipse(boundingRect().x(), boundingRect().y(), boundingRect().width(), boundingRect().height());
    return path;
}

void Weapon::start()
{
    m_state=WeaponState::Starting;

    m_animation->setId(static_cast<uint8_t>(m_state));
}

void Weapon::activate()
{
    m_state=WeaponState::Active;

    m_animation->setId(static_cast<uint8_t>(m_state));
}

void Weapon::desactivate()
{
    m_state=WeaponState::Idle;

    m_animation->setId(static_cast<uint8_t>(m_state));
}

bool Weapon::isActive() const
{
    return m_state ==WeaponState::Active;
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
