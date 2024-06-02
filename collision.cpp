#include "collision.h"
#include <algorithm>
#include "character.h"
#include "tile.h"
#include "bomb.h"

CollisionRect::CollisionRect(QGraphicsItem *parent, qreal &speed_x, qreal &speed_y, qreal &weight) : m_speed_x(speed_x), m_speed_y(speed_y), m_weight(weight)
{
    this->setParentItem(parent);

    m_bounding_rect = parent->sceneBoundingRect();
    m_old_rect = m_bounding_rect;
    m_new_rect = m_bounding_rect;

    m_collision_handeled = false;
    m_is_top_collision = false;
    m_is_bottom_collision = false;
    m_is_left_collision = false;
    m_is_right_collision = false;

    m_weight_x = m_weight;
    m_weight_y = m_weight;

    m_margin = QMarginsF(0, 0, 0, 0);
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
    painter->drawPath(this->shape());

    ////////////////////::
    QPen pen2(Qt::red);
    pen.setWidth(2);
    painter->setPen(pen2);
    painter->drawRect(this->mapRectFromScene(m_new_rect));
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
    m_new_rect = new_rect; //.marginsAdded(m_margin);
}

QRectF CollisionRect::getEntityRect()
{
    return m_new_rect;
}

QPointF CollisionRect::getEntityPos()
{
    //    return m_new_rect.marginsRemoved(m_margin).topLeft() + QPointF(m_speed_x, m_speed_y);
    qDebug() << m_speed_x << m_speed_y;
    return m_new_rect.topLeft() + QPointF(m_speed_x, m_speed_y);
}

void CollisionRect::update()
{
    m_collision_handeled = false;
    m_is_top_collision = false;
    m_is_bottom_collision = false;
    m_is_left_collision = false;
    m_is_right_collision = false;

    m_weight_x = m_weight;
    m_weight_y = m_weight;

    QRectF entity_bounding_rect = this->parentItem()->boundingRect();

    setEntityRect(this->mapRectToScene(entity_bounding_rect));

    m_bounding_rect = entity_bounding_rect.marginsAdded(QMarginsF(entity_bounding_rect.width() / 2, m_speed_y < 0 ? -m_speed_y + entity_bounding_rect.height() : entity_bounding_rect.height() / 2, abs(m_speed_x) + entity_bounding_rect.width(), m_speed_y > 0 ? m_speed_y + entity_bounding_rect.height() : entity_bounding_rect.height() / 2)); // QRectF(-250,-250,500,500));
}

void CollisionRect::setMargin(QMarginsF margin)
{
    //    setEntityRect(this->mapRectToScene(this->parentItem()->boundingRect()).marginsAdded(margin));
    m_margin = margin;
}

