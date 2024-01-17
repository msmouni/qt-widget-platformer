#include "platform.h"
#include "csv.h"

Platform::Platform(QSizeF tile_size, QString map_csv_path, QString tileset_png_path, QHash<int, TileType> tiles_hash)
{
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

// To factorize later
QRectF Platform::handleCollision(QRectF rect, qreal &dx, qreal &dy) const
{
    int left_idx = fmax(0, fmin(rect.left(), rect.left() + dx) / m_tile_size.width());
    int right_idx = fmin(m_nb_columns - 1, fmax(rect.right(), rect.right() + dx) / m_tile_size.width());
    int top_idx = fmax(0, fmin(rect.top(), rect.top() + dy) / m_tile_size.height());
    int bottom_idx = fmin(m_nb_rows - 1, fmax(rect.bottom(), rect.bottom() + dy) / m_tile_size.height());

    bool going_down = dy > 0;
    bool going_right = dx > 0;
    bool moving_hor = abs(dx) > 1e-7;
    bool moving_ver = abs(dy) > 1e-7;

    for (int ind_x = left_idx; ind_x <= right_idx; ind_x++)
    {
        for (int ind_y = top_idx; ind_y <= bottom_idx; ind_y++)
        {
            int i_x;
            if (!going_right)
            {
                i_x = (right_idx + left_idx) - ind_x;
            }
            else
            {
                i_x = ind_x;
            }

            int i_y;
            if (!going_down)
            {
                i_y = (bottom_idx + top_idx) - ind_y;
            }
            else
            {
                i_y = ind_y;
            }

            qreal left = i_x * m_tile_size.width();
            qreal right = (i_x + 1) * m_tile_size.width();

            qreal top = i_y * m_tile_size.height();
            qreal bottom = (i_y + 1) * m_tile_size.height();

            if (!m_tiles[i_x][i_y]->isEmpty() &&
                (m_tiles[i_x][i_y]->isSolid() ||
                 (m_tiles[i_x][i_y]->checkUp() && !going_down && (bottom < rect.top())) ||
                 (m_tiles[i_x][i_y]->checkDown() && going_down && (top > rect.bottom()))))
            {

                if (moving_ver && !moving_hor)
                {
                    // dx=0
                    if (!going_down && rect.top() + dy < bottom)
                    {
                        dy = bottom - rect.top() + 0.001;
                    }
                    else if (going_down && rect.bottom() + dy > top)
                    {
                        dy = top - rect.bottom() - 0.001;
                    }
                }
                else if (moving_hor && !moving_ver)
                {
                    // dy=0
                    if (!going_right && rect.left() + dx < right)
                    {
                        dx = right - rect.left() + 0.001;
                    }
                    else if (going_right && rect.right() + dx > left)
                    {
                        dx = left - rect.right() - 0.001;
                    }
                }
                else if (moving_hor && moving_ver)
                {
                    // y = (dy/dx)*(x-x0) + y0
                    // x = (dx/dy)*(y-y0) + x0

                    qreal dyx = dy / dx;
                    qreal dxy = dx / dy;

                    if (going_right)
                    {
                        qreal y_top_right = dyx * (left - rect.right()) + rect.top();

                        qreal y_bottom_right = dyx * (left - rect.right()) + rect.bottom();

                        if ((y_top_right >= top && y_top_right <= bottom) || (y_bottom_right >= top && y_bottom_right <= bottom))
                        {
                            dx = fmin(dx, left - rect.right() - 0.001);
                        }
                    }
                    else
                    {
                        // Left
                        qreal y_top_left = dyx * (right - rect.left()) + rect.top();

                        qreal y_bottom_left = dyx * (right - rect.left()) + rect.bottom();

                        if ((y_top_left >= top && y_top_left <= bottom) || (y_bottom_left >= top && y_bottom_left <= bottom))
                        {
                            dx = fmax(dx, right - rect.left() + 0.001);
                        }
                    }

                    ///////////////////////////////
                    if (going_down)
                    {
                        qreal x_bottom_right = dxy * (top - rect.bottom()) + rect.right();

                        qreal x_bottom_left = dxy * (top - rect.bottom()) + rect.left();

                        if ((x_bottom_right >= left && x_bottom_right <= right) || (x_bottom_left >= left && x_bottom_left <= right))
                        {
                            dy = fmin(dy, top - rect.bottom() - 0.001);
                        }
                    }
                    else
                    {
                        // Up
                        qreal x_top_right = dxy * (bottom - rect.top()) + rect.right();

                        qreal x_top_left = dxy * (bottom - rect.top()) + rect.left();

                        if ((x_top_right >= left && x_top_right <= right) || (x_top_left >= left && x_top_left <= right))
                        {
                            dy = fmax(dy, bottom - rect.top() + 0.001);
                        }
                    }
                }
                else
                {
                    // (!moving_ver && !moving_hor)
                    dy = 0;
                    dx = 0;
                }
            }
        }
    }
    return rect.translated(dx, dy);
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

QRect Platform::getIndexRect(QRectF rect, qreal &dx, qreal &dy) const
{
    int left_idx = fmax(0, fmin(rect.left(), rect.left() + dx) / m_tile_size.width());
    int right_idx = fmin(m_nb_columns - 1, fmax(rect.right(), rect.right() + dx) / m_tile_size.width());
    int top_idx = fmax(0, fmin(rect.top(), rect.top() + dy) / m_tile_size.height());
    int bottom_idx = fmin(m_nb_rows - 1, fmax(rect.bottom(), rect.bottom() + dy) / m_tile_size.height());

    return QRect(QPoint(left_idx, top_idx), QPoint(right_idx, bottom_idx));
}
