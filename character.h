#ifndef CHARACTER_H
#define CHARACTER_H

#include <QGraphicsItem>
#include <QPainter>

enum class CharacterType
{
    Player,
    Enemy
};

class Character : public QGraphicsItem
{
public:
    Character(const QRectF &rect, const QColor &color);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    virtual void update() = 0;

protected:
    QRectF m_bounding_rect;
    CharacterType m_type;
    QColor m_color; // To change later with sprite
};

#endif // CHARACTER_H
