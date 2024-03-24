#ifndef CHARACTER_H
#define CHARACTER_H

#include <QGraphicsPixmapItem>
#include <QPainter>
#include "platform.h"
#include "animation.h"
#include "weapon.h"
#include "dynamic.h"

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

class Character : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Character(const QPointF &pos, const QString &res_path, const Platform &platform);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    /*
boundingRect() returns the bounding rectangle of the item. The bounding rectangle is used for optimization purposes and provides a hint about the item's extent. It should include the entire area that the item occupies, even if it's transparent or not painted.

shape() returns the shape of the item as a QPainterPath. The shape represents the actual outline of the item. It is used for hit testing, collision detection, and other geometric operations.
     */
    //    QPainterPath shape() const override;

    void updateKinematics();
    virtual void gameUpdate() = 0;

    const CollisionRect *getCollisionRect() const;
protected slots:
    void updateView();
    void dropWeapon(Weapon *);
    void jumpTimeout();

protected:
    const int M_JUMP_TIMEOUT_MS = 250;

    QRectF m_bounding_rect;
    CharacterType m_type;
    SpriteAnimation *m_animation;
    const Platform &m_platform; // Only used by Enemy (path following)
    QTimer m_jump_timer;

    EntityDynamics *m_dynamics;

    CharacterState m_state;

    int m_weapons_count;
    QHash<int, Weapon *> m_weapons;

    void updateCharacter();
    void updateState();
    virtual void updateAnimation();

    bool isOnGround();

    void moveRight();
    void moveLeft();
    void jump();
    void stopRight();
    void stopLeft();
    void stopJump();
};

#endif // CHARACTER_H
