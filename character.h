#ifndef CHARACTER_H
#define CHARACTER_H

#include <QGraphicsItem>
#include <QRectF>
#include <QPainter>
#include <QColor>
#include <QTimer>
#include <QObject>
#include <QElapsedTimer>

// Jumping: Timer(ay>0)
enum class CharacterState{
    Idle,
    Jumping,
    Falling,
    Walking,
};

// Pix / meter (shared: Mutex)

class Character : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    Character(const QRectF& rect, const QColor& color)
        : m_bounding_rect(rect), m_color(color) {
            m_bounding_rect=QRectF(-rect.width()/2,-rect.height()/2,rect.width(),rect.height());
            this->setPos(rect.center());

            m_timer = new QTimer();

            m_state=CharacterState::Idle;

            // TODO: move to Player
            m_nb_jump=0;
            m_nb_jump_done=0;
            m_jump_timer=new QElapsedTimer();
            m_jump_timer->start();
            m_jump_timeout_ms=30;//100; // 100 ms


            m_jump_pressed=false;
            m_left_pressed=false;
            m_right_pressed=false;
            m_down_pressed=false;

            // TO MOVE LATER
            m_timer->setInterval(10); // 50 ms
            m_timer->start();
            connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
     };

    QRectF boundingRect() const override
    {
        return m_bounding_rect;
//        qreal penWidth = 1;
//        return QRectF(-10 - penWidth / 2, -10 - penWidth / 2,
//                      20 + penWidth, 20 + penWidth);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override
    {
        painter->setBrush(QBrush(m_color));
        painter->drawRect(m_bounding_rect);
    }

    /*void jump(){
        m_ay=-100*m_g; // apply during some delta_t
    }*/

    void up_pressed(){
        if (!m_jump_pressed){
            if (m_nb_jump<2){
                m_nb_jump+=1;
            }
        }
        m_jump_pressed=true;
    }
    void up_released(){
        m_jump_pressed=false;
    }
    void right_pressed(){
        m_right_pressed=true;
    }
    void right_released(){
        m_right_pressed=false;
    }
    void left_pressed(){
        m_left_pressed=true;
    }
    void left_released(){
        m_left_pressed=false;
    }
    void down_pressed(){
        m_down_pressed=true;
    }
    void down_released(){
        m_down_pressed=false;
    }

private:
    QRectF m_bounding_rect;
    QColor m_color; // To change later with sprite

    // TODO: move later to another class that handles physics
    //    qreal m_weight=70; //70kg
    qreal m_g=700;//9.8;
    qreal m_px;
    qreal m_py;
    qreal m_sx=0;
    qreal m_sy=0;
    qreal m_ax=0;
    qreal m_ay=0;


    QTimer* m_timer;
    CharacterState m_state;

    // TODO: move to Player
    uint8_t m_nb_jump;
    uint8_t m_nb_jump_done=0;
    QElapsedTimer* m_jump_timer;
    int m_jump_timeout_ms;

    bool m_jump_pressed;
    bool m_left_pressed;
    bool m_right_pressed;
    bool m_down_pressed;

    void updatePosition(){
        QPointF position= pos();
        qDebug()<<position;

        qreal t_s=0.01; // 10ms

        m_sx=m_ax*t_s+m_sx;
        qreal x=m_sx*t_s +position.x();

        m_sy=(m_g+ m_ay)*t_s+m_sy;
        qreal y=m_sy*t_s +position.y();


//        qDebug()<<"Check"<<"x_n_1"<<position<<"x_n"<<x<<","<<y;

        setPos(x,y);

//        m_ax=0;
//        m_ay=0;

        QList<QGraphicsItem *> collidingItems=this->collidingItems();


        // Handle collisions with other items
        foreach (QGraphicsItem *item, collidingItems) {
            // Check if item is not itself
            if (item->data(0) == "Platform") {
                qDebug()<<"Collision";
                // check direction: landed
                QPointF item_pos=item->scenePos();

                qDebug()<<"item:"<<item_pos<<"this"<<pos()<<"scene"<<this->scenePos();
                switch (m_state) {
                case CharacterState::Falling : {
                    if (item_pos.y()>y){
                        m_sy=0;
                        m_ay=-m_g;
//                        setPos(position);
                        setY(position.y());
                        m_state=CharacterState::Idle;
                    }

                    break;
                }
                case CharacterState::Jumping : {
                    if (item_pos.y()<y){
                        qDebug()<<"checkk jump";
                        m_sy=0;
                        m_ay=-m_g;
//                        setPos(position);
                        setY(position.y());
                    }

                    break;
                }
                case CharacterState::Walking : {

                    if (item_pos.y()>y){
                        m_ay=-m_g;
                        setY(position.y());
                        m_state=CharacterState::Walking;
                    }

                    break;
                }
                default:
                    break;
                }

                if ((m_left_pressed & (item_pos.x()<x) ) || (m_right_pressed & (item_pos.x()>x) )){
                    qDebug()<<"checkk left_right";
                    m_sx=0;
                    m_ax=0;
                    //                        setPos(position);
                    setX(position.x());
                }
//                m_sx=0;
//                m_sy=0;
//                m_ax=0;
//                m_ay=-m_g;
//                setPos(position);

            }
        }
    }


private slots:
    void update(){
        switch (m_state) {
        case CharacterState::Idle : {
            qDebug()<<"Idle";
//            m_ay=-m_g;

            if (m_jump_pressed){
                m_state=CharacterState::Jumping;
            } else if (m_left_pressed | m_right_pressed){
                m_state=CharacterState::Walking;
            }


            break;
        }
        case CharacterState::Falling : {
            qDebug()<<"Falling "<<m_sy;
            if (m_sy<=0){
                m_state=CharacterState::Idle;
            }

            if(m_left_pressed){
                m_sx=-50;
            }else if (m_right_pressed){
                m_sx=50;
            }else {
                m_sx=0;
            }

            updatePosition();

            break;
        }
        case CharacterState::Jumping : {
            qDebug()<<"Jumping"<<"m_sy"<<m_sy<<"m_ay"<<m_ay;
            if (m_nb_jump_done<m_nb_jump){
                m_ay=-10*m_g;
                m_jump_timer->restart();
                m_nb_jump_done+=1;
            } else if (m_jump_timer->hasExpired(m_jump_timeout_ms)){
                m_ay=0;

                if (m_sy>0){
                    m_nb_jump=0;
                    m_nb_jump_done=0;
                    m_state=CharacterState::Falling;
                }
            }

            if(m_left_pressed){
                m_sx=-50;
            }else if (m_right_pressed){
                m_sx=50;
            }else {
                m_sx=0;
            }

            updatePosition();

            break;
        }
        case CharacterState::Walking : {
            qDebug()<<"Walking";
            // Move later
            m_ay=0;
//            m_state=CharacterState::Falling;
            // Maybe use acceleration and limit speed
            // TODO: Factorize with jumping + Falling
            if (m_jump_pressed) {
                m_state=CharacterState::Jumping;
            }else if(m_left_pressed){
                m_sx=-50;
            }else if (m_right_pressed){
                m_sx=50;
            }else {
                m_sx=0;
//                m_state=CharacterState::Idle;
                m_state=CharacterState::Falling;
            }

            updatePosition();


            break;
        }
        default:
            break;
        }




    };

};

#endif // CHARACTER_H
