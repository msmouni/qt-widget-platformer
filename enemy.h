#ifndef ENEMY_H
#define ENEMY_H

#include "character.h"

class Enemy : public Character
{
public:
    Enemy(const QRectF &rect, const QColor &color, const QRectF & player_rect);

    void gameUpdate(const Platform *platform) override;

    // TMP
    /*void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;*/

private:
    const QRectF & m_player_rect;

    // TMP
    bool m_jumping_to_point;

//    void updateAnimation() override;
};

#endif // ENEMY_H
