#ifndef PLATFORM_H
#define PLATFORM_H

#include <QGraphicsItem>
#include <QPainter>
#include "tile.h"

class Platform : public QGraphicsItemGroup
{
public:
    Platform(QSizeF tile_size, QString map_csv_path);

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QRectF handleCollision(QRectF rect, qreal &dx, qreal &dy) const;
    void update();

private:
    QSizeF m_tile_size;
    int m_nb_rows;
    int m_nb_columns;
    QVector<QVector<int>> m_map;
    QVector<Tile *> m_tiles;
};

#endif // PLATFORM_H
