#ifndef COLLISION_H
#define COLLISION_H

#include <QRectF>
#include <QVector>
#include <QGraphicsRectItem>
#include <QPainter>

// To Rename : Dynamics (character + weapon ...)
class CollisionRect : public QGraphicsRectItem
{
public:
    CollisionRect(QGraphicsItem *parent, qreal &speed_x, qreal &speed_y, qreal &weight);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    QPainterPath shape() const override;

    void handleCollision(QRectF &new_rect, const QRectF &old_rect, qreal &speed_x, qreal &speed_y, qreal &weight_x, qreal &weight_y);
    void handleDynamicCollision(CollisionRect &other);
    void handleDynamicCollision(QVector<CollisionRect *>);
    void handleStaticCollision(QRectF rect);
    void handleStaticCollision(QVector<QRectF> rects);
    void handleCollision(CollisionRect *caller = nullptr);

    void setEntityRect(QRectF new_rect);
    QRectF getEntityRect();

    QPointF getEntityPos();

    void update();

    void setMargin(QMarginsF margin);

    bool isBottomCollision();

private:
    const qreal M_COLLISION_MARGIN = 1e-3;

    QRectF m_bounding_rect;

    QRectF m_old_rect;
    QRectF m_new_rect;
    qreal &m_speed_x;
    qreal &m_speed_y;

    qreal &m_weight;
    qreal m_weight_x;
    qreal m_weight_y;

    bool m_collision_handeled;
    bool m_is_top_collision;
    bool m_is_bottom_collision;
    bool m_is_left_collision;
    bool m_is_right_collision;
    QMarginsF m_margin;

    qreal distance(const QRectF &rect1, const QRectF &rect2) const;
    bool compareDistance(const QRectF &rect1, const QRectF &rect2, const QRectF &targetRect) const;
};

#endif // COLLISION_H
