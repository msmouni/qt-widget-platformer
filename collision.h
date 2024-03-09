#ifndef COLLISION_H
#define COLLISION_H

#include <QRectF>
#include <QVector>
#include <QGraphicsRectItem>
#include <QPainter>

class CollisionRect : public QGraphicsRectItem
{
public:
    CollisionRect(QRectF entity_rect, qreal speed_x = 0, qreal speed_y = 0, QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    QPainterPath shape() const override;

    void handleCollision(const QRectF &new_rect, const QRectF &old_rect, qreal speed_x, qreal speed_y);
    void handleDynamicCollision(const CollisionRect &other);
    void handleDynamicCollision(QVector<const CollisionRect *>);
    void handleStaticCollision(QRectF rect);
    void handleStaticCollision(QVector<QRectF> rects);
    void handleCollision();

    void setEntityRect(QRectF new_rect);
    QRectF getEntityRect();

    qreal getSpeedX() const;
    qreal getSpeedY() const;
    void setSpeedX(qreal speed_x);
    void setSpeedY(qreal speed_y);

    void update(QRectF new_rect, qreal speed_x, qreal speed_y);

    bool isBottomCollision();

private:
    const qreal M_COLLISION_MARGIN = 1e-3;

    QRectF m_bounding_rect;

    QRectF m_old_rect;
    QRectF m_new_rect;
    qreal m_speed_x;
    qreal m_speed_y;

    bool m_is_static;

    bool m_is_top_collision;
    bool m_is_bottom_collision;
    bool m_is_left_collision;
    bool m_is_right_collision;

    qreal distance(const QRectF &rect1, const QRectF &rect2) const;
    bool compareDistance(const QRectF &rect1, const QRectF &rect2, const QRectF &targetRect) const;
};

#endif // COLLISION_H
