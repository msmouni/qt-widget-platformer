#ifndef BOMB_H
#define BOMB_H

#include "weapon.h"
#include <QTimer>
#include "collision.h"

class Bomb : public Weapon
{
public:
    Bomb(int id, const QPointF &pos, qreal speed_x, qreal speed_y, qreal power_x, qreal power_y, const Platform &platform, const QString &res_path);

    void start() override;

    void updateShapes() override; // To rename
    void updateWeapon() override;

    const CollisionRect* getCollisionRect() const;

protected slots:
    void explosion();
    void end();

private:
    const int M_EXPLOSION_TIEMOUT_MS = 3000;
    const int M_DROP_TIEMOUT_MS = 500;
    QTimer *m_explosion_timer;
    QTimer *m_drop_timer;

    qreal m_speed_x;
    qreal m_speed_y;
    qreal m_acc_x;
    qreal m_acc_y;

    qreal m_friction;
    qreal m_gravity;

    CollisionRect *m_collision_rect;
};

#endif // BOMB_H
