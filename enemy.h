#ifndef ENEMY_H
#define ENEMY_H

#include "character.h"

class Enemy : public Character
{
public:
    Enemy(const QRectF &rect, const QString &res_path, const QRectF &player_rect);

    void gameUpdate(const Platform *platform) override;

private:
    const QRectF &m_player_rect;
};

#endif // ENEMY_H
