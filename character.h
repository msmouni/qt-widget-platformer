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

enum class CharacterState : uint8_t
{
    Init = 0,
    Idle = 1,
    Run = 2,
    Jump = 3,
    Fall = 4,
    Ground = 5,
    Hit = 6,
    DeadHit = 7,
};

enum class CharacterDirection
{
    Left,
    Right,
};

class Character : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    // TODO: Add character shape rectangle + collision with other characters + collision with objects(weapons)
    Character(const QPointF &pos, const QString &res_path, const Platform &platform);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    /*
boundingRect() returns the bounding rectangle of the item. The bounding rectangle is used for optimization purposes and provides a hint about the item's extent. It should include the entire area that the item occupies, even if it's transparent or not painted.

shape() returns the shape of the item as a QPainterPath. The shape represents the actual outline of the item. It is used for hit testing, collision detection, and other geometric operations.
     */
    //    QPainterPath shape() const override;

    virtual void gameUpdate() = 0;

    const CharacterDirection &getDirection() const;
protected slots:
    void updateView();

protected:
    QRectF m_bounding_rect;
    CharacterType m_type;
    SpriteAnimation *m_animation;
    const Platform &m_platform;

    qreal m_speed_x;
    qreal m_speed_y;
    qreal m_acc_x;
    qreal m_acc_y;

    qreal m_acc_max;
    qreal m_friction;
    qreal m_gravity;

    CharacterState m_state;
    CharacterDirection m_direction;

    void updateCharacter();
    virtual void updateAnimation();
    bool isOnGround();
};

#endif // CHARACTER_H
