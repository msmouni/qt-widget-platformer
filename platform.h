#ifndef PLATFORM_H
#define PLATFORM_H

#include <QGraphicsItem>
#include <QPainter>
#include "tile.h"

class Platform : public QGraphicsItemGroup
{
public:
    Platform(QSizeF tile_size, QString map_csv_path, QString tileset_png_path, QHash<int, TileType> tiles_hash);

    QRectF boundingRect() const override;

    QRectF handleCollision(QRectF rect, qreal &dx, qreal &dy) const;

private:
    QSizeF m_tile_size;
    int m_nb_rows;
    int m_nb_columns;
    QVector<QVector<Tile *>> m_tiles;
    QPixmap m_tileset_pixmap;

    QRect getIndexRect(QRectF rect, qreal &dx, qreal &dy) const;
};

#endif // PLATFORM_H
