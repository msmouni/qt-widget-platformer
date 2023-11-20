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

    // To move later to Map class (or World)
    //    QGraphicsPixmapItem *m_map_img;

    Platform *m_platform;

    QPointF m_camera_pos;

    void updateCam();

    /*protected:
        void keyPressEvent(QKeyEvent* event) override;
        void keyReleaseEvent(QKeyEvent* event) override;

        void resizeEvent(QResizeEvent *event) override;

    private:
        QGraphicsScene* scene;
        qreal m_init_height;
        qreal m_init_width;
        QPointF m_init_pos;
        qreal m_old_scale=1;*/
private slots:
    void updateItems();
};

#endif // PLAYVIEW_H
