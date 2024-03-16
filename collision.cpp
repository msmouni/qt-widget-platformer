#include "collision.h"
#include <algorithm>
#include <QDebug>


CollisionRect::CollisionRect(QRectF entity_rect, qreal speed_x, qreal speed_y, QGraphicsItem *parent)
{
    this->setParentItem(parent);

    // TMP
    m_bounding_rect = entity_rect;
    m_old_rect = entity_rect;
    m_new_rect = entity_rect;
    m_speed_x = speed_x;
    m_speed_y = speed_y;
    m_is_static = true;
}

QRectF CollisionRect::boundingRect() const
{
    return m_bounding_rect;
}

void CollisionRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen(Qt::yellow);
    pen.setWidth(2);
    painter->setPen(pen);
//    painter->drawRect(boundingRect());
    painter->drawPath(this->shape());
}

QPainterPath CollisionRect::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void CollisionRect::setEntityRect(QRectF new_rect)
{
    m_old_rect = m_new_rect;
    m_new_rect = new_rect;
}

QRectF CollisionRect::getEntityRect()
{
    return m_new_rect;
}

qreal CollisionRect::getSpeedX() const
{
    return m_speed_x;
}

qreal CollisionRect::getSpeedY() const
{
    return m_speed_y;
}

void CollisionRect::setSpeedX(qreal speed_x)
{
    m_speed_x = speed_x;
}

void CollisionRect::setSpeedY(qreal speed_y)
{
    m_speed_y = speed_y;
}

void CollisionRect::update(QRectF new_rect, qreal speed_x, qreal speed_y)
{
//    qDebug()<<this->boundingRect()<<this->sceneBoundingRect()<<this->collidingItems()<<this->scenePos();
    setEntityRect(new_rect);
    setSpeedX(speed_x);
    setSpeedY(speed_y);

    QRectF entity_bounding_rect = this->mapRectFromScene(new_rect);

    m_bounding_rect=entity_bounding_rect.marginsAdded(QMarginsF(entity_bounding_rect.width() / 2, m_speed_y < 0 ? -m_speed_y + entity_bounding_rect.height() : entity_bounding_rect.height() / 2, abs(m_speed_x) + entity_bounding_rect.width(), m_speed_y > 0 ? m_speed_y + entity_bounding_rect.height() : entity_bounding_rect.height() / 2)); // QRectF(-250,-250,500,500));
}


