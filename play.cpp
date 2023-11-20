#include "play.h"

PlayView::PlayView(QWidget *parent) : QGraphicsView(parent)
{
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_scene = new QGraphicsScene(this);
    this->setScene(m_scene);

    m_scene->setBackgroundBrush(Qt::blue); // TMP

    m_platform = new Platform(QSizeF(64, 64), ":Pirate_bomb/map_64_64_w30_h20.csv");

    m_platform->setData(0, "Platform");

    m_platform->setData(0,"Platform");

    for (QGraphicsItem *child_items : m_platform->childItems())
    {
        m_scene->addItem(child_items);
    }

    m_player = new Player(QRectF(200, 200, 58, 58), Qt::red);

    m_player->setData(0, "Player");

    m_scene->addItem(m_player);

    m_update_timer = new QTimer;

    m_update_timeout_ms = 50;

    m_update_timer->setInterval(m_update_timeout_ms);
    m_update_timer->start();
    connect(m_update_timer, SIGNAL(timeout()), this, SLOT(updateItems()));
}

void PlayView::wheelEvent(QWheelEvent *event)
{
    event->ignore();
}

void PlayView::updateCam()
{
    QPointF player_pos = m_player->pos();
    m_camera_pos.setX(m_camera_pos.x() + (player_pos.x() - m_camera_pos.x()) / 5);
    m_camera_pos.setY(m_camera_pos.y() + (player_pos.y() - m_camera_pos.y()) / 5);
    this->centerOn(m_camera_pos);
}

void PlayView::updateItems()
{
    m_platform->update();
    m_player->update(m_platform);

    updateCam();
}
