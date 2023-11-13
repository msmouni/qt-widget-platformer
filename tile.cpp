#include "tile.h"

Tile::Tile(QRectF rect)
{
    m_rect=rect;

}

QRectF Tile::boundingRect() const
{
    return m_rect;
}

void Tile::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // TMP
    painter->setBrush(Qt::green);
    painter->drawRect(m_rect);
}