void CollisionRect::handleCollision(const QRectF &new_rect, const QRectF &old_rect, qreal speed_x, qreal speed_y)
{
    bool bottom_collision = m_new_rect.bottom() + m_speed_y >= new_rect.top() && m_old_rect.bottom() < old_rect.top();
    bool top_collision = m_new_rect.top() + m_speed_y <= new_rect.bottom() && m_old_rect.top() > old_rect.bottom();
    bool right_collision = m_new_rect.right() + m_speed_x >= new_rect.left() && m_old_rect.right() < old_rect.left();
    bool left_collision = m_new_rect.left() + m_speed_x <= new_rect.right() && m_old_rect.left() > old_rect.right();


    bool vertical_collision = (bottom_collision | top_collision);
    bool horizontal_collision = (right_collision | left_collision);


    if (!vertical_collision & !horizontal_collision)
    {
        return;
    }
    else if (vertical_collision & !horizontal_collision & (m_old_rect.right() >= new_rect.left() & m_old_rect.left() <= new_rect.right()))
    {
        if (bottom_collision)
        {
            m_new_rect.moveBottom(new_rect.top() - M_COLLISION_MARGIN);
        }
        else
        {
            // top_collision
            m_new_rect.moveTop(new_rect.bottom() + M_COLLISION_MARGIN);
        }

        m_speed_y = speed_y * m_speed_y < 0 ? speed_y : 0; // 0
    }
    else if (horizontal_collision & !vertical_collision & (m_old_rect.top() <= new_rect.bottom() & m_old_rect.bottom() >= new_rect.top()))
    {
        if (left_collision)
        {
            m_new_rect.moveLeft(new_rect.right() + M_COLLISION_MARGIN);
        }
        else
        {
            // right_collision
            m_new_rect.moveRight(new_rect.left() - M_COLLISION_MARGIN);
        }

        m_speed_x = speed_x * m_speed_x < 0 ? speed_x : 0; // 0
    }
    else if (vertical_collision & horizontal_collision)
    {
        qDebug() << "Before" << m_new_rect << m_old_rect << m_speed_x << m_speed_y << new_rect << old_rect;

        // y = (dy/dx)*(x-x0) + y0
        // x = (dx/dy)*(y-y0) + x0

        qreal dyx = m_speed_y / m_speed_x; // 4,123633333
        qreal dxy = m_speed_x / m_speed_y;

        if (right_collision)
        {
            qreal y_top_right = dyx * (new_rect.left() - m_new_rect.right()) + m_new_rect.top();

            qreal y_bottom_right = dyx * (new_rect.left() - m_new_rect.right()) + m_new_rect.bottom();

            if ((y_top_right >= new_rect.top() && y_top_right <= new_rect.bottom()) || (y_bottom_right >= new_rect.top() && y_bottom_right <= new_rect.bottom()))
            {
                m_new_rect.moveRight(new_rect.left() - M_COLLISION_MARGIN);
                m_speed_x = speed_x * m_speed_x < 0 ? speed_x : 0; // 0
            }
        }
        else
        {
            // left_collision
            qreal y_top_left = dyx * (new_rect.right() - m_new_rect.left()) + m_new_rect.top();

            qreal y_bottom_left = dyx * (new_rect.right() - m_new_rect.left()) + m_new_rect.bottom();

            if ((y_top_left >= new_rect.top() && y_top_left <= new_rect.bottom()) || (y_bottom_left >= new_rect.top() && y_bottom_left <= new_rect.bottom()))
            {
                m_new_rect.moveLeft(new_rect.right() + M_COLLISION_MARGIN);
                m_speed_x = speed_x * m_speed_x < 0 ? speed_x : 0; // 0
            }
        }

        ///////////////////////////////
        if (bottom_collision)
        {
            qreal x_bottom_right = dxy * (new_rect.top() - m_new_rect.bottom()) + m_new_rect.right();

            qreal x_bottom_left = dxy * (new_rect.top() - m_new_rect.bottom()) + m_new_rect.left();

            if ((x_bottom_right >= new_rect.left() && x_bottom_right <= new_rect.right()) || (x_bottom_left >= new_rect.left() && x_bottom_left <= new_rect.right()))
            {
                m_new_rect.moveBottom(new_rect.top() - M_COLLISION_MARGIN);
                m_speed_y = speed_y * m_speed_y < 0 ? speed_y : 0; // 0
            }
        }
        else
        {
            // top_collision
            qreal x_top_right = dxy * (new_rect.bottom() - m_new_rect.top()) + m_new_rect.right();

            qreal x_top_left = dxy * (new_rect.bottom() - m_new_rect.top()) + m_new_rect.left();

            if ((x_top_right >= new_rect.left() && x_top_right <= new_rect.right()) || (x_top_left >= new_rect.left() && x_top_left <= new_rect.right()))
            {
                m_new_rect.moveTop(new_rect.bottom() + M_COLLISION_MARGIN);
                m_speed_y = speed_y * m_speed_y < 0 ? speed_y : 0; // 0
            }
        }
    }
}

void CollisionRect::handleCollision(QRectF rect)
{
    handleCollision(rect, rect, 0, 0);
}

void CollisionRect::handleCollision(const CollisionRect &other)
{
    handleCollision(other.m_new_rect, other.m_old_rect, other.m_speed_x, other.m_speed_y);
}

void CollisionRect::handleCollision(QVector<QRectF> rects)
{
    std::sort(rects.begin(), rects.end(), [&](QRectF rect1, QRectF rect2)
              { return compareDistance(rect1, rect2, m_old_rect); });

    for (QRectF colliding_rect : rects)
    {
        handleCollision(colliding_rect);
    }
}


void CollisionRect::handleCollision(QVector<const CollisionRect *> colliding_rects)
{
    std::sort(colliding_rects.begin(), colliding_rects.end(), [&](const CollisionRect *item1, const CollisionRect *item2)
              { return compareDistance(item1->m_new_rect, item2->m_new_rect, m_old_rect); });

    for (const CollisionRect *colliding_rect : colliding_rects)
    {
        handleCollision(*colliding_rect);
    }
}



qreal CollisionRect::distance(const QRectF &rect1, const QRectF &rect2) const
{
    QPointF center1 = rect1.center();
    QPointF center2 = rect2.center();
    qreal dx = center1.x() - center2.x();
    qreal dy = center1.y() - center2.y();
    return std::sqrt(dx * dx + dy * dy);
}

bool CollisionRect::compareDistance(const QRectF &rect1, const QRectF &rect2, const QRectF &targetRect) const
{
    qreal distance1 = distance(rect1, targetRect);
    qreal distance2 = distance(rect2, targetRect);
    return distance1 < distance2;
}
