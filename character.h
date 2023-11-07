#ifndef CHARACTER_H
#define CHARACTER_H

#include <QGraphicsItem>
#include <QRectF>
#include <QPainter>
#include <QColor>
#include <QTimer>
#include <QObject>

class Character : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    Character(const QRectF& rect, const QColor& color)
        : m_bounding_rect(rect), m_color(color) {
            // TO MOVE LATER
            m_timer.setInterval(10); // 50 ms
            m_timer.start();
            connect(&m_timer, SIGNAL(timeout()), this, SLOT(update()));
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

    void jump(){
        m_ay=-100*m_g; // apply during some delta_t
    }

private:
    QRectF m_bounding_rect;
    QColor m_color; // To change later with sprite

    // TODO: move later to another class that handles physics
    qreal m_px;
    qreal m_py;
    qreal m_sx=0;
    qreal m_sy=0;
    qreal m_ax=0;
    qreal m_ay=0;
//    qreal m_weight=70; //70kg
    qreal m_g=700;//9.8;
    QTimer m_timer;

private slots:
    void update(){


        QPointF position= pos();

        qreal t_s=0.01; // 50ms

        m_sx=m_ax*t_s+m_sx;
        qreal x=m_sx*t_s +position.x();

        m_sy=(m_g+ m_ay)*t_s+m_sy;
        qreal y=m_sy*t_s +position.y();


        qDebug()<<"Check"<<"x_n_1"<<position<<"x_n"<<x<<","<<y;

        setPos(x,y);

        m_ax=0;
        m_ay=0;

        QList<QGraphicsItem *> collidingItems=this->collidingItems();


        // Handle collisions with other items
        foreach (QGraphicsItem *item, collidingItems) {
            // Check if item is not itself
            if (item->data(0) == "Enemy") {
                // check direction: landed
                m_ay=-m_g;
                setPos(position);
            }
        }


    };

};

#endif // CHARACTER_H
