#include "character.h"
#include <QGraphicsScene>

Character::Character(const QPointF &pos, const QString &res_path, const Platform &platform) : m_platform(platform)
{
    QHash<uint8_t, QString> animations_ids;
    animations_ids.insert((uint8_t)CharacterState::Idle, res_path + "/Idle");
    animations_ids.insert((uint8_t)CharacterState::Run, res_path + "/Run");
    animations_ids.insert((uint8_t)CharacterState::Jump, res_path + "/Jump");
    animations_ids.insert((uint8_t)CharacterState::Fall, res_path + "/Fall");
    animations_ids.insert((uint8_t)CharacterState::Ground, res_path + "/Ground");
    animations_ids.insert((uint8_t)CharacterState::Hit, res_path + "/Hit");
    animations_ids.insert((uint8_t)CharacterState::DeadHit, res_path + "/Dead Hit");

    m_animation = new SpriteAnimation(animations_ids, 50);

    m_bounding_rect = m_animation->getRect();
    this->setPos(pos);

    m_state = CharacterState::Idle;

    m_animation->setId(static_cast<uint8_t>(m_state));

    setPixmap(m_animation->getPixmap());

    connect(m_animation, SIGNAL(updatePixmap()), this, SLOT(updateView()));

    m_dynamics = new EntityDynamics(this, 0, 0, 0.5, true);
}

QRectF Character::boundingRect() const
{
    return m_bounding_rect;
}

void Character::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(m_bounding_rect, pixmap(), this->shape().boundingRect());
}

void Character::updateKinematics()
{
    m_bounding_rect = this->shape().boundingRect();

    m_dynamics->updateKinematics();

    for (QHash<int, Weapon *>::const_iterator weapons_it = m_weapons.constBegin(); weapons_it != m_weapons.constEnd(); ++weapons_it)
    {
        weapons_it.value()->updateKinematics();
    }
}

const CollisionRect *Character::getCollisionRect() const
{
    return m_dynamics->getCollisionRect();
}

void Character::updateView()
{
    setPixmap(m_animation->getPixmap());
}

void Character::dropWeapon(Weapon *weapon)
{
    m_weapons.remove(weapon->getId());

    weapon->deleteLater();
}

void Character::updateCharacter()
{
    for (QHash<int, Weapon *>::const_iterator weapons_it = m_weapons.constBegin(); weapons_it != m_weapons.constEnd(); ++weapons_it)
    {
        weapons_it.value()->updateWeapon();
    }

    m_dynamics->updateDynamics();
    this->setPos(m_dynamics->getEntityPos());

    updateState();
    updateAnimation();
}

void Character::updateState()
{
    qreal speed_x = m_dynamics->getSpeedX();
    qreal speed_y = m_dynamics->getSpeedY();

    if (m_dynamics->isBottomCollision() && m_state == CharacterState::Fall)
    {
        m_state = CharacterState::Ground;
    }
    else if (speed_y > 0.01)
    {
        m_state = CharacterState::Fall;
    }
    else if (speed_y < -0.01)
    {
        m_state = CharacterState::Jump;
    }
    else if (abs(speed_x) > 1)
    {
        m_state = CharacterState::Run;
    }
    else
    {
        m_state = CharacterState::Idle;
    }

    for (QGraphicsItem *item : this->collidingItems())
    {
        if (item->data(0) == "Weapon")
        {
            Weapon *weapon = static_cast<Weapon *>(item);
            if (weapon->isActive())
            {
                m_state = CharacterState::Hit;

                QPointF diff_pos = this->sceneBoundingRect().center() - weapon->sceneBoundingRect().center();
                qreal dir_x = 1;
                qreal dir_y = -1;

                if (diff_pos.x() < 0)
                {
                    dir_x = -1;
                }

                if (diff_pos.y() > 0)
                {
                    dir_y = 1;
                }

                m_dynamics->setSpeedX(speed_x + weapon->getPowerX() * dir_x);

                m_dynamics->setSpeedY(speed_y + weapon->getPowerY() * dir_y);
            }
        }
    }
}

void Character::updateAnimation()
{
    m_animation->setId(static_cast<uint8_t>(m_state));
}

bool Character::isOnGround()
{
    return m_state == CharacterState::Ground | m_state == CharacterState::Idle | m_state == CharacterState::Run;
}
