#ifndef PLAYVIEW_H
#define PLAYVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include "player.h"
#include "platform.h"
#include "enemy.h"

class PlayView : public QGraphicsView
{
    Q_OBJECT
public:
    PlayView(QWidget *parent = nullptr);

protected:
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    QGraphicsScene *m_scene;
    Player *m_player;
    QTimer *m_update_timer;
    int m_update_timeout_ms;
    bool m_pause;

    Platform *m_platform;

    QPointF m_camera_pos;

    QRectF m_player_rect;
    QVector<Enemy *> m_enemies;

    void updateCam();
    void pause();
    void resume();

private slots:
    void updateItems();
};

#endif // PLAYVIEW_H
