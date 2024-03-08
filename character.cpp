#include "character.h"
#include "bomb.h"
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
    m_direction = CharacterDirection::Right;

    m_animation->setId(static_cast<uint8_t>(m_state));

    setPixmap(m_animation->getPixmap());

    connect(m_animation, SIGNAL(updatePixmap()), this, SLOT(updateView()));

    m_speed_x = 0;
    m_speed_y = 0;
    m_acc_x = 0;
    m_acc_y = 0;

    m_acc_max = 15;
    m_friction = 0.5;
    m_gravity = 13;

    m_collision_rect = new CollisionRect(sceneBoundingRect(), m_speed_x, m_speed_y, this);
}

QRectF Character::boundingRect() const
{
    return m_bounding_rect;
}

void Character::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(m_bounding_rect, pixmap(), this->shape().boundingRect());
}

void Character::updateShapes()
{
    m_speed_x *= m_friction; // Friction
    m_speed_x += m_acc_x;

    m_speed_y *= m_friction; // Friction
    m_speed_y += m_acc_y + m_gravity;

    // Note: m_bounding_rect should match the shape bounding rect: shape is at pos=(x,y) reg this->pos()
    m_bounding_rect = this->shape().boundingRect();

    m_collision_rect->update(sceneBoundingRect(), m_speed_x, m_speed_y);

    for (QHash<int, Weapon *>::const_iterator weapons_it = m_weapons.constBegin(); weapons_it != m_weapons.constEnd(); ++weapons_it)
    {
        weapons_it.value()->updateShapes();
    }
}

const CollisionRect *Character::getCollisionRect() const
{
    return m_collision_rect;
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

    if (m_speed_x > 1)
    {
        this->setTransform(QTransform().scale(1, 1));
        m_direction = CharacterDirection::Right;
    }
    else if (m_speed_x < -1 || m_direction == CharacterDirection::Left)
    {
        this->setTransform(QTransform().scale(-1, 1).translate(-m_bounding_rect.width(), 0)); ////////// Width change ...
        m_direction = CharacterDirection::Left;
    }

    QVector<const CollisionRect *> dyn_collision_rects;
    QVector<QRectF> static_collision_rects;

    for (QGraphicsItem *item : m_collision_rect->collidingItems())
    {
        if (item->data(0) == "Enemy" && m_type != CharacterType::Enemy || item->data(0) == "Player" && m_type != CharacterType::Player)
        {
            Character *chara = static_cast<Character *>(item);
            dyn_collision_rects.append(chara->m_collision_rect);
        }
        else if (item->data(1) == "Bomb")
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

    /////////////////////////: State

    if (m_collision_rect->isBottomCollision() && m_state == CharacterState::Fall)
    {
        m_state = CharacterState::Ground;
    }
    else if (abs(m_speed_y) <= 0.01)
    {
        m_state = CharacterState::Idle;
    }
    else if (m_speed_y > 0.01)
    {
        m_state = CharacterState::Fall;
    }
    else if (m_speed_y < -0.01)
    {
        m_state = CharacterState::Jump;
    }
    else if (abs(m_speed_x) > 1)
    {
        m_state = CharacterState::Run;
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

                m_speed_x += weapon->getPowerX() * dir_x;

                m_speed_y += weapon->getPowerY() * dir_y;
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////////

    QPointF scene_adjustmnt = m_bounding_rect.topLeft();
    if (m_direction == CharacterDirection::Left)
    {
        scene_adjustmnt.setX(-scene_adjustmnt.x());
    }

    this->setPos(res.topLeft() - scene_adjustmnt);
    updateAnimation();
}

void Character::updateAnimation()
{
    m_animation->setId(static_cast<uint8_t>(m_state));
}

bool Character::isOnGround()
{
    return m_state == CharacterState::Ground | m_state == CharacterState::Idle | m_state == CharacterState::Run;
}
