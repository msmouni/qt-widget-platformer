#ifndef TILE_H
#define TILE_H

#include <QGraphicsItem>
#include <QPainter>

class Tile : public QGraphicsItem
{
public:
    Tile(QRectF rect);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

private:
    QRectF m_rect;
};

#endif // TILE_H
