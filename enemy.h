#ifndef ENEMY_H
#define ENEMY_H

#include "character.h"
#include <QMediaPlayer>
#include <QAudioOutput>

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
    QMediaPlayer *m_media_player;
    QAudioOutput* m_audio_output;

//    void updateAnimation() override;
};

#endif // ENEMY_H
