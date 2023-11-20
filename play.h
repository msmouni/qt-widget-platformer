#ifndef PLAYVIEW_H
#define PLAYVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include "player.h"
#include "platform.h"

class PlayView : public QGraphicsView
{
    Q_OBJECT
public:
    PlayView(QWidget *parent = nullptr);

protected:
    void wheelEvent(QWheelEvent *event) override;

private:
    QGraphicsScene *m_scene;
    Player *m_player;
    QTimer *m_update_timer;
    int m_update_timeout_ms;

    Platform *m_platform;

    QPointF m_camera_pos;

    void updateCam();
private slots:
    void updateItems();
};

#endif // PLAYVIEW_H
