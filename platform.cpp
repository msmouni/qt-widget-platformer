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
