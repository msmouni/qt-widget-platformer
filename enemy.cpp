#include "enemy.h"


Enemy::Enemy(const QRectF &rect, const QColor &color,const QRectF & player_rect): Character(rect,color), m_player_rect(player_rect)
{

    QHash<uint8_t, QString> animations_ids;
    animations_ids.insert(0,":/Pirate_bomb/2-Enemy-Bald Pirate/1-Idle");
    animations_ids.insert(1,":/Pirate_bomb/2-Enemy-Bald Pirate/2-Run");
    animations_ids.insert(2,":/Pirate_bomb/2-Enemy-Bald Pirate/4-Jump");
    animations_ids.insert(3,":/Pirate_bomb/2-Enemy-Bald Pirate/5-Fall");
    animations_ids.insert(4,":/Pirate_bomb/2-Enemy-Bald Pirate/6-Ground");
    animations_ids.insert(5,":/Pirate_bomb/2-Enemy-Bald Pirate/7-Hit");
    animations_ids.insert(6,":/Pirate_bomb/2-Enemy-Bald Pirate/8-Dead Hit");

    m_animation = new SpriteAnimation(animations_ids, 50);

    setPixmap(m_animation->getPixmap());

    connect(m_animation, SIGNAL(updatePixmap()), this, SLOT(updateView()));

    m_type = CharacterType::Enemy;

}

void Enemy::gameUpdate(const Platform *platform)
{
//    QPoinF this->pos();

    // TMP
    if (abs(m_player_rect.x() - sceneBoundingRect().x())<500){
        m_acc_x = (m_player_rect.x() - sceneBoundingRect().x())/50;
    //    m_acc_y = (m_player_rect.y() - sceneBoundingRect().y())/10;
        if (m_player_rect.y() < sceneBoundingRect().y()){
            m_speed_y -= 300;
        }
    }else {
        m_acc_x=0;
        m_speed_x=0;
        m_speed_y=0;
    }

    updateCharacter(platform);

//    qDebug()<<"player rect"<<m_player_rect<<"rect"<<sceneBoundingRect()<<"m_acc_x"<<m_acc_x<<"m_acc_y"<<m_acc_y;
}

/*void Enemy::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(QBrush(m_color));
    painter->drawRect(m_bounding_rect);

}*/

/*void Enemy::updateAnimation()
{

}*/
