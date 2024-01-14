#ifndef PLAYER_H
#define PLAYER_H

#include "character.h"
#include <QKeyEvent>
#include <QTimer>

class Player : public Character
{
public:
    Player(const QRectF &rect, const QString &res_path, const Platform &platform);

    void gameUpdate() override;
private slots:
    void jumpTimeout();

private:
    const int M_JUMP_ACCEL = -50;
    const int M_JUMP_TIMEOUT_MS = 250;
    QTimer m_jump_timer;

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
};

#endif // PLAYER_H
