#include "platform.h"
#include "csv.h"

Platform::Platform(QSizeF tile_size, QString map_csv_path)
{
    m_tile_size = tile_size;

    m_map = loadCSV(map_csv_path);

    if (!m_map.isEmpty())
    {
        m_nb_rows = m_map.size();
        m_nb_columns = m_map[0].size();
    }
    else
    {
        m_nb_rows = 0;
        m_nb_columns = 0;
    }

    for (int i = 0; i < m_nb_rows; i++)
    {
        for (int j = 0; j < m_nb_columns; j++)
        {
            if (m_map[i][j] >= 0)
            {
                m_tiles.append(new Tile(QRectF(j * m_tile_size.width(), i * m_tile_size.height(), m_tile_size.width(), m_tile_size.height())));

                this->addToGroup(m_tiles.last());
            }
        }
    }
}

QRectF Platform::boundingRect() const
{
    return childrenBoundingRect();
}

void Platform::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // TMP
    QPen pen(Qt::black);
    pen.setWidth(2);
    painter->setPen(pen);
    painter->setBrush(Qt::green);
    painter->setFont(QFont("Arial", 10));

    for (Tile *tile : m_tiles)
    {
        tile->paint(painter, option, widget);
    }
}

QRectF Platform::handleCollision(QRectF rect, qreal &dx, qreal &dy) const
{
    return rect.translated(dx, dy);
}

void Platform::update()
{
}
