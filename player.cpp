#include "player.h"
#include "platform.h"

Player::Player(const QRectF &rect, const QColor &color) : Character(rect, color)
{
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

    m_animation = new SpriteAnimation(":/Pirate_bomb/1-Player-Bomb Guy/1-Idle", 50);

    setPixmap(m_animation->getPixmap());

    connect(m_animation, SIGNAL(updatePixmap()), this, SLOT(updateView()));

    m_type = CharacterType::Player;

    m_state =CharacterState::Idle;
    m_direction = CharacterDirection::Right;

    m_speed_x = 0;
    m_speed_y = 0;
    m_acc_x = 0;
    m_acc_y = 0;

    m_acc_max = 15;
    m_friction = 0.5;
    m_gravity = 13;

    m_jump = false;
}

void Player::gameUpdate(const Platform *platform)
{
    //    qDebug()<<"m_acc_x"<<m_acc_x;
    // with this model: S_n = S_{n-1} * f + Acc => Sn = Acc *(1 - f^n)/(1-f) => #with f < 1 and n >> 0: Sn = Acc/(1-f)
    m_speed_x *= m_friction; // Friction
    m_speed_x += m_acc_x;

    m_speed_y *= m_friction; // Friction
    m_speed_y += m_acc_y + m_gravity;

    // TODO: Better handle for State and direction
    if (m_speed_x >0.001){
        m_direction =CharacterDirection::Right;
    }else if (m_speed_x <-0.001){
        m_direction =CharacterDirection::Left;
    }

    if (m_speed_y <-0.001){
        m_state =CharacterState::Jump;
    }

//    qreal no_collision_speed_x=m_speed_x;
    qreal no_collision_speed_y=m_speed_y;

    QRectF res = platform->handleCollision(sceneBoundingRect(), m_speed_x, m_speed_y);

//    qDebug()<<"m_speed_x"<<m_speed_x<<"| m_speed_y"<<m_speed_y;

    if (no_collision_speed_y>m_speed_y && m_state ==CharacterState::Fall){
        m_state =CharacterState::Ground;
    }else if (m_speed_y>0.01){
        m_state =CharacterState::Fall;
    }else if (abs(m_speed_x)>0.01){
        m_state =CharacterState::Run;
    }else if (abs(m_speed_y)<=0.01){
        m_state =CharacterState::Idle;
    }

    updateAnimation();


    this->setPos(res.center());

    //    setPos(x() + m_speed_x, y() + m_speed_y);

    /*QList<QGraphicsItem *> collidingItems=this->collidingItems();

    if (!collidingItems.empty()){
        QRectF inter_all_rect=collidingItems[0]->sceneBoundingRect().intersected(this->sceneBoundingRect());//inter_all_rect(0,0,0,0);//
//        first_rect.set

        qreal dx=0;
        qreal dy=0;

        // Handle collisions with other items
        foreach (QGraphicsItem *item, collidingItems) {
            // Check if item is not itself
            if (item!=this) {
                // The qgraphicsitem_cast function takes care of checking whether the conversion is valid and returns the derived class pointer if successful, or nullptr if the conversion is not possible.
                if (Platform *platform= qgraphicsitem_cast<Platform *>(item)){
                    // Use platform to detect collision: 1)- use custom shape and only get the collision rect | 2)- check collision each time by yourself

                    platform->handleCollision(m_bounding_rect,m_speed_x,m_speed_y);
                }

            }
        }

    }*/
}

void Player::updateAnimation()
{
    switch (m_state) {
    case CharacterState::Idle:
        qDebug()<<"Idle";
        break;
    case CharacterState::Run:
        qDebug()<<"Run";
        break;
    case CharacterState::Jump:
        qDebug()<<"Jump";
        break;
    case CharacterState::Fall:
        qDebug()<<"Fall";
        break;
    case CharacterState::Ground :
        qDebug()<<"Ground";
        break;
    default:
        break;
    }
}

void Player::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();

    if (key == Qt::Key_Right)
    {
        m_acc_x = m_acc_max;
    }
    else if (key == Qt::Key_Left)
    {
        m_acc_x = -m_acc_max;
    }
    else if (key == Qt::Key_Up)
    {
        m_acc_y = -m_acc_max;
    }
    else if (key == Qt::Key_Down)
    {
        m_acc_y = m_acc_max;
    }
    else if (key == Qt::Key_Space)
    {
        //        qDebug()<<"check";
        // TMP (state: walking)
        if (!m_jump)
        {
            m_speed_y -= 300;
            m_jump = true;
        }
    }
}

void Player::keyReleaseEvent(QKeyEvent *event)
{
    int key = event->key();

    if (key == Qt::Key_Right)
    {
        m_acc_x = fmin(m_acc_x, 0);
    }
    else if (key == Qt::Key_Left)
    {
        m_acc_x = fmax(m_acc_x, 0);
    }
    else if (key == Qt::Key_Up)
    {
        m_acc_y = fmax(m_acc_y, 0);
    }
    else if (key == Qt::Key_Down)
    {
        m_acc_y = fmin(m_acc_y, 0);
    }
    else if (key == Qt::Key_Space)
    {
        m_jump = false;
    }
}
