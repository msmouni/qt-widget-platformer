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

const TileType &Tile::getType()
{
    return m_type;
}

QRectF Tile::boundingRect() const
{
    return m_rect;
}

void Tile::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (!isEmpty())
    {
        painter->drawTiledPixmap(m_rect, m_tileset_pixmap, m_tileset_pos);
    }

    // Set the pen and brush for the rectangle
    QPen pen(Qt::black);
    pen.setWidth(2);
    painter->setPen(pen);
    painter->setFont(QFont("Arial", 5));

    //    QString txt = "(" + QString::number(sceneBoundingRect().center().x()) + "," + QString::number(sceneBoundingRect().center().y()) + ")";
    QString txt = "(" + QString::number((int)(sceneBoundingRect().center().x() / m_rect.width())) + "," + QString::number((int)(sceneBoundingRect().center().y() / m_rect.height())) + ")";
    painter->drawText(sceneBoundingRect().center(), txt);
}
