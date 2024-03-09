#ifndef BOMB_H
#define BOMB_H

#include "weapon.h"
#include <QTimer>
#include "dynamic.h"

class Bomb : public Weapon
{
public:
    Bomb(int id, const QPointF &pos, qreal speed_x, qreal speed_y, qreal power_x, qreal power_y, const QString &res_path);

    void start() override;

    void updateKinematics() override;
    void updateWeapon() override;

    const CollisionRect *getCollisionRect() const;

protected slots:
    void explosion();
    void end();

private:
    const int M_EXPLOSION_TIEMOUT_MS = 3000;
    const int M_DROP_TIEMOUT_MS = 500;
    QTimer *m_explosion_timer;
    QTimer *m_drop_timer;

    EntityDynamics *m_dynamics;
};

#endif // BOMB_H