bool CollisionRect::isBottomCollision()
{
    return m_is_bottom_collision;
}
/*
NOTES:
seperate <collision detection> and <collision resolving>

-> for item in items{
    // Static vs dynamic
    QVector<CollisionRect &> item->detect_collision();
}

///////////////// DYNAMIC ?
///////////////// GET A CENTRALIZED COLLIDING ITEMS vector: QVector<(item1, item2, top/bottom/none, left/right/none)>
///////////////// THAN RESOLVE COLLISION on that vector
*/
void CollisionRect::handleCollision(QRectF &new_rect, const QRectF &old_rect, qreal &speed_x, qreal &speed_y, qreal &weight_x, qreal &weight_y)
{
    // NOTE: it's the light one who should handle collision with the heavier one
    //    if ((int)weight>=(int)m_weight){
    /*bool bottom_collision = weight_y >= m_weight_y && m_new_rect.bottom() + m_speed_y >= new_rect.top() + speed_y && m_old_rect.bottom() < old_rect.top();
    bool top_collision = weight_y >= m_weight_y && m_new_rect.top() + m_speed_y <= new_rect.bottom() + speed_y && m_old_rect.top() > old_rect.bottom();
    bool right_collision = weight_x >= m_weight_x && m_new_rect.right() + m_speed_x >= new_rect.left() + speed_x && m_old_rect.right() < old_rect.left();
    bool left_collision = weight_x >= m_weight_x && m_new_rect.left() + m_speed_x <= new_rect.right() + speed_x && m_old_rect.left() > old_rect.right();*/

    // TO FACTORIZE LATER IF OK
    bool bottom_collision = m_new_rect.bottom() + m_speed_y >= new_rect.top() + speed_y && m_old_rect.bottom() < old_rect.top();
    bool top_collision = m_new_rect.top() + m_speed_y <= new_rect.bottom() + speed_y && m_old_rect.top() > old_rect.bottom();
    bool right_collision = m_new_rect.right() + m_speed_x >= new_rect.left() + speed_x && m_old_rect.right() < old_rect.left();
    bool left_collision = m_new_rect.left() + m_speed_x <= new_rect.right() + speed_x && m_old_rect.left() > old_rect.right();

    bool vertical_collision = (bottom_collision | top_collision);
    bool horizontal_collision = (right_collision | left_collision);

    /*
     * Problem to resolve:

    if (weight_y >= m_weight_y)
    {
        other haven't yet handled its collision (m_speed=speed ...)
    }
    else
    {
        after handling other's collision, its weight must become greater
    }
    */

    if (!vertical_collision & !horizontal_collision)
    {
        return;
    }
    else if (vertical_collision & !horizontal_collision & (m_old_rect.right() >= new_rect.left() & m_old_rect.left() <= new_rect.right()))
    {
        if (bottom_collision)
        {
            if (weight_y >= m_weight_y)
            {
                m_new_rect.moveBottom(new_rect.top() - M_COLLISION_MARGIN);
                m_speed_y = speed_y;
                m_weight_y = weight_y;
            }
            else
            {
                new_rect.moveBottom(m_new_rect.top() - M_COLLISION_MARGIN);
                speed_y = m_speed_y;
                weight_y = m_weight_y;
            }
            m_is_bottom_collision = true;
        }
        else
        {
            // top_collision
            if (weight_y >= m_weight_y)
            {
                m_new_rect.moveTop(new_rect.bottom() + M_COLLISION_MARGIN);
                m_speed_y = speed_y;
                m_weight_y = weight_y;
            }
            else
            {
                new_rect.moveTop(m_new_rect.bottom() + M_COLLISION_MARGIN);
                speed_y = m_speed_y;
                weight_y = m_weight_y;
            }
            m_is_top_collision = true;
        }

        // NOTE: m_speed_y = (m_speed_y*m_weight + speed_y*weight)/(m_weight+weight);
        // m_speed_y = speed_y; // * m_speed_y < 0 ? speed_y : 0; // 0//0; //
        // m_weight_y = weight_y;
    }
    else if (horizontal_collision & !vertical_collision & (m_old_rect.top() <= new_rect.bottom() & m_old_rect.bottom() >= new_rect.top()))
    {
        if (left_collision)
        {
            if (weight_x >= m_weight_x)
            {
                m_new_rect.moveLeft(new_rect.right() + M_COLLISION_MARGIN);
                m_speed_x = speed_x;
                m_weight_x = weight_x;
            }
            else
            {
                new_rect.moveLeft(m_new_rect.right() + M_COLLISION_MARGIN);
                speed_x = m_speed_x;
                weight_x = m_weight_x;
            }
            m_is_left_collision = true;
        }
        else
        {
            // right_collision
            if (weight_x >= m_weight_x)
            {
                m_new_rect.moveRight(new_rect.left() - M_COLLISION_MARGIN);
                m_speed_x = speed_x;
                m_weight_x = weight_x;
            }
            else
            {
                new_rect.moveRight(m_new_rect.left() - M_COLLISION_MARGIN);
                speed_x = m_speed_x;
                weight_x = m_weight_x;
            }
            m_is_right_collision = true;
        }

        // m_speed_x = speed_x; // * m_speed_x < 0 ? speed_x : 0; // 0//0;//
        // m_weight_x = weight_x;
    }
    else if (vertical_collision & horizontal_collision)
    {
        // y = (dy/dx)*(x-x0) + y0
        // x = (dx/dy)*(y-y0) + x0

        qreal dyx = m_speed_y / m_speed_x;
        qreal dxy = m_speed_x / m_speed_y;

        if (right_collision)
        {
            qreal y_top_right = dyx * (new_rect.left() - m_new_rect.right()) + m_new_rect.top();

            qreal y_bottom_right = dyx * (new_rect.left() - m_new_rect.right()) + m_new_rect.bottom();

            if ((y_top_right >= new_rect.top() && y_top_right <= new_rect.bottom()) || (y_bottom_right >= new_rect.top() && y_bottom_right <= new_rect.bottom()))
            {
                if (weight_x >= m_weight_x)
                {
                    m_new_rect.moveRight(new_rect.left() - M_COLLISION_MARGIN);
                    m_speed_x = speed_x;
                    m_weight_x = weight_x;
                }
                else
                {
                    new_rect.moveRight(m_new_rect.left() - M_COLLISION_MARGIN);
                    speed_x = m_speed_x;
                    weight_x = m_weight_x;
                }
                // m_speed_x = speed_x; // * m_speed_x < 0 ? speed_x : 0; // 0//0; //
                // m_weight_x = weight_x;

                m_is_right_collision = true;
            }
        }
        else
        {
            // left_collision
            qreal y_top_left = dyx * (new_rect.right() - m_new_rect.left()) + m_new_rect.top();

            qreal y_bottom_left = dyx * (new_rect.right() - m_new_rect.left()) + m_new_rect.bottom();

            if ((y_top_left >= new_rect.top() && y_top_left <= new_rect.bottom()) || (y_bottom_left >= new_rect.top() && y_bottom_left <= new_rect.bottom()))
            {
                if (weight_x >= m_weight_x)
                {
                    m_new_rect.moveLeft(new_rect.right() + M_COLLISION_MARGIN);
                    m_speed_x = speed_x;
                    m_weight_x = weight_x;
                }
                else
                {
                    new_rect.moveLeft(m_new_rect.right() + M_COLLISION_MARGIN);
                    speed_x = m_speed_x;
                    weight_x = m_weight_x;
                }
                // m_speed_x = speed_x; // * m_speed_x < 0 ? speed_x : 0; // 0//0; //
                // m_weight_x = weight_x;

                m_is_left_collision = true;
            }
        }

        ///////////////////////////////
        if (bottom_collision)
        {
            qreal x_bottom_right = dxy * (new_rect.top() - m_new_rect.bottom()) + m_new_rect.right();

            qreal x_bottom_left = dxy * (new_rect.top() - m_new_rect.bottom()) + m_new_rect.left();

            if ((x_bottom_right >= new_rect.left() && x_bottom_right <= new_rect.right()) || (x_bottom_left >= new_rect.left() && x_bottom_left <= new_rect.right()))
            {
                if (weight_y >= m_weight_y)
                {
                    m_new_rect.moveBottom(new_rect.top() - M_COLLISION_MARGIN);
                    m_speed_y = speed_y;
                    m_weight_y = weight_y;
                }
                else
                {
                    new_rect.moveBottom(m_new_rect.top() - M_COLLISION_MARGIN);
                    speed_y = m_speed_y;
                    weight_y = m_weight_y;
                }
                // m_speed_y = speed_y; // * m_speed_y < 0 ? speed_y : 0; // 0//0; //
                // m_weight_y = weight_y;

                m_is_bottom_collision = true;
            }
        }
        else
        {
            // top_collision
            qreal x_top_right = dxy * (new_rect.bottom() - m_new_rect.top()) + m_new_rect.right();

            qreal x_top_left = dxy * (new_rect.bottom() - m_new_rect.top()) + m_new_rect.left();

            if ((x_top_right >= new_rect.left() && x_top_right <= new_rect.right()) || (x_top_left >= new_rect.left() && x_top_left <= new_rect.right()))
            {
                if (weight_y >= m_weight_y)
                {
                    m_new_rect.moveTop(new_rect.bottom() + M_COLLISION_MARGIN);
                    m_speed_y = speed_y;
                    m_weight_y = weight_y;
                }
                else
                {
                    new_rect.moveTop(m_new_rect.bottom() + M_COLLISION_MARGIN);
                    speed_y = m_speed_y;
                    weight_y = m_weight_y;
                }
                // m_speed_y = speed_y; // * m_speed_y < 0 ? speed_y : 0; // 0// 0; //
                // m_weight_y = weight_y;

                m_is_top_collision = true;
            }
        }
    }
    //    }
}

