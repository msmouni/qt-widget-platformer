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

//        qDebug()<<"enemy jumping"<<"v_x"<<m_speed_x<<"v_y"<<m_speed_y;

        /*if (abs(m_speed_x) <0.001 && abs(m_speed_y)<0.001){
            qDebug()<<"jumping false";
            m_acc_x=0;
            m_speed_x=0;
            m_acc_y=0;
            m_speed_y=0;
            m_jumping_to_point=false;
        }*/

    }else if (m_player_rect.y() < sceneBoundingRect().y() && isOnGround()){
//        m_speed_y -= 300;
        QVector<QPointF> reached_tiles;
        if (m_player_rect.x() >=sceneBoundingRect().x() ){
            qDebug()<<"Right";
            reached_tiles=platform->getReachedTiles(sceneBoundingRect(), 300, -300); // reachin_points:300
        }else {
            qDebug()<<"Left";
            reached_tiles=platform->getReachedTiles(sceneBoundingRect(), -300, -300); // reachin_points:300
        }


        if (!reached_tiles.isEmpty()){
            qreal x_0=sceneBoundingRect().x();
            qreal y_0=sceneBoundingRect().y();

            qreal x_n;//=reached_tiles[0].x();
            qreal y_n;//=reached_tiles[0].y() - 2*sceneBoundingRect().height();
            bool jump=false;

            for (QPointF pnt:reached_tiles ){
                if (pnt.y()>=m_player_rect.y() && pnt.y()<y_0) {// && abs(pnt.x()-x_0)<abs(m_player_rect.x()-x_0) +50){
                    x_n=pnt.x();
                    y_n=pnt.y()- 2*sceneBoundingRect().height();
                    jump=true;

                    break;
                }
            }

            if (jump){
                qDebug()<<"rect:"<<sceneBoundingRect()<<"pl_y"<<m_player_rect.y()<<"rt:"<<reached_tiles;

                qreal n=10;

                // TMP
                /*qreal acc_x=0;
                qreal v_x= ((x_n-x_0)*(1 -m_friction))/(1- pow(m_friction,n+1));
    //            qreal v_x= ((x_n-x_0)*(1 -m_friction) - (n+1)*acc_x)/(1- pow(m_friction,n+1)) + acc_x;
                qreal v_y= ((y_n-y_0)*(1 -m_friction) - (n+1)*m_gravity)/(1- pow(m_friction,n+1)) + m_gravity;*/

                /*qreal vx_n=0;
                qreal vy_n=0;

                qreal c_a=( (1-pow(m_friction,n+1))/(1-m_friction) - pow(m_friction,n)*( (n+1)/(1-pow(m_friction,n+1)) -m_friction) );
                qreal bx_a= - pow(m_friction,n)*(x_n-x_0)*(1-m_friction)/ (1-pow(m_friction,n+1));
                qreal by_a= - pow(m_friction,n)*(y_n-y_0)*(1-m_friction)/ (1-pow(m_friction,n+1));

                qreal acc_x= (vx_n -bx_a)/c_a;
                qreal acc_y= (vy_n -by_a)/c_a -m_gravity;

                qreal v_x=(vx_n - acc_x*(1-pow(m_friction,n+1))/(1-m_friction))/pow(m_friction,n);
                qreal v_y=(vy_n - acc_y*(1-pow(m_friction,n+1))/(1-m_friction))/pow(m_friction,n) -m_gravity;*/

    //            // TMP: TODO: x_n=x_0
    //            qreal a=(y_n-y_0)/(pow(x_n,2)- pow(x_0,2));
    //            qreal b= y_0 - a*pow(x_0,2);
                /////////////////////////////////////////////////////////////////////////

                qreal vx_n=0;
                qreal vy_n=0;

                qreal c1=pow(m_friction,n);
                qreal c2=(1-(pow(m_friction,n)))/(1-m_friction);

                qreal c3=(m_friction-(pow(m_friction,(n+1))))/(1-m_friction);
                qreal c4=(pow(m_friction,n))*( ((pow(m_friction,n)) -1)/((pow(m_friction,n-1))*(pow((m_friction-1),2))  ) - n/((pow(m_friction,n+1)) - (pow(m_friction,n)) ));

                qreal acc_x=(x_n - x_0 - (vx_n/c1))/(c4 -(c2/c1));
                qreal acc_y=0;//(y_n - y_0 - (vy_n/c1))/(c4 -(c2/c1)) - m_gravity

                qreal v0_x=(vx_n - acc_x*c2)/c1;
                qreal v0_y=(y_n - y_0 -m_gravity*c4 )/c3;//(1/(m_friction**n))*(vy_n-m_gravity*c2) #(vy_n - (acc_y + m_gravity)*c2)/c1

                qDebug()<<"enemy START jumping"<< "acc_x"<<acc_x<<"acc_y"<<acc_y<<"v0_x"<<v0_x<<"v0_y"<<v0_y;

                m_acc_x=acc_x;
                m_acc_y=acc_y;
                m_speed_x=v0_x;
                m_speed_y=v0_y;

                m_jumping_to_point=true;
            }
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

    if (m_jumping_to_point && m_state == CharacterState::Ground){
        qDebug()<<"jumping false";
        m_acc_x=0;
        m_speed_x=0;
        m_acc_y=0;
        m_speed_y=0;
        m_jumping_to_point=false;
    }

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
