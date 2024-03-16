#include "collision.h"
#include <algorithm>
#include <QDebug>

Collision::Collision()
{
}

QRectF Collision::handle(QRectF prev_rect, QRectF new_rect, QVector<QRectF> colliding_rects) const
{
    std::sort(colliding_rects.begin(), colliding_rects.end(), [&](QRectF &rect1, QRectF &rect2)
              { return compareDistance(rect1, rect2, prev_rect); });

    qreal dx_r = new_rect.right() - prev_rect.right();
    qreal dx_l = new_rect.left() - prev_rect.left();
    qreal dy_t = new_rect.top() - prev_rect.top();
    qreal dy_b = new_rect.bottom() - prev_rect.bottom();

    bool moving_up = false;
    bool moving_down = false;
    bool moving_right = false;
    bool moving_left = false;

    for (const QRectF &colliding_rect : colliding_rects)
    {
        //        qDebug()<<dx_l<<dx_r<<dy_t<<dy_b; // Must Update new_rect
        if (dx_r > M_COLLISION_MARGIN)
        {
            moving_right = true;
        }
        else
        {
            dx_r = (dx_r < -M_COLLISION_MARGIN) ? dx_r : 0;
            moving_right = false;
        }

        if (dx_l < -M_COLLISION_MARGIN)
        {
            moving_left = true;
        }
        else
        {
            dx_l = (dx_l > M_COLLISION_MARGIN) ? dx_l : 0;
            moving_left = false;
        }

        if (dy_b > M_COLLISION_MARGIN)
        {
            moving_down = true;
        }
        else
        {
            dy_b = (dy_b < -M_COLLISION_MARGIN) ? dy_b : 0;
            moving_down = false;
        }

        if (dy_t < -M_COLLISION_MARGIN)
        {
            moving_up = true;
        }
        else
        {
            dy_t = (dy_t > M_COLLISION_MARGIN) ? dy_t : 0;
            moving_up = false;
        }

        bool moving_hor = moving_left | moving_right;
        bool moving_ver = moving_up | moving_down;

        //    for (const QRectF &colliding_rect : colliding_rects)
        //    {
        qreal top = colliding_rect.top();
        qreal bottom = colliding_rect.bottom();
        qreal left = colliding_rect.left();
        qreal right = colliding_rect.right();

        if (moving_ver && !moving_hor && (prev_rect.right() >= left && prev_rect.left() <= right))
        {
            // dx=0
            if (moving_up && prev_rect.top() + dy_t < bottom && prev_rect.top() > bottom)
            {
                qreal new_dy_t = bottom - prev_rect.top() + M_COLLISION_MARGIN;
                dy_b += dy_t - new_dy_t;
                dy_t = new_dy_t;
                //                moving_ver=false;
            }

            if (moving_down && prev_rect.bottom() + dy_b > top && prev_rect.bottom() < top)
            {
                qreal new_dy_b = top - prev_rect.bottom() - M_COLLISION_MARGIN;
                dy_t -= dy_b - new_dy_b;
                dy_b = new_dy_b;
                //                moving_ver=false;
            }
        }
        else if (moving_hor && !moving_ver && (prev_rect.top() <= bottom && prev_rect.bottom() >= top))
        {
            // dy=0
            if (moving_left && prev_rect.left() + dx_l < right && prev_rect.left() > right)
            {
                qreal new_dx_l = right - prev_rect.left() + M_COLLISION_MARGIN;
                dx_r += dx_l - new_dx_l;
                dx_l = new_dx_l;
                //                moving_hor=false;
            }
            else if (moving_right && prev_rect.right() + dx_r > left && prev_rect.right() < left)
            {
                qreal new_dx_r = left - prev_rect.right() - M_COLLISION_MARGIN;
                dx_l -= dx_r - new_dx_r;
                dx_r = new_dx_r;
                //                moving_hor=false;
            }
        }
        else if (moving_hor && moving_ver)
        {
            // y = (dy/dx)*(x-x0) + y0
            // x = (dx/dy)*(y-y0) + x0

            qreal d_yx_tr = dy_t / dx_r;
            qreal d_yx_br = dy_b / dx_r;
            qreal d_yx_tl = dy_t / dx_l;
            qreal d_yx_bl = dy_b / dx_l;

            qreal d_xy_rb = dx_r / dy_b;
            qreal d_xy_lb = dx_l / dy_b;
            qreal d_xy_rt = dx_r / dy_t;
            qreal d_xy_lt = dx_l / dy_t;
            // NOte: if collision => moving_dir= false; ( Maybe recompute moving ...)

            if (moving_right && prev_rect.right() < left)
            {
                qreal y_top_right = d_yx_tr * (left - prev_rect.right()) + prev_rect.top();

                qreal y_bottom_right = d_yx_br * (left - prev_rect.right()) + prev_rect.bottom();

                //                if ((y_top_right >= top && y_top_right <= bottom) || (y_bottom_right >= top && y_bottom_right <= bottom))
                if (y_top_right <= bottom && y_bottom_right >= top)
                {
                    qreal new_dx_r = fmin(dx_r, left - prev_rect.right() - M_COLLISION_MARGIN);
                    dx_l -= dx_r - new_dx_r;
                    dx_r = new_dx_r;
                    //                    moving_right=false;
                }
            }

            if (moving_left && prev_rect.left() > right)
            {
                qreal y_top_left = d_yx_tl * (right - prev_rect.left()) + prev_rect.top();

                qreal y_bottom_left = d_yx_bl * (right - prev_rect.left()) + prev_rect.bottom();

                //                if ((y_top_left >= top && y_top_left <= bottom) || (y_bottom_left >= top && y_bottom_left <= bottom))
                if (y_top_left <= bottom && y_bottom_left >= top)
                {
                    qreal new_dx_l = fmax(dx_l, right - prev_rect.left() + M_COLLISION_MARGIN);
                    dx_r += dx_l - new_dx_l;
                    dx_l = new_dx_l;
                    //                    moving_left=false;
                }
            }

            ///////////////////////////////
            if (moving_down && prev_rect.bottom() < top) //+2*M_COLLISION_MARGIN)
            {
                qreal x_bottom_right = d_xy_rb * (top - prev_rect.bottom()) + prev_rect.right();

                qreal x_bottom_left = d_xy_lb * (top - prev_rect.bottom()) + prev_rect.left();

                // (A && B) || (C && D) => A && D
                // a*b+c*d
                // a*d = a*b +c*d +a*d -a*b -c*d
                // A && D = A && (D || B || !B) = (A && B) || (A && D) || (A && !B) || (C && D) || (!C && D)
                // => A && D = (A && B) || (C && D) || (A && !B) || (!C && D)
                // (A && !B) = x_bottom_right >= left && x_bottom_right > right
                // (!C && D) = x_bottom_left < left && x_bottom_left <= right
                ////////////////////////////////////////
                // (A && D) || (C || !D) || (!A || B) = (A && B) || (C && D)
                ///////////////////////////////////////////////////////////////////////////////////////////////////
                // // A && D = A && (D && (B || !B)) = A & D & B || A & D & !B = D & ( (A & B) || (A &!B))
                // ( (A & B) || (A &!B)) & (C || !C) & D = ( (A & B) || (A &!B)) & ((C & D) || (!C & D))
                // => (A & B) &
                //                if ((x_bottom_right >= left && x_bottom_right <= right) || (x_bottom_left >= left && x_bottom_left <= right))
                //                {
                // prev_rect.width() > colliding_rect.width() || prev_rect.width() < colliding_rect.width()
                if (x_bottom_right >= left && x_bottom_left <= right)
                {
                    qreal new_dy_b = fmin(dy_b, top - prev_rect.bottom() - M_COLLISION_MARGIN);
                    dy_t -= dy_b - new_dy_b;
                    dy_b = new_dy_b;
                    //                    moving_down=false;
                }
                else
                {
                    qDebug() << x_bottom_right << dx_r << dy_b << dx_l << left << x_bottom_left << right;
                }
            }
            if (moving_up && prev_rect.top() >= bottom)
            {
                qreal x_top_right = d_xy_rt * (bottom - prev_rect.top()) + prev_rect.right();

                qreal x_top_left = d_xy_lt * (bottom - prev_rect.top()) + prev_rect.left();

                //                if ((x_top_right >= left && x_top_right <= right) || (x_top_left >= left && x_top_left <= right))
                if (x_top_right >= left && x_top_left <= right)
                {
                    qreal new_dy_t = fmax(dy_t, bottom - prev_rect.top() + M_COLLISION_MARGIN);
                    dy_b += dy_t - new_dy_t;
                    dy_t = new_dy_t;
                    //                    moving_up=false;
                }
            }
        }
        //        else
        //        {
        //            // (!moving_ver && !moving_hor)
        //            dy_b = 0;
        //            dy_t = 0;
        //            dx_l = 0;
        //            dx_r = 0;

        //            break;
        //        }

        //        moving_hor = moving_hor&(moving_left | moving_right);
        //        moving_ver = moving_ver&(moving_up | moving_down);
    }

    return QRectF(prev_rect.topLeft() + QPointF(dx_l, dy_t), prev_rect.bottomRight() + QPointF(dx_r, dy_b));
}

qreal Collision::distance(const QRectF &rect1, const QRectF &rect2) const
{
    QPointF center1 = rect1.center();
    QPointF center2 = rect2.center();
    qreal dx = center1.x() - center2.x();
    qreal dy = center1.y() - center2.y();
    return std::sqrt(dx * dx + dy * dy);
}

bool Collision::compareDistance(const QRectF &rect1, const QRectF &rect2, const QRectF &targetRect) const
{
    qreal distance1 = distance(rect1, targetRect);
    qreal distance2 = distance(rect2, targetRect);
    return distance1 < distance2;
}
