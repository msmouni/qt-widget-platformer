#ifndef PLAYER_H
#define PLAYER_H

#include "character.h"
#include <QKeyEvent>
#include <QTimer>

class Player : public Character
{
public:
    Player(const QPointF &pos, const QString &res_path, const Platform &platform);

    void gameUpdate() override;

private:
    const int M_NB_BOMB_MAX = 3;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
};

#endif // PLAYER_H
