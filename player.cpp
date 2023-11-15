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
                /*if (Platform *platform= qgraphicsitem_cast<Platform *>(item)){
                    // Use platform to detect collision: 1)- use custom shape and only get the collision rect | 2)- check collision each time by yourself
                }*/
                if (Tile *tile= qgraphicsitem_cast<Tile *>(item)){
                    QRectF tile_rect=tile->sceneBoundingRect();
                    QRectF player_rect=this->sceneBoundingRect();

    //                qDebug()<<"tile"<<tile_rect<<"| player:"<<player_rect;

                    /*if (m_speed_x>0.01){
                        m_speed_x=0;
                        this->setX(tile_rect.left()-  player_rect.width()/2);
                    }else if (m_speed_x<-0.01){
                        m_speed_x=0;
                        this->setX(tile_rect.right()+  player_rect.width()/2);
                    }else if (m_speed_y>0.01){
                        qDebug()<<m_speed_y;
                        m_speed_y=0;
                        this->setY(tile_rect.top()-  player_rect.height()/2);
                    }else if (m_speed_y<-0.01){
                        qDebug()<<m_speed_y;
                        m_speed_y=0;
                        this->setY(tile_rect.bottom()+  player_rect.height()/2);
                    }*/
                    /////////////////////////////////////////////////////////////////////////////////
    //                QRectF inter_rect=tile_rect.intersected(player_rect);
    ////                qreal r_w=9999;
    ////                qreal r_h=9999;
    //                qreal r_pos=sqrt(pow(m_speed_x,2) + pow(m_speed_y,2));
    //                qreal r=9999;
    //                bool move=false;
    //                if (abs(m_speed_x)>0.1){
    //                    r = fmin(r,(inter_rect.width() *r_pos)/abs(m_speed_x));
    //                    move=true;
    //                }
    //                if (abs(m_speed_y)>0.1){
    //                    r = fmin(r,(inter_rect.height() *r_pos)/abs(m_speed_y));
    //                    move=true;
    //                }

    ////                qreal r = fmin(r_w,r_h);

    //                if (move){
    //                    // set position at the end
    //                    qDebug()<< r;
    //                    moveBy(-r*m_speed_x/r_pos,-r*m_speed_y/r_pos);
    //                    m_speed_x=0;
    //                    m_speed_y=0;
    //                    // NOTE: do not return posion (slide on walls ...)
    //                }
                    /////////////////////////////////////////////////////////////////////////////////
                    QRectF inter_rect=tile_rect.intersected(player_rect);
                    qDebug()<<inter_rect;
                    // Factorize later
                    /*if (abs(inter_rect.width()-inter_rect.height())<1 ){
                        qDebug()<<"1";
    //                    m_speed_x=0;
                        if (player_rect.center().x() <= tile_rect.center().x()){
                            setX(x()-inter_rect.width()-1);
                        }else {
                            setX(x()+inter_rect.width()+1);
                        }

    //                    m_speed_y=0;
                        if (player_rect.center().y() <= tile_rect.center().y()){
                            setY(y()-inter_rect.height()-1);
                        }else {
                            setY(y()+inter_rect.height()+1);
                        }

                    }else */
                    /*if ((inter_rect.height()-inter_rect.width())>0.001){
                        qDebug()<<"2";
                        m_speed_x=0;
                        if (player_rect.center().x() <= tile_rect.center().x()){
                            setX(x()-inter_rect.width());
                        }else {
                            setX(x()+inter_rect.width());
                        }
                    }else if ((inter_rect.width()-inter_rect.height())>0.001){
                        qDebug()<<"3";
                        m_speed_y=0;
                        if (player_rect.center().y() <= tile_rect.center().y()){
                            setY(y()-inter_rect.height());
                        }else {
                            setY(y()+inter_rect.height());
                        }
                    }*/
                    //////////////////////////////////////////////
                    inter_all_rect.setLeft(fmin(inter_all_rect.left(),inter_rect.left()));
                    inter_all_rect.setRight(fmax(inter_all_rect.right(),inter_rect.right()));
                    inter_all_rect.setTop(fmin(inter_all_rect.top(),inter_rect.top()));
                    inter_all_rect.setBottom(fmax(inter_all_rect.bottom(),inter_rect.bottom()));
                    /////////////////////////////////////////////////////////////////////////////////
                    if ((inter_rect.height()-inter_rect.width())>0.001){
                        dx= fmax(dx,abs(inter_rect.width()));
                    }else if ((inter_rect.width()-inter_rect.height())>0.001){
                        dy= fmax(dy,inter_rect.height());
                    }
                }

            }
        }
        /*qDebug()<<"inter_all_rect"<<inter_all_rect;

        if ((inter_all_rect.height()-inter_all_rect.width())>0.001){
            qDebug()<<"2";

            if (m_speed_x>0.001){
                setX(x()-(inter_all_rect.width()));
            }else {
                setX(x()+inter_all_rect.width());
            }
            m_speed_x=0;
        }else if ((inter_all_rect.width()-inter_all_rect.height())>0.001){
            qDebug()<<"3";

            if (m_speed_y>0.001){
                setY(y()-inter_all_rect.height());
            }else {
                setY(y()+inter_all_rect.height());
            }

            m_speed_y=0;
        }*/
        /////////////////////////////////////////////////////////////////////////////////
        qDebug()<<"dx"<<dx<<"dy"<<dy;
        if (dx>0.001){
            qDebug()<<"2";

            if (m_speed_x>0.001){
                setX(x()-dx);
            }else if (m_speed_x<-0.001) {
                setX(x()+dx);
            }
            m_speed_x=0;
        }

        if (dy>0.001){
            qDebug()<<"3";

            if (m_speed_y>0.001){
                setY(y()-dy);
            }else  if (m_speed_y<-0.001){
                setY(y()+dy);
            }

            m_speed_y=0;
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
