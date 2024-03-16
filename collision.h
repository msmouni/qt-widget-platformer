#ifndef COLLISION_H
#define COLLISION_H

#include <QRectF>
#include <QVector>

// To Rename : Dynamics (character + weapon ...)
class CollisionRect{
public:
    CollisionRect();

    void handleCollision(const CollisionRect &other);
// TMP
//private:
    const qreal M_COLLISION_MARGIN = 1e-3;

    QRectF m_old_rect;
    QRectF m_new_rect;
    qreal m_speed_x;
    qreal m_speed_y;
    bool m_is_static;
};

class Collision
{
public:
    Collision();

    QRectF handle(QRectF prev_rect, QRectF new_rect, QVector<QRectF> colliding_rects) const;

private:
    const qreal M_COLLISION_MARGIN = 1e-3;

    qreal distance(const QRectF &rect1, const QRectF &rect2) const;
    bool compareDistance(const QRectF &rect1, const QRectF &rect2, const QRectF &targetRect) const;
};

#endif // COLLISION_H
