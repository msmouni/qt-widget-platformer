#ifndef PLAYER_H
#define PLAYER_H

#include "character.h"
#include <QKeyEvent>

class Player : public Character
{
public:
    Player(const QRectF &rect, const QColor &color);

    void gameUpdate(const Platform *platform) override;

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    //    bool m_up_pressed;
    //    bool m_down_pressed;
    //    bool m_right_pressed;
    //    bool m_left_pressed;

    //    qreal m_g=700;//9.8; // Pixel not meter
    bool m_jump;
};

#endif // PLAYER_H
