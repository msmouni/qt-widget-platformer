#include "platform.h"
#include "csv.h"

Platform::Platform(QSizeF tile_size, QString map_csv_path)
{
    m_tile_size=tile_size;

    m_map=loadCSV(map_csv_path);//":Pirate_bomb/map_64_64_w30_h20.csv");

    if (!m_map.isEmpty()){
        m_nb_rows=m_map.size();
        m_nb_columns=m_map[0].size();

//        m_rect=QRectF(0,0,tile_size.width()*m_nb_columns,tile_size.height()*m_nb_rows);
        qDebug()<<"platform:"<<"m_nb_rows"<<m_nb_rows<<"m_nb_columns"<<m_nb_columns;
    }else {
        m_nb_rows=0;
        m_nb_columns=0;
    }

    //////////////////////
    this->childItems();

    for (int i = 0; i < m_nb_rows; i++)
    {
        for (int j = 0; j < m_nb_columns; j++)
        {
            if (m_map[i][j]>=0)
            {
//                QGraphicsRectItem rect(j * m_tile_size.width(), i * m_tile_size.height(), m_tile_size.width(), m_tile_size.height());
                m_tiles.append(new Tile(QRectF(j * m_tile_size.width(), i * m_tile_size.height(), m_tile_size.width(), m_tile_size.height())));

                this->addToGroup(m_tiles.last());
//                m_path.addRect(QRect(j * m_tile_size.width(), i * m_tile_size.height(), m_tile_size.width(), m_tile_size.height()));
            }
        }
    }


    qDebug()<<"m_tile_size"<<m_tile_size;

}

QRectF Platform::boundingRect() const
{
//    return m_rect;
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

    for (int i = 0; i < m_nb_rows; i++)
    {

//        qDebug()<<m_map[i];
        for (int j = 0; j < m_nb_columns; j++)
        {
            if (m_map[i][j]>=0)
            {
                painter->drawRect(QRect(j * m_tile_size.width(), i * m_tile_size.height(), m_tile_size.width(), m_tile_size.height()));
            }
            /*QString txt = "(" +QString::number(i) +", "+QString::number(j) + ") :"+QString::number(m_map[i][j])  ;
            painter->drawText(j * m_tile_size.width(), i * m_tile_size.height()+15, txt);*/
        }
    }
}

