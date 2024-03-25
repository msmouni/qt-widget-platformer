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

    m_dynamics = new EntityDynamics(this, 0, 0,  70, 0.5, true);

    m_jump_timer.setSingleShot(true);
    m_jump_timeout_ms=((qreal)(m_platform.getTileSize().height() *M_MAX_TILES_JUMP))/m_dynamics->getMaxAbsSpeedY() * 50; // NOTE: m_update_timeout_ms = 50;
//    qDebug()<<"m_jump_timeout_ms"<<m_jump_timeout_ms;
    connect(&m_jump_timer, &QTimer::timeout, this, &Character::jumpTimeout);
    m_remaining_jump_time = 0;

    m_attack_timer.setSingleShot(true);
    connect(&m_attack_timer, &QTimer::timeout, this, &Character::attackTimeout);
    m_remaining_attack_time = 0;
    m_attacking = false;

    m_hit_timer.setSingleShot(true);
    connect(&m_hit_timer, &QTimer::timeout, this, &Character::hitTimeout);
    m_remaining_hit_time=0;
    m_hit=false;
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

    pen.setWidth(9);
    painter->setPen(pen);
    painter->drawPoint(this->mapFromScene(sceneBoundingRect().topLeft()));
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

bool Character::isAttacking() const
{
    return m_state == CharacterState::Attack;
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

void Character::attackTimeout()
{
    qDebug() << "Attack Timeout";
    m_attacking = false;
}

void Character::hitTimeout()
{
    m_hit = false;
}

void Character::pause()
{
    m_animation->pause();
    m_remaining_jump_time = m_jump_timer.isActive() ? m_jump_timer.remainingTime() : 0;
    m_jump_timer.stop();
    m_remaining_attack_time = m_attack_timer.isActive() ? m_attack_timer.remainingTime() : 0;
    m_attack_timer.stop();

    for (QHash<int, Weapon *>::const_iterator weapons_it = m_weapons.constBegin(); weapons_it != m_weapons.constEnd(); ++weapons_it)
    {
        weapons_it.value()->pause();
    }
}

void Character::resume()
{
    m_animation->resume();
    m_jump_timer.start(m_remaining_jump_time);
    m_attack_timer.start(m_remaining_attack_time);

    for (QHash<int, Weapon *>::const_iterator weapons_it = m_weapons.constBegin(); weapons_it != m_weapons.constEnd(); ++weapons_it)
    {
        weapons_it.value()->resume();
    }
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
    if (!m_hit){
        qreal speed_x = m_dynamics->getSpeedX();
        qreal speed_y = m_dynamics->getSpeedY();

        if (m_attacking)
        {
            // && m_attack_timer not expired
            if (m_state != CharacterState::Attack)
            {
                qDebug() << "Attack timer start";
                m_attack_timer.start(m_animation->getIdDuration(static_cast<uint8_t>(CharacterState::Attack)));
                m_state = CharacterState::Attack;
            }
            else if (!m_attack_timer.isActive())
            {
                // continue attacking
                //            qDebug() << "Check";
                m_attack_timer.start(m_animation->getIdDuration(static_cast<uint8_t>(CharacterState::Attack)));
            }
        }
        else if (m_dynamics->isBottomCollision() && m_state == CharacterState::Fall)
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
                    hit(weapon->sceneBoundingRect().center(), weapon->getPowerX(), weapon->getPowerY());
                }
            }
            else if (item->data(0) == "Enemy" && this->data(0) != "Enemy")
            {
                Character *enemy = static_cast<Character *>(item);

                if (enemy->isAttacking())
                {
                    hit(enemy->sceneBoundingRect().center(), 100, 100); // NOTE: SPEED LIMITS
                }
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

bool Character::isHit()
{
    return m_state == CharacterState::Hit;
}

void Character::moveRight()
{
//    if (!isHit()){
        m_dynamics->setAccelX(m_dynamics->getMaxAbsAccelX());
//    }
}

void Character::moveLeft()
{
//    if (!isHit()){
        m_dynamics->setAccelX(-m_dynamics->getMaxAbsAccelX());
//    }
}

void Character::jump()
{
//    if (!isHit() && isOnGround() && !isAttacking())
    if (isOnGround() && !isAttacking())
    {
        m_jump_timer.start(m_jump_timeout_ms);//M_JUMP_TIMEOUT_MS);
        // Sp: cst & A: cst => TODO: Use A to decrease Sp (ex: Sp_{n} = 0.95 *Sp_{n-1})
        // Sp= Sp * friction + A + G
        m_dynamics->setAccelY(-m_dynamics->getMaxAbsSpeedY() * (1 - m_dynamics->getFriction()) - m_dynamics->getGravity());
        m_dynamics->setSpeedY(-m_dynamics->getMaxAbsSpeedY());
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

void Character::hit(QPointF hit_pos, qreal power_x, qreal power_y)
{
    m_state = CharacterState::Hit;

    QPointF diff_pos = this->sceneBoundingRect().center() - hit_pos;
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

    m_dynamics->setSpeedX(m_dynamics->getSpeedX() + power_x * dir_x);

    m_dynamics->setSpeedY(m_dynamics->getSpeedY() + power_y * dir_y);

    m_hit=true;
    m_hit_timer.start(M_HIT_TIMEOUT_MS);
}
