#ifndef PLAYER_H
#define PLAYER_H

#include "character.h"
#include <QKeyEvent>

class Player : public Character
{
public:
    Player(const QRectF &rect, const QColor &color);

    void update(const Platform *platform) override;

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    //    bool m_up_pressed;
    //    bool m_down_pressed;
    //    bool m_right_pressed;
    //    bool m_left_pressed;

    //    qreal m_g=700;//9.8; // Pixel not meter
    qreal m_speed_x;
    qreal m_speed_y;
    qreal m_acc_x;
    qreal m_acc_y;

    qreal m_acc_max;
    qreal m_friction;
    qreal m_gravity;
    bool m_jump;
};

#endif // PLAYER_H
