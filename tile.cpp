#include "tile.h"

Tile::Tile(int id, QRectF rect, TileType type, QPixmap &tileset_pixmap) : m_tileset_pixmap(tileset_pixmap)
{
    m_id = id;
    m_rect = rect;
    m_type = type;

    int nb_col = m_tileset_pixmap.width() / m_rect.width();
    int y_idx = m_id / nb_col;
    int x_idx = m_id - y_idx * nb_col;
    m_tileset_pos = QPointF(x_idx * m_rect.width(), y_idx * m_rect.height());
}

bool Tile::isEmpty()
{
    return m_type == TileType::Empty;
}

bool Tile::isSolid()
{
    return m_type == TileType::Solid;
}

bool Tile::checkUp()
{
    return m_type != TileType::JumpThrough;
}

bool Tile::checkDown()
{
    return m_type != TileType::FallThrough;
}

QRectF Tile::boundingRect() const
{
    return m_rect;
}

void Tile::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (!isEmpty())
    {
        // TMP
        /*painter->setBrush(Qt::green);
        painter->drawRect(m_rect);*/

        painter->drawTiledPixmap(m_rect, m_tileset_pixmap, m_tileset_pos);
    }
    //    painter->drawTiledPixmap()
}
