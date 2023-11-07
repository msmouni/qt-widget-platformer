#ifndef CHARACTER_H
#define CHARACTER_H

#include <QGraphicsItem>
#include <QRectF>
#include <QPainter>
#include <QColor>

class Character : public QGraphicsItem
{
public:
    Character(const QRectF& rect, const QColor& color)
        : m_bounding_rect(rect), m_color(color) {};

    QRectF boundingRect() const override
    {
        return m_bounding_rect;
//        qreal penWidth = 1;
//        return QRectF(-10 - penWidth / 2, -10 - penWidth / 2,
//                      20 + penWidth, 20 + penWidth);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override
    {
        painter->setBrush(QBrush(m_color));
        painter->drawRect(m_bounding_rect);
    }

private:
    QRectF m_bounding_rect;
    QColor m_color; // To change later with sprite

};

#endif // CHARACTER_H
