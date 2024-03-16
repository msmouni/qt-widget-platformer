#include "player.h"
#include "platform.h"
#include "bomb.h"
#include <QGraphicsScene>

Player::Player(const QPointF &pos, const QString &res_path, const Platform &platform) : Character(pos, res_path, platform)
{
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

    setData(0, "Player");

    m_type = CharacterType::Player;

    m_weapons_count = 0;
}

void Player::gameUpdate()
{
    updateCharacter();
}

void Player::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();

    if (key == Qt::Key_Right)
    {
        moveRight();
    }
    else if (key == Qt::Key_Left)
    {
        moveLeft();
    }
    else if (key == Qt::Key_Up)
    {
        m_dynamics->setAccelY(-M_ACCEL_MAC);
    }
    else if (key == Qt::Key_Down)
    {
        m_dynamics->setAccelY(M_ACCEL_MAC);
    }
    else if (key == Qt::Key_Space)
    {
        jump();
    }
    else if (key == Qt::Key_B)
    {
        qreal dir_x = 1;
        if (m_dynamics->getDirection() == EntityDirection::MovingLeft)
        {
            dir_x = -1;
        }

        // Note: pixmap pos (-50 ...)
        Bomb *wpn = new Bomb(m_weapons_count, sceneBoundingRect().topLeft() - QPointF(50, 50), dir_x * 100, -100, 250, 250, ":/Pirate_bomb/Objects/BOMB");

        wpn->start();
        this->scene()->addItem(wpn);
        connect(wpn, SIGNAL(terminate(Weapon *)), this, SLOT(dropWeapon(Weapon *)));
        m_weapons.insert(m_weapons_count, wpn);

        m_weapons_count += 1;
    }
}

void Player::keyReleaseEvent(QKeyEvent *event)
{
    int key = event->key();

    if (key == Qt::Key_Right)
    {
        stopRight();
    }
    else if (key == Qt::Key_Left)
    {
        stopLeft();
    }
    else if (key == Qt::Key_Up)
    {
        m_dynamics->setAccelY(fmax(m_dynamics->getAccelY(), 0));
    }
    else if (key == Qt::Key_Down)
    {
        m_dynamics->setAccelY(fmin(m_dynamics->getAccelY(), 0));
    }
    else if (key == Qt::Key_Space)
    {
        stopJump();
    }
}
