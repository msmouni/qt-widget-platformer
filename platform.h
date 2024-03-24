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

    QVector<QRectF> getCollidingRects(QRectF prev_rect, QRectF new_rect) const;

    int getNbColumns() const;
    int getNbRows() const;
    QSizeF getTileSize() const;
    const TileType &getTileType(int &x, int &y) const;
    const TileType &getTileType(QPoint &point) const;
    QPoint getTileIdx(QPointF &point) const;
    bool isInMap(const QPoint &indx) const;
    QPointF getPosInTile(const QPoint &pos, const QRectF &rect) const;
    QRectF getTileRect(const QPoint &pos) const;

private:
    const qreal M_COLLISION_MARGIN = 1e-3;
    QSizeF m_tile_size;
    int m_nb_rows;
    int m_nb_columns;
    QVector<QVector<Tile *>> m_tiles;
    QPixmap m_tileset_pixmap;

    QRect getIndexRect(QRectF rect, qreal &dx, qreal &dy) const;
};

#endif // PLATFORM_H
