#include "platform.h"
#include "csv.h"

Platform::Platform(QSizeF tile_size, QString map_csv_path, QString tileset_png_path, QHash<int, TileType> tiles_hash)
{
    setData(0, "Platform");

    m_tile_size = tile_size;

    m_tileset_pixmap = QPixmap(tileset_png_path);

    QVector<QVector<int>> map = loadCSV(map_csv_path);

    m_nb_columns = map.size();
    m_nb_rows = (m_nb_columns > 0) ? map[0].size() : 0;

    for (int idx_x = 0; idx_x < m_nb_columns; idx_x++)
    {
        QVector<Tile *> tile_line;
        for (int idx_y = 0; idx_y < m_nb_rows; idx_y++)
        {
            tile_line.append(new Tile(map[idx_x][idx_y], QRectF(idx_x * m_tile_size.width(), idx_y * m_tile_size.height(), m_tile_size.width(), m_tile_size.height()), tiles_hash.value(map[idx_x][idx_y]), m_tileset_pixmap));

            this->addToGroup(tile_line.last());
        }
        m_tiles.append(tile_line);
    }
}

QRectF Platform::boundingRect() const
{
    return childrenBoundingRect();
}



QRectF Platform::handleCollision(QRectF prev_rect, QRectF new_rect) const
{
    return m_collision_handler.handle(prev_rect, new_rect,getCollidingRects(prev_rect, new_rect));
}

QVector<QRectF> Platform::getCollidingRects(QRectF prev_rect, QRectF new_rect) const
{
    // To rename: moving_x
    bool going_up = new_rect.top() - prev_rect.top() < -M_COLLISION_MARGIN;
    bool going_down = new_rect.bottom() - prev_rect.bottom() > M_COLLISION_MARGIN;

    int left_idx = fmax(0, (fmin(prev_rect.left(), new_rect.left()) + M_COLLISION_MARGIN) / m_tile_size.width());
    int right_idx = fmin(m_nb_columns - 1, (fmax(prev_rect.right(), new_rect.right()) - M_COLLISION_MARGIN) / m_tile_size.width());
    int top_idx = fmax(0, (fmin(prev_rect.top(), new_rect.top()) + M_COLLISION_MARGIN) / m_tile_size.height());
    int bottom_idx = fmin(m_nb_rows - 1, (fmax(prev_rect.bottom(), new_rect.bottom()) - M_COLLISION_MARGIN) / m_tile_size.height());


    QVector<QRectF> colliding_rects;

    for (int ind_x = left_idx; ind_x <= right_idx; ind_x++)
    {
        for (int ind_y = top_idx; ind_y <= bottom_idx; ind_y++)
        {
            int i_x = ind_x;
            int i_y = ind_y;

            qreal top = i_y * m_tile_size.height();
            qreal bottom = (i_y + 1) * m_tile_size.height();

            if (!m_tiles[i_x][i_y]->isEmpty() &&
                (m_tiles[i_x][i_y]->isSolid() ||
                 (m_tiles[i_x][i_y]->checkUp() && going_up && (bottom < prev_rect.top())) ||
                 (m_tiles[i_x][i_y]->checkDown() && going_down && (top > prev_rect.bottom()))))
            {
                colliding_rects.append(QRectF(i_x * m_tile_size.width(),i_y * m_tile_size.height(), m_tile_size.width(),m_tile_size.height()));
            }
        }
    }

    return colliding_rects;
}

int Platform::getNbColumns() const
{
    return m_nb_columns;
}

int Platform::getNbRows() const
{
    return m_nb_rows;
}

QSizeF Platform::getTileSize() const
{
    return m_tile_size;
}

const TileType &Platform::getTileType(int &x, int &y) const
{
    return m_tiles[x][y]->getType();
}

QPointF Platform::getTileBottomCenter(const QPoint &pos) const
{
    return QPointF((pos.x() + 0.5) * m_tile_size.width(), (pos.y() + 1.0) * m_tile_size.height() - M_COLLISION_MARGIN);
}

QRect Platform::getIndexRect(QRectF rect, qreal &dx, qreal &dy) const
{
    int left_idx = fmax(0, fmin(rect.left(), rect.left() + dx) / m_tile_size.width());
    int right_idx = fmin(m_nb_columns - 1, fmax(rect.right(), rect.right() + dx) / m_tile_size.width());
    int top_idx = fmax(0, fmin(rect.top(), rect.top() + dy) / m_tile_size.height());
    int bottom_idx = fmin(m_nb_rows - 1, fmax(rect.bottom(), rect.bottom() + dy) / m_tile_size.height());

    return QRect(QPoint(left_idx, top_idx), QPoint(right_idx, bottom_idx));
}