QRectF Platform::handleCollision(QRectF rect, qreal &dx, qreal &dy) const
{
    qreal right_bound=fmax(rect.right(), rect.right() +dx);
    qreal left_bound=fmin(rect.left(), rect.left() +dx);
    qreal top_bound=fmin(rect.top(), rect.top() +dy);
    qreal bottom_bound=fmax(rect.bottom(), rect.bottom() +dy);

    int left_idx=left_bound/m_tile_size.width();
    int right_idx=right_bound/m_tile_size.width();
    int top_idx=top_bound/m_tile_size.height();
    int bottom_idx=bottom_bound/m_tile_size.height();

    bool going_down = dy>0;
    bool going_right = dx>0;
    bool moving_hor=abs(dx)>1e-7;
    bool moving_ver=abs(dy)>1e-7;

//    qDebug()<<"rect"<<rect<<"m_tile_size"<<m_tile_size<<"left_idx"<<left_idx<<"|right_idx"<<right_idx<<"|top_idx"<<top_idx<<"|bottom_idx"<<bottom_idx;

    for (int ind_x=left_idx;ind_x<=right_idx; ind_x++){
        for (int ind_y=top_idx;ind_y<=bottom_idx; ind_y++){
            int i_x;
            if (!going_right){
                i_x=(right_idx+left_idx)-ind_x;
            }else {
                i_x=ind_x;
            }
            int i_y;
            if (!going_down){
                i_y=(bottom_idx+top_idx)-ind_y;
            }else {
                i_y=ind_y;
            }

            qDebug()<<"ind_x"<<ind_x<<"ind_y"<<ind_y<<"| i_x"<<i_x<<"i_y"<<i_y;
            if (m_map[i_y][i_x]>=0){
                qreal left = i_x*m_tile_size.width();
                qreal right = (i_x+1)*m_tile_size.width();

                qreal top = i_y*m_tile_size.height();
                qreal bottom = (i_y+1)*m_tile_size.height();


//                qDebug()<<"i_x"<<i_x<<"| i_y"<<i_y<<"| left"<<left<<"| right"<<right<<"| top"<<top<<"| right"<<right;

                /*if (moving_ver && !moving_hor){
                    // dx=0
                    if (!going_down && rect.top()+dy < bottom){
                        dy=bottom-rect.top()+0.001;
                    }else if (going_down &&rect.bottom()+dy > top){
                        dy=top-rect.bottom()-0.001;
                    }
                }else if (moving_ver) {
                    // y = (dy/dx)*(x-x0) + y0

                }else {
                    dy=0;
                }

                if (moving_hor && !moving_ver){
                    // dy=0
                    if (!going_right && rect.left()+dx < right){
                        dx=right-rect.left()+0.001;
                    }else if (going_right &&rect.right()+dx > left){
                        dx=left-rect.right()-0.001;
                    }
                }else if (moving_hor) {

                }else {
                    dx=0;
                }*/

                if (moving_ver && !moving_hor){
                    // dx=0
                    if (!going_down && rect.top()+dy < bottom){
                        dy=bottom-rect.top()+0.001;
                    }else if (going_down &&rect.bottom()+dy > top){
                        dy=top-rect.bottom()-0.001;
                    }
                }else if (moving_hor && !moving_ver){
                    // dy=0
                    if (!going_right && rect.left()+dx < right){
                        dx=right-rect.left()+0.001;
                    }else if (going_right &&rect.right()+dx > left){
                        dx=left-rect.right()-0.001;
                    }
                }else if (moving_hor && moving_ver){
                    // y = (dy/dx)*(x-x0) + y0
                    // x = (dx/dy)*(y-y0) + x0

                    qreal dyx=dy/dx;
                    qreal dxy=dx/dy;

                    if (going_right){
                        qreal y_top_right=dyx*(left-rect.right())+rect.top();

                        qreal y_bottom_right=dyx*(left-rect.right())+rect.bottom();
//                        qDebug()<<"check 1"<<"y_top_right"<<y_top_right<<"y_bottom_right"<<y_bottom_right<<"top"<<top<<"bottom"<<bottom;

                        if ((y_top_right>=top && y_top_right<=bottom) || (y_bottom_right>=top && y_bottom_right<=bottom)){
//                            qDebug()<<"check";
                            dx= left-rect.right()-0.001;
                        }
                    }else {
                        // Left
                        qreal y_top_left=dyx*(right-rect.left())+rect.top();

                        qreal y_bottom_left=dyx*(right-rect.left())+rect.bottom();
                        //                        qDebug()<<"check 1"<<"y_top_right"<<y_top_right<<"y_bottom_right"<<y_bottom_right<<"top"<<top<<"bottom"<<bottom;

                        if ((y_top_left>=top && y_top_left<=bottom) || (y_bottom_left>=top && y_bottom_left<=bottom)){
                            //                            qDebug()<<"check";
                            dx=right-rect.left()+0.001;
                        }

                    }
                    ///////////////////////////////
                    if (going_down){
                        qreal x_bottom_right=dxy*(top-rect.bottom())+rect.right();

                        qreal x_bottom_left=dxy*(top-rect.bottom())+rect.left();

                        //                        qDebug()<<"check 1"<<"y_top_right"<<y_top_right<<"y_bottom_right"<<y_bottom_right<<"top"<<top<<"bottom"<<bottom;

                        if ((x_bottom_right>=left && x_bottom_right<=right) || (x_bottom_left>=left && x_bottom_left<=right)){
                            //                            qDebug()<<"check";
                            dy=top-rect.bottom()-0.001;
                        }
                    }else {
                        // Up
                        qreal x_top_right=dxy*(bottom-rect.top())+rect.right();

                        qreal x_top_left=dxy*(bottom-rect.top())+rect.left();

                        //                        qDebug()<<"check 1"<<"y_top_right"<<y_top_right<<"y_bottom_right"<<y_bottom_right<<"top"<<top<<"bottom"<<bottom;

                        if ((x_top_right>=left && x_top_right<=right) || (x_top_left>=left && x_top_left<=right)){
                            //                            qDebug()<<"check";
                            dy=bottom-rect.top()+0.001;
                        }

                    }
                }else {
                    // (!moving_ver && !moving_hor)
                    dy=0;
                    dx=0;
                }

//                qDebug()<<"dx"<<dx<<"dy"<<dy;
            }
        }
    }
    return rect.translated(dx,dy);
}

void Platform::update()
{

}

/*bool Platform::collidesWithItem(const QGraphicsItem *other, Qt::ItemSelectionMode mode) const
{
    QRectF other_rect=other->sceneBoundingRect();

    if (m_rect.intersects(other_rect)){
        int x_min=(int)(other_rect.left() / m_tile_size.width());
        int x_max=(int)(other_rect.right() / m_tile_size.width());
        int y_min=(int)(other_rect.top() / m_tile_size.height());
        int y_max=(int)(other_rect.bottom() / m_tile_size.height());

        for (int i=x_min; i<=x_max;i++){
            for (int j=y_min; j<=y_max;j++){
                if (m_map[i][j]>0)
                {
                    qDebug()<< "collides" << i <<","<<j;
                    return true;
                }
            }
        }

    }

    return false;
}

QPainterPath Platform::shape() const
{
    return m_path;
}
*/
