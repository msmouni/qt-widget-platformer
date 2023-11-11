#include "character.h"

Character::Character(const QRectF &rect, const QColor &color)
{
    // pos = center
    m_bounding_rect = QRectF(-rect.width() / 2, -rect.height() / 2, rect.width(), rect.height());
    this->setPos(rect.center());

    m_color = color;
}

QRectF Character::boundingRect() const
{
    return m_bounding_rect;
}

void Character::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    painter->setBrush(QBrush(m_color));
    painter->drawRect(m_bounding_rect);
}
