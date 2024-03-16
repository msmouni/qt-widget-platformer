#include "character.h"
#include "bomb.h"
#include <QGraphicsScene>

Character::Character(const QPointF &pos, const QString &res_path, const Platform &platform) : m_platform(platform)
{
    //    this->setData(0,"Character");

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

    m_collision_rect= new CollisionRect(sceneBoundingRect(), m_speed_x, m_speed_y, this);

    //    m_animation->stop();

    //    coverage_rect=QGraphicsRectItem(m_bounding_rect,this);
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
    painter->drawRect(this->boundingRect());

    painter->drawPath(this->shape());

    /*QPen pen2(Qt::yellow);
    pen.setWidth(2);
    painter->setPen(pen2);
    painter->drawRect(coverage_rect.boundingRect());*/
    ///////////////////////////////////////////::
    painter->drawRect(this->sceneBoundingRect());

    pen.setWidth(9);
    painter->setPen(pen);
    painter->drawPoint(boundingRect().topLeft());
    painter->drawPoint(sceneBoundingRect().topLeft());
}

void Character::updateShapes()
{
    qDebug()<<this->sceneBoundingRect()<<m_collision_rect->sceneBoundingRect();

    m_speed_x *= m_friction; // Friction
    m_speed_x += m_acc_x;

    m_speed_y *= m_friction; // Friction
    m_speed_y += m_acc_y + m_gravity;

    // Note: m_bounding_rect should match the shape bounding rect: shape is at pos=(x,y) reg this->pos()
    m_bounding_rect = this->shape().boundingRect();

    m_collision_rect->update(sceneBoundingRect(), m_speed_x, m_speed_y);
}

const CharacterDirection &Character::getDirection() const
{
    return m_direction;
}

const CollisionRect *Character::getCollisionRect() const
{
    return m_collision_rect;
}

void Character::updateView()
{
    setPixmap(m_animation->getPixmap());
}

void Character::updateCharacter()
{
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


    if (abs(m_speed_x) > 1)
    {
        m_state = CharacterState::Run;
    }
    else if (abs(m_speed_y) <= 0.01)
    {
        m_state = CharacterState::Idle;
    }


    QVector<const CollisionRect *> dyn_collision_rects;
    QVector<QRectF> static_collision_rects;

    for (QGraphicsItem *item : m_collision_rect->collidingItems())
    {
        qDebug()<<"Check";
        if (item->data(0) == "Enemy" && m_type != CharacterType::Enemy || item->data(0) == "Player" && m_type != CharacterType::Player)
        {
            Character *chara = static_cast<Character *>(item);
            dyn_collision_rects.append(chara->m_collision_rect);
        }else if (item->data(0) =="Bomb"){
            Bomb *bomb=static_cast<Bomb *>(item);
            if (!bomb->isActive())
            {
                dyn_collision_rects.append(bomb->getCollisionRect());
            }
        }
        else if (item->data(0) == "Tile")
        {
            Tile *tile = static_cast<Tile *>(item);

            if (tile->isSolid() | (!tile->isEmpty() & ((tile->checkUp() && m_speed_y < 0) | (tile->checkDown() && m_speed_y > 0))))
            {
                static_collision_rects.append(tile->sceneBoundingRect());
            }
        }
    }

    m_collision_rect->handleCollision(dyn_collision_rects);
    m_collision_rect->handleCollision(static_collision_rects);
    //    qDebug()<<"End";

    m_speed_x = m_collision_rect->getSpeedX();
    m_speed_y = m_collision_rect->getSpeedY();
    QRectF res = m_collision_rect->getEntityRect().translated(m_speed_x, m_speed_y);


    /*m_speed_x = res.center().x() - sceneBoundingRect().center().x();
    m_speed_y = res.center().y() - sceneBoundingRect().center().y();*/
    //    qDebug()<<m_speed_x<<m_speed_y;

    //    this->moveBy(m_speed_x, m_speed_y);

    //    this->shape().

    /*for (QGraphicsItem *item : this->collidingItems())
    {
        if (item->data(0) == "Weapon")
        {
            Weapon *weapon = static_cast<Weapon *>(item);
            if (weapon->isActive())
            {
                m_state = CharacterState::Hit;

                // Note: Weapon rect + pos -> shape
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
        }else if (item->data(0) == "Enemy")
        {
//            qDebug()<<"Enemy";
//            Character *chara = static_cast<Character *>(item);
//            colliding_rects.append(chara->sceneBoundingRect());
        }

    }*/

    ////////////////////////////////////////////////////////////////////////////

    QPointF scene_adjustmnt = m_bounding_rect.topLeft();
    if (m_direction == CharacterDirection::Left)
    {
        scene_adjustmnt.setX(-scene_adjustmnt.x());
    }

    this->setPos(res.topLeft() - scene_adjustmnt); // 0,0);//res.topLeft());//

    //    this->moveBy(m_speed_x,m_speed_y);
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

/*QRectF Character::getBoundingRect()
{
    QRectF res= m_bounding_rect;

    res.moveTopLeft(pos());

//    qDebug()<<res<<sceneBoundingRect();

    return res;
}*/
