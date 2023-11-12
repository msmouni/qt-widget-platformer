#include "player.h"
#include "platform.h"

Player::Player(const QRectF &rect, const QColor &color) : Character(rect, color)
{
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

    m_type = CharacterType::Player;
    m_speed_x = 0;
    m_speed_y = 0;
    m_acc_x = 0;
    m_acc_y = 0;

    m_acc_max = 15;
    m_friction = 0.5;
}

void Player::update()
{
    // with this model: S_n = S_{n-1} * f + Acc => Sn = Acc *(1 - f^n)/(1-f) => #with f < 1 and n >> 0: Sn = Acc/(1-f)
    m_speed_x *= m_friction; // Friction
    m_speed_x += m_acc_x;

    m_speed_y *= m_friction; // Friction
    m_speed_y += m_acc_y;

    setPos(x() + m_speed_x, y() + m_speed_y);

    QList<QGraphicsItem *> collidingItems=this->collidingItems();


    // Handle collisions with other items
    foreach (QGraphicsItem *item, collidingItems) {
        // Check if item is not itself
        if (item!=this) {
            // The qgraphicsitem_cast function takes care of checking whether the conversion is valid and returns the derived class pointer if successful, or nullptr if the conversion is not possible.
            if (Platform *platform= qgraphicsitem_cast<Platform *>(item)){
                // Use platform to detect collision: 1)- use custom shape and only get the collision rect | 2)- check collision each time by yourself
            }
//            if (item->data(0) =="Platform"){
////                for (QGraphicsItem* child :item->childItems()){

////                }
//// Use with shape:
////                    if (item->collidesWithItem(this)){
////                        qDebug() << "Plt";
////                    }
//            }
//            qDebug()<<"Collision";
////            item->collidesWithItem(this);
        }
    }

//    qDebug() << "m_speed_x" << m_speed_x << "m_speed_y" << m_speed_y;

    /*m_speed_x=(m_acc_x-m_friction*m_speed_x)*delta_t_s+m_speed_x ; // TODO: Limit speed
    setX(m_speed_x*delta_t_s +x());

    m_speed_y=m_acc_y*delta_t_s+m_speed_y ;//-m_friction*m_speed_y;
    setY(m_speed_y*delta_t_s +y());*/
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
}

void Player::keyReleaseEvent(QKeyEvent *event)
{
    int key = event->key();

    if (key == Qt::Key_Right | key == Qt::Key_Left)
    {
        m_acc_x = 0;
    }
    else if (key == Qt::Key_Up | key == Qt::Key_Down)
    {
        m_acc_y = 0;
    }
}
