#ifndef TILE_H
#define TILE_H

#include <QGraphicsItem>
#include <QPainter>
#include "collision.h"

enum class TileType : uint8_t
{
    Empty,
    Solid,
    JumpThrough,
    FallThrough,
};

class Tile : public QGraphicsItem
{
public:
    Tile(int id, QRectF rect, TileType type, QPixmap &tileset_pixmap);

    bool isEmpty();
    bool isSolid();
    bool checkUp();
    bool checkDown();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    const TileType &getType();
    // TMP
    CollisionRect *m_collision_rect;

private:
    int m_id;
    QRectF m_rect;
    TileType m_type;
    QPixmap &m_tileset_pixmap;
    QPointF m_tileset_pos;
};

#endif // TILE_H
