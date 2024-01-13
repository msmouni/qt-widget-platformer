#include "character.h"

Character::Character(const QRectF &rect, const QString &res_path)
{
    // pos = center
    m_bounding_rect = QRectF(-rect.width() / 2, -rect.height() / 2, rect.width(), rect.height());
    this->setPos(rect.center());

    QHash<uint8_t, QString> animations_ids;
    animations_ids.insert((uint8_t)CharacterState::Idle, res_path + "/Idle");
    animations_ids.insert((uint8_t)CharacterState::Run, res_path + "/Run");
    animations_ids.insert((uint8_t)CharacterState::Jump, res_path + "/Jump");
    animations_ids.insert((uint8_t)CharacterState::Fall, res_path + "/Fall");
    animations_ids.insert((uint8_t)CharacterState::Ground, res_path + "/Ground");
    animations_ids.insert((uint8_t)CharacterState::Hit, res_path + "/Hit");
    animations_ids.insert((uint8_t)CharacterState::DeadHit, res_path + "/Dead Hit");

    m_animation = new SpriteAnimation(animations_ids, 50);

    setPixmap(m_animation->getPixmap());

    connect(m_animation, SIGNAL(updatePixmap()), this, SLOT(updateView()));

    m_state = CharacterState::Idle;
    m_direction = CharacterDirection::Right;

    m_speed_x = 0;
    m_speed_y = 0;
    m_acc_x = 0;
    m_acc_y = 0;

    m_acc_max = 15;
    m_friction = 0.5;
    m_gravity = 13;
}

QRectF Character::boundingRect() const
{
    return m_bounding_rect;
}

void Character::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    switch (m_direction)
    {
    case CharacterDirection::Left:
        painter->drawPixmap(m_bounding_rect, pixmap().transformed(QTransform().scale(-1, 1)), m_animation->getRect());
        break;
    case CharacterDirection::Right:
        painter->drawPixmap(m_bounding_rect, pixmap(), m_animation->getRect());
        break;
    default:
        break;
    }

    // Set the pen and brush for the rectangle
    QPen pen(Qt::red);
    pen.setWidth(2);
    painter->setPen(pen);
    painter->setFont(QFont("Arial", 15));
}

void Character::updateView()
{
    setPixmap(m_animation->getPixmap());
}

void Character::updateCharacter(const Platform *platform)
{
    // with this model: S_n = S_{n-1} * f + Acc => Sn = Acc *(1 - f^n)/(1-f) => #with f < 1 and n >> 0: Sn = Acc/(1-f)
    m_speed_x *= m_friction; // Friction
    m_speed_x += m_acc_x;

    m_speed_y *= m_friction; // Friction
    m_speed_y += m_acc_y + m_gravity;

    // TODO: Better handle for State and direction

    if (m_speed_x > 0.001)
    {
        m_direction = CharacterDirection::Right;
    }
    else if (m_speed_x < -0.001)
    {
        m_direction = CharacterDirection::Left;
    }

    if (m_speed_y < -0.001)
    {
        m_state = CharacterState::Jump;
    }

    qreal no_collision_speed_y = m_speed_y;

    QRectF res = platform->handleCollision(sceneBoundingRect(), m_speed_x, m_speed_y);

    if (no_collision_speed_y > m_speed_y && m_state == CharacterState::Fall)
    {
        m_state = CharacterState::Ground;
    }
    else if (m_speed_y > 0.01)
    {
        m_state = CharacterState::Fall;
    }
    else if (abs(m_speed_x) > 1)
    {
        m_state = CharacterState::Run;
    }
    else if (abs(m_speed_y) <= 0.01)
    {
        m_state = CharacterState::Idle;
    }

    updateAnimation();

    this->setPos(res.center());
}

void Character::updateAnimation()
{
    m_animation->setId(static_cast<uint8_t>(m_state));
}

bool Character::isOnGround()
{
    return m_state == CharacterState::Ground | m_state == CharacterState::Idle | m_state == CharacterState::Run;
}