void CollisionRect::handleStaticCollision(QRectF rect)
{
    // TMP use bool static as argument ...
    qreal speed_x = 0;
    qreal speed_y = 0;
    qreal weight_x = 9999;
    qreal weight_y = 9999;
    handleCollision(rect, rect, speed_x, speed_y, weight_x, weight_y);
    //    handleCollision(rect, rect, 0, 0, 9999, 9999);
}

void CollisionRect::handleDynamicCollision(CollisionRect &other)
{
    handleCollision(other.m_new_rect, other.m_old_rect, other.m_speed_x, other.m_speed_y, other.m_weight_x, other.m_weight_y);
}

void CollisionRect::handleStaticCollision(QVector<QRectF> rects)
{
    std::sort(rects.begin(), rects.end(), [&](QRectF rect1, QRectF rect2)
              { return compareDistance(rect1, rect2, m_old_rect); });

    for (QRectF colliding_rect : rects)
    {
        handleStaticCollision(colliding_rect);
    }
}

void CollisionRect::handleDynamicCollision(QVector<CollisionRect *> colliding_rects)
{
    std::sort(colliding_rects.begin(), colliding_rects.end(), [&](CollisionRect *item1, CollisionRect *item2)
              { return compareDistance(item1->m_new_rect, item2->m_new_rect, m_old_rect); });

    for (CollisionRect *colliding_rect : colliding_rects)
    {
        // TODO: colliding_rect->handleCollision() // not const *
        //         colliding_rect->handleCollision(this);
        handleDynamicCollision(*colliding_rect);
    }
}

