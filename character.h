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
    Attack = 8,
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

    virtual void updateKinematics();
    virtual void gameUpdate() = 0;

    CollisionRect *getCollisionRect();

    bool isAttacking() const;

public slots:
    virtual void pause();
    virtual void resume();

protected slots:
    void updateView();
    void dropWeapon(Weapon *);
    void jumpTimeout();
    void attackTimeout();
    void hitTimeout();

protected:
    //    const int M_JUMP_TIMEOUT_MS = 500;
    const int M_MAX_TILES_JUMP = 5;
    const int M_HIT_TIMEOUT_MS = 300;

    QRectF m_bounding_rect;
    CharacterType m_type;
    SpriteAnimation *m_animation;
    const Platform &m_platform; // Only used by Enemy (path following)

    // TODO: USE the same Timer
    int m_jump_timeout_ms;
    QTimer m_jump_timer;
    int m_remaining_jump_time;

    QTimer m_attack_timer;
    int m_remaining_attack_time;
    bool m_attacking;

    QTimer m_hit_timer;
    int m_remaining_hit_time;
    bool m_hit;

    EntityDynamics *m_dynamics;

    CharacterState m_state;

    int m_weapons_count;
    QHash<int, Weapon *> m_weapons;

    void updateCharacter();
    virtual void updateState();
    virtual void updateAnimation();

    bool isOnGround();
    bool isHit();

    void moveRight();
    void moveLeft();
    void jump();
    void stopRight();
    void stopLeft();
    void stopJump();
    void hit(QPointF hit_pos, qreal power_x, qreal power_y);
};

#endif // CHARACTER_H
