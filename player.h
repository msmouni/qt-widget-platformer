#ifndef PLAYER_H
#define PLAYER_H

#include "character.h"
#include <QKeyEvent>
#include <QTimer>
// #include "weapon.h"

class Player : public Character
{
public:
    Player(const QPointF &pos, const QString &res_path, const Platform &platform);

    void gameUpdate() override;
private slots:
    void jumpTimeout();

private:
    const int M_JUMP_ACCEL = -50;
    const int M_JUMP_TIMEOUT_MS = 250;
    QTimer m_jump_timer;
    //    QVector<Weapon> m_weapons;
    //    Weapon *m_weapon;

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
};

#endif // PLAYER_H