void CollisionRect::handleCollision(CollisionRect *caller)
{
    if (!m_collision_handeled)
    {
        m_collision_handeled = true; // Lock

        QVector<CollisionRect *> dyn_collision_rects;
        QVector<QRectF> static_collision_rects;

        for (QGraphicsItem *item : collidingItems())
        {
            if (item != this->parentItem()) // && (caller==nullptr))//  || item !=caller->parentItem()) )
            {
                if (item->data(0) == "Enemy" || item->data(0) == "Player")
                {
                    Character *chara = static_cast<Character *>(item);
                    // don't add if : (chara Enemy & chara isAttacking & this not Enemy) || (this Enemy & this isAttacking & chara not Enemy)
                    //                if (!(chara->data(0) == "Enemy" && chara->isAttacking() && this->data(0) != "Enemy") && !(chara->data(0) == "Enemy" && m isAttacking() && chara->data(0) != "Enemy")){
                    //                if (!chara->isAttacking() || this->data(0) != "Enemy"){
                    // TODO: if parentItem is Character & Attacking -> only handle collision with Tiles
                    dyn_collision_rects.append(chara->getCollisionRect());

                    //                }
                }
                else if (item->data(1) == "Bomb")
                {
                    Bomb *bomb = static_cast<Bomb *>(item);
                    if (!bomb->isActive())
                    {
                        dyn_collision_rects.append(bomb->getCollisionRect());
                    }
                }
                else if (item->data(0) == "Tile")
                {
                    Tile *tile = static_cast<Tile *>(item);

                    QRectF tile_bnd_rect = tile->sceneBoundingRect();

                    if (tile->isSolid() | (!tile->isEmpty() & ((tile->checkUp() && m_speed_y < 0 && sceneBoundingRect().bottom() >= tile_bnd_rect.bottom()) | (tile->checkDown() && m_speed_y > 0 && sceneBoundingRect().top() <= tile_bnd_rect.top()))))
                    {
                        static_collision_rects.append(tile_bnd_rect);
                    }
                }
            }
        }

        handleStaticCollision(static_collision_rects);
        handleDynamicCollision(dyn_collision_rects);
        // handleStaticCollision(static_collision_rects);
        m_collision_handeled = false;
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
