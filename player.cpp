#include "player.h"
#include "platform.h"
#include "weapon.h"

Player::Player(const QPointF & pos, const QString &res_path, const Platform &platform) : Character(pos, res_path, platform)
{
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

    m_jump_timer.setSingleShot(true);
    connect(&m_jump_timer, &QTimer::timeout, this, &Player::jumpTimeout);

    setData(0, "Player");
    m_type = CharacterType::Player;
}

void Player::gameUpdate()
{
    updateCharacter();
}

void Player::jumpTimeout()
{
    m_acc_y = 0;
}

void Player::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();

    if (key == Qt::Key_Right)
    {
        m_acc_x = m_acc_max/2;
    }
    else if (key == Qt::Key_Left)
    {
        m_acc_x = -m_acc_max/2;
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
        if (isOnGround())
        {
            m_jump_timer.start(M_JUMP_TIMEOUT_MS);
            m_acc_y = M_JUMP_ACCEL;
        }
    }else if (key == Qt::Key_A){
        qDebug()<<"A";
        for (QGraphicsItem* item: this->collidingItems()){
            if (item->data(0) =="Weapon"){
                Weapon* weapon= static_cast<Weapon*>(item);

                weapon->activate();
            }
        }
    }else if (key == Qt::Key_S){
        qDebug()<<"S";
        for (QGraphicsItem* item: this->collidingItems()){
            if (item->data(0) =="Weapon"){
                qDebug()<<"Weapon";
                Weapon* weapon= static_cast<Weapon*>(item);

                weapon->start();
            }
        }
    }
//    else if (key == Qt::Key_B){
//        qDebug()<<"B";
//        // TMP
////        Weapon wpn=Weapon(QRectF(x(), y(), 58, 58), QRectF(x(), y(), 200, 200), 25,25,m_platform, ":/Pirate_bomb/Objects/BOMB");
////        m_weapons.append(wpn);
////        m_weapons.append(Weapon(QRectF(x(), y(), 58, 58), QRectF(x(), y(), 200, 200), 25,25,m_platform, ":/Pirate_bomb/Objects/BOMB"));
//        m_weapon =new Weapon(QRectF(x(), y(), 58, 58), QRectF(x(), y(), 200, 200), 25,25,m_platform, ":/Pirate_bomb/Objects/BOMB");

//        this->scene()->
//    }
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
        m_acc_y = 0;
    }
}
