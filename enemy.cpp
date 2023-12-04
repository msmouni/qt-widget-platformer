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

    // TMP
    m_jumping_to_point=false;

}

void Enemy::gameUpdate(const Platform *platform)
{
//    QPoinF this->pos();

    // TMP
    if (m_jumping_to_point){
//        qDebug()<<"enemy jumping";

        qDebug()<<"enemy jumping"<<"v_x"<<m_speed_x<<"v_y"<<m_speed_y;

        if (m_speed_x <0.001){
            qDebug()<<"jumping false";
            m_acc_x=0;
            m_speed_x=0;
            m_jumping_to_point=false;
        }

    }else if (m_player_rect.y() < sceneBoundingRect().y()){
//        m_speed_y -= 300;
        QVector<QPointF> reached_tiles =platform->getReachedTiles(sceneBoundingRect(), 300, -300); // reachin_points:300

        if (!reached_tiles.isEmpty()){
            qreal x_0=sceneBoundingRect().x();
            qreal y_0=sceneBoundingRect().y();

            qreal x_n=reached_tiles[0].x();
            qreal y_n=reached_tiles[0].y();

            qreal n=10;

            // TMP
            qreal acc_x=0;
            qreal v_x= ((x_n-x_0)*(1 -m_friction))/(1- pow(m_friction,n+1));
//            qreal v_x= ((x_n-x_0)*(1 -m_friction) - (n+1)*acc_x)/(1- pow(m_friction,n+1)) + acc_x;
            qreal v_y= ((y_n-y_0)*(1 -m_friction) - (n+1)*m_gravity)/(1- pow(m_friction,n+1)) + m_gravity;

            qDebug()<<"enemy START jumping"<<"v_x"<<v_x<<"v_y"<<v_y;

            m_acc_x=acc_x;
            m_speed_x=v_x;
            m_speed_y=-v_y;

            m_jumping_to_point=true;
        }
    }else if (abs(m_player_rect.x() - sceneBoundingRect().x())<500){
        m_acc_x = (m_player_rect.x() - sceneBoundingRect().x())/50;
    //    m_acc_y = (m_player_rect.y() - sceneBoundingRect().y())/10;

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
