#include "character.h"

Character::Character(const QRectF &rect, const QColor &color)
{
    // pos = center
    m_bounding_rect = QRectF(-rect.width() / 2, -rect.height() / 2, rect.width(), rect.height());
    this->setPos(rect.center());

    m_color = color;

//    setPixmap(m_animation->getPixmap());

//    connect(m_animation, SIGNAL(updatePixmap()), this, SLOT(updateView()));
}

QRectF Character::boundingRect() const
{
    return m_bounding_rect;
}

void Character::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

//    painter->setBrush(QBrush(m_color));
//    painter->drawRect(m_bounding_rect);
//    m_animation->paint(painter,option,widget);
    switch (m_direction) {
    case CharacterDirection::Left:
        painter->drawPixmap(m_bounding_rect,pixmap().transformed(QTransform().scale(-1, 1)),m_animation->getRect());
        break;
    case CharacterDirection::Right:
        painter->drawPixmap(m_bounding_rect,pixmap(),m_animation->getRect());
        break;
    default:
        break;
    }

}

void Character::updateView()
{
    setPixmap(m_animation->getPixmap());
}


