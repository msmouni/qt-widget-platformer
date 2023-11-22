#ifndef CHARACTER_H
#define CHARACTER_H

#include <QGraphicsPixmapItem>
#include <QPainter>
#include "platform.h"
#include "animation.h"

enum class CharacterType
{
    Player,
    Enemy
};

enum class CharacterState{
    Idle,
    Run,
    Jump,
    Fall,
    Ground,
    Hit,
    DeadHit,
};

enum class CharacterDirection{
    Left,
    Right,
};

class Character : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Character(const QRectF &rect, const QColor &color);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    virtual void gameUpdate(const Platform *platform) = 0;
protected slots:
    void updateView();
protected:
    QRectF m_bounding_rect;
    CharacterType m_type;
    QColor m_color; // To change later with sprite
    SpriteAnimation *m_animation;
    CharacterState m_state;
    CharacterDirection m_direction;
};

#endif // CHARACTER_H
