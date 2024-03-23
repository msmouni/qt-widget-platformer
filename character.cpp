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

    m_state = CharacterState::Init;

    m_animation->setId(static_cast<uint8_t>(m_state));

    setPixmap(m_animation->getPixmap());

    connect(m_animation, SIGNAL(updatePixmap()), this, SLOT(updateView()));

    m_dynamics = new EntityDynamics(this, 0, 0, 0.5, true);

    m_jump_timer.setSingleShot(true);
    connect(&m_jump_timer, &QTimer::timeout, this, &Character::jumpTimeout);
}

QRectF Character::boundingRect() const
{
    return m_bounding_rect;
}

void Character::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(m_bounding_rect, pixmap(), this->shape().boundingRect());

    // Set the pen and brush for the rectangle
    QPen pen(Qt::green);
    pen.setWidth(2);
    painter->setPen(pen);
    //    painter->drawRect(this->boundingRect());

    painter->drawPath(this->shape());

    /*QPen pen2(Qt::yellow);
    pen.setWidth(2);
    painter->setPen(pen2);
    painter->drawRect(coverage_rect.boundingRect());*/
    ///////////////////////////////////////////::
    //    painter->drawRect(this->sceneBoundingRect());

    pen.setWidth(9);
    painter->setPen(pen);
    //    painter->drawPoint(this->mapFromScene(m_dynamics->getEntityBoxPos()));//pos()));//scenePos()));//m_dynamics->getEntityPos()));//+boundingRect().topLeft()));//boundingRect().topLeft());
    //    painter->drawPoint(sceneBoundingRect().topLeft());
    painter->drawPoint(this->mapFromScene(QPointF(fmin(sceneBoundingRect().left(), sceneBoundingRect().right()), sceneBoundingRect().top())));
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

void Character::jumpTimeout()
{
    m_dynamics->setAccelY(0);
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
    return m_state != CharacterState::Init && m_state != CharacterState::Jump && m_state != CharacterState::Fall;
}

void Character::moveRight()
{
    m_dynamics->setAccelX(m_dynamics->getMaxAccel());
}

void Character::moveLeft()
{
    m_dynamics->setAccelX(m_dynamics->getMinAccel());
}

void Character::jump()
{
    if (isOnGround())
    {
        m_jump_timer.start(M_JUMP_TIMEOUT_MS);
        // Sp: cst & A: cst => TODO: Use A to decrease Sp (ex: Sp_{n} = 0.95 *Sp_{n-1})
        // Sp= Sp * friction + A + G
        m_dynamics->setAccelY(m_dynamics->getMinSpeed() * (1 - m_dynamics->getFriction()) - m_dynamics->getGravity());
        m_dynamics->setSpeedY(m_dynamics->getMinSpeed());
    }
}

void Character::stopRight()
{
    m_dynamics->setAccelX(fmin(m_dynamics->getAccelX(), 0));
}

void Character::stopLeft()
{
    m_dynamics->setAccelX(fmax(m_dynamics->getAccelX(), 0));
}

void Character::stopJump()
{
    m_dynamics->setAccelY(0);
}
