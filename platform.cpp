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

// To factorize later
QRectF Platform::handleCollision(QRectF rect, qreal &dx, qreal &dy) const
{
    bool moving_ver = false;
    bool moving_hor = false;
    bool going_down = false;
    bool going_right = false;

    if (dx > M_COLLISION_MARGIN)
    {
        going_right = true;
        moving_hor = true;
    }
    else if (dx < -M_COLLISION_MARGIN)
    {
        moving_hor = true;
    }
    else
    {
        dx = 0;
    }

    if (dy > M_COLLISION_MARGIN)
    {
        going_down = true;
        moving_ver = true;
    }
    else if (dy < -M_COLLISION_MARGIN)
    {
        moving_ver = true;
    }
    else
    {
        dy = 0;
    }

    int left_idx = fmax(0, (fmin(rect.left(), rect.left() + dx) + M_COLLISION_MARGIN) / m_tile_size.width());
    int right_idx = fmin(m_nb_columns - 1, (fmax(rect.right(), rect.right() + dx) - M_COLLISION_MARGIN) / m_tile_size.width());
    int top_idx = fmax(0, (fmin(rect.top(), rect.top() + dy) + M_COLLISION_MARGIN) / m_tile_size.height());
    int bottom_idx = fmin(m_nb_rows - 1, (fmax(rect.bottom(), rect.bottom() + dy) - M_COLLISION_MARGIN) / m_tile_size.height());

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
                        dy = bottom - rect.top() + M_COLLISION_MARGIN;
                    }
                    else if (going_down && rect.bottom() + dy > top)
                    {
                        dy = top - rect.bottom() - M_COLLISION_MARGIN;
                    }
                }
                else if (moving_hor && !moving_ver)
                {
                    // dy=0
                    if (!going_right && rect.left() + dx < right)
                    {
                        dx = right - rect.left() + M_COLLISION_MARGIN;
                    }
                    else if (going_right && rect.right() + dx > left)
                    {
                        dx = left - rect.right() - M_COLLISION_MARGIN;
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
                            dx = fmin(dx, left - rect.right() - M_COLLISION_MARGIN);
                        }
                    }
                    else
                    {
                        // Left
                        qreal y_top_left = dyx * (right - rect.left()) + rect.top();

                        qreal y_bottom_left = dyx * (right - rect.left()) + rect.bottom();

                        if ((y_top_left >= top && y_top_left <= bottom) || (y_bottom_left >= top && y_bottom_left <= bottom))
                        {
                            dx = fmax(dx, right - rect.left() + M_COLLISION_MARGIN);
                        }
                    }

                    ///////////////////////////////
                    if (going_down)
                    {
                        qreal x_bottom_right = dxy * (top - rect.bottom()) + rect.right();

                        qreal x_bottom_left = dxy * (top - rect.bottom()) + rect.left();

                        if ((x_bottom_right >= left && x_bottom_right <= right) || (x_bottom_left >= left && x_bottom_left <= right))
                        {
                            dy = fmin(dy, top - rect.bottom() - M_COLLISION_MARGIN);
                        }
                    }
                    else
                    {
                        // Up
                        qreal x_top_right = dxy * (bottom - rect.top()) + rect.right();

                        qreal x_top_left = dxy * (bottom - rect.top()) + rect.left();

                        if ((x_top_right >= left && x_top_right <= right) || (x_top_left >= left && x_top_left <= right))
                        {
                            dy = fmax(dy, bottom - rect.top() + M_COLLISION_MARGIN);
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

QRectF Platform::handleCollision(QRectF prev_rect, QRectF new_rect) const
{
    // TMP
//    return new_rect;
    // To complete: QRectF &new_rect -> translate to avoid collision
    qreal dx_r=new_rect.right()-prev_rect.right();
    qreal dx_l=new_rect.left()-prev_rect.left();
    qreal dy_t=new_rect.top()-prev_rect.top();
    qreal dy_b=new_rect.bottom()-prev_rect.bottom();

    // To rename: moving_x
    bool going_up = false;
    bool going_down = false;
    bool going_right = false;
    bool going_left = false;

    if (dx_r > M_COLLISION_MARGIN)
    {
        going_right = true;
    }else {
        dx_r=(dx_l < -M_COLLISION_MARGIN)? dx_l:0;//fmin(0,dx_l);
    }

    if (dx_l < -M_COLLISION_MARGIN)
    {
        going_left = true;
    }
    else
    {
        dx_l =(dx_r > M_COLLISION_MARGIN)? dx_r:0 ;//fmax(0, dx_r);
    }

    if (dy_b > M_COLLISION_MARGIN)
    {
        going_down = true;
    }
    else {
//        qDebug()<<"b:"<<dy_b<<dy_t;
        dy_b= (dy_t < -M_COLLISION_MARGIN)? dy_t:0;//fmin(0, dy_t);
    }

    if (dy_t < -M_COLLISION_MARGIN)
    {
        going_up = true;
    }
    else {
//        qDebug()<<"t:"<<dy_b<<dy_t;
        dy_t= (dy_b > M_COLLISION_MARGIN)? dy_t:0 ;//fmax(0, dy_b);
    }

    bool moving_hor=going_left | going_right;
    bool moving_ver=going_up | going_down;

    int left_idx = fmax(0, (fmin(prev_rect.left(), new_rect.left()) + M_COLLISION_MARGIN) / m_tile_size.width());
    int right_idx = fmin(m_nb_columns - 1, (fmax(prev_rect.right(), new_rect.right()) - M_COLLISION_MARGIN) / m_tile_size.width());
    int top_idx = fmax(0, (fmin(prev_rect.top(), new_rect.top()) + M_COLLISION_MARGIN) / m_tile_size.height());
    int bottom_idx = fmin(m_nb_rows - 1, (fmax(prev_rect.bottom(), new_rect.bottom()) - M_COLLISION_MARGIN) / m_tile_size.height());

//    qDebug()<<"prev_rect"<<prev_rect<<"new_rect"<<new_rect<<"left_idx"<<left_idx<<"right_idx"<<right_idx<<"top_idx"<<top_idx<<"bottom_idx"<<bottom_idx;
    for (int ind_x = left_idx; ind_x <= right_idx; ind_x++)
    {
        for (int ind_y = top_idx; ind_y <= bottom_idx; ind_y++)
        {
//            qDebug()<<"Check1";
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
                 (m_tiles[i_x][i_y]->checkUp() && going_up && (bottom < prev_rect.top())) ||
                 (m_tiles[i_x][i_y]->checkDown() && going_down && (top > prev_rect.bottom()))))
            {
//                qDebug()<<"Check";

                if (moving_ver && !moving_hor)
                {
//                    qDebug()<<"Check1";
//                    qDebug()<<"Ver"<<going_down<<prev_rect.bottom() + dy_b<<top;
                    // dx=0
                    if (going_up && prev_rect.top() + dy_t < bottom)
                    {
                        qreal new_dy_t=bottom - prev_rect.top() + M_COLLISION_MARGIN;
                        dy_b+=dy_t-new_dy_t;
                        dy_t = new_dy_t;
                    }

                    if (going_down && prev_rect.bottom() + dy_b > top)
                    {
                        qreal new_dy_b=top - prev_rect.bottom() - M_COLLISION_MARGIN;
                        dy_t-=dy_b -new_dy_b;
                        dy_b = new_dy_b;
//                        qDebug()<<"Down"<<dy_b;
                    }
                }
                else if (moving_hor && !moving_ver)
                {
//                    qDebug()<<"Check2";
                    // dy=0
                    if (going_left && prev_rect.left() + dx_l < right)
                    {
                        qreal new_dx_l=right - prev_rect.left() + M_COLLISION_MARGIN;
                        dx_r+=dx_l-new_dx_l;
                        dx_l = new_dx_l;
                    }
                    else if (going_right && prev_rect.right() + dx_r > left)
                    {
                        qreal new_dx_r=left - prev_rect.right() - M_COLLISION_MARGIN;
                        dx_l-=dx_r-new_dx_r;
                        dx_r = new_dx_r;
                    }
                }
                else if (moving_hor && moving_ver)
                {
//                    qDebug()<<"Check3";
                    // y = (dy/dx)*(x-x0) + y0
                    // x = (dx/dy)*(y-y0) + x0

                    /*qreal dyx = dy / dx;
                    qreal dxy = dx / dy;*/

                    if (going_right)
                    {
//                        qDebug()<<"Check3-1";
                        qreal y_top_right = dy_t / dx_r * (left - prev_rect.right()) + prev_rect.top();

                        qreal y_bottom_right = dy_b / dx_r * (left - prev_rect.right()) + prev_rect.bottom();

                        if ((y_top_right >= top && y_top_right <= bottom) || (y_bottom_right >= top && y_bottom_right <= bottom))
                        {
                            qreal new_dx_r=fmin(dx_r, left - prev_rect.right() - M_COLLISION_MARGIN);
                            dx_l-=dx_r-new_dx_r;
                            dx_r = new_dx_r;
                        }
                    }

                    if (going_left)
                    {
//                        qDebug()<<"Check3-2";
                        // Left
                        qreal y_top_left = dy_t / dx_l * (right - prev_rect.left()) + prev_rect.top();

                        qreal y_bottom_left = dy_b / dx_l * (right - prev_rect.left()) + prev_rect.bottom();

                        if ((y_top_left >= top && y_top_left <= bottom) || (y_bottom_left >= top && y_bottom_left <= bottom))
                        {
                            qreal new_dx_l=fmax(dx_l, right - prev_rect.left() + M_COLLISION_MARGIN);
                            dx_r+=dx_l-new_dx_l;
                            dx_l = new_dx_l;
                        }
                    }

                    ///////////////////////////////
                    if (going_down)
                    {
//                        qDebug()<<"Check3-3";
                        qreal x_bottom_right = dx_r/ dy_b * (top - prev_rect.bottom()) + prev_rect.right();

                        qreal x_bottom_left = dx_l/dy_b * (top - prev_rect.bottom()) + prev_rect.left();

                        if ((x_bottom_right >= left && x_bottom_right <= right) || (x_bottom_left >= left && x_bottom_left <= right))
                        {
                            qreal new_dy_b=fmin(dy_b, top - prev_rect.bottom() - M_COLLISION_MARGIN);
                            dy_t-=dy_b-new_dy_b;
                            dy_b = new_dy_b;
                        }
                    }
                    else
                    {
//                        qDebug()<<"Check3-4";
                        // Up
                        qreal x_top_right = dx_r/ dy_t * (bottom - prev_rect.top()) + prev_rect.right();

                        qreal x_top_left = dx_l/dy_t * (bottom - prev_rect.top()) + prev_rect.left();

                        if ((x_top_right >= left && x_top_right <= right) || (x_top_left >= left && x_top_left <= right))
                        {
                            qreal new_dy_t=fmax(dy_t, bottom - prev_rect.top() + M_COLLISION_MARGIN);
                            dy_b+=dy_t-new_dy_t;
                            dy_t =new_dy_t;
                        }
                    }
                }
                else
                {
//                    qDebug()<<"Check4";
                    // (!moving_ver && !moving_hor)
                    dy_b = 0;
                    dy_t = 0;
                    dx_l = 0;
                    dx_r = 0;
                }
            }
        }
    }

    QRectF res=QRectF(prev_rect.topLeft() + QPointF(dx_l,dy_t), prev_rect.bottomRight() + QPointF(dx_r,dy_b));

//    qDebug()<<prev_rect<<new_rect<<res;
    return res;

//    return rect.translated(dx, dy);
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
