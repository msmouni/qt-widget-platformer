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

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    //    bool collidesWithItem(const QGraphicsItem* other, Qt::ItemSelectionMode mode) const override;

    //    QPainterPath shape() const override;

    QRectF handleCollision(QRectF rect, qreal &dx, qreal &dy) const;
    void update();

//    const TileType& getTileType(poin)
    QVector<QPointF> getReachedTiles(QRectF rect, qreal dx, qreal dy) const;

private:
    QSizeF m_tile_size;
    int m_nb_rows;
    int m_nb_columns;
    //    QRectF m_rect;

    // Maybe to move elsewhere and use it for other items also
    //    QVector<QVector<int>> m_map;
    //    QPainterPath m_path;
    QVector<QVector<Tile *>> m_tiles;
    QPixmap m_tileset_pixmap;

    QRect getIndexRect(QRectF rect, qreal &dx, qreal &dy) const;
};

#endif // PLATFORM_H
