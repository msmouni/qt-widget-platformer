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

enum class CharacterState: uint8_t{
    Idle =0,
    Run=1,
    Jump=2,
    Fall=3,
    Ground=4,
    Hit=5,
    DeadHit=6,
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

    qreal m_speed_x;
    qreal m_speed_y;
    qreal m_acc_x;
    qreal m_acc_y;

    qreal m_acc_max;
    qreal m_friction;
    qreal m_gravity;

    CharacterState m_state;
    CharacterDirection m_direction;

    void updateCharacter(const Platform *platform);
    virtual void updateAnimation();
};

#endif // CHARACTER_H
