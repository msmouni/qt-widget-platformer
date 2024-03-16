#ifndef PLATFORM_H
#define PLATFORM_H

#include <QGraphicsItem>
#include <QPainter>
#include "tile.h"
#include "collision.h"


class Platform : public QGraphicsItemGroup
{
public:
    Platform(QSizeF tile_size, QString map_csv_path, QString tileset_png_path, QHash<int, TileType> tiles_hash);

    QRectF boundingRect() const override;

    QRectF handleCollision(QRectF prev_rect, QRectF new_rect) const; // TODO: use refs

    int getNbColumns() const;
    int getNbRows() const;
    QSizeF getTileSize() const;
    const TileType &getTileType(int &x, int &y) const;
    QPointF getTileBottomCenter(const QPoint &pos) const;

private:
    const qreal M_COLLISION_MARGIN = 1e-3;
    QSizeF m_tile_size;
    int m_nb_rows;
    int m_nb_columns;
    QVector<QVector<Tile *>> m_tiles;
    QPixmap m_tileset_pixmap;

    // TMP
    Collision m_collision_handler;

    QRect getIndexRect(QRectF rect, qreal &dx, qreal &dy) const;
};

#endif // PLATFORM_H
