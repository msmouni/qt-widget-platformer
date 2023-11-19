#include "play.h"

PlayView::PlayView(QWidget *parent) : QGraphicsView(parent)
{
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

void PlayView::updateItems()
{
    m_platform->update();
    m_player->update(m_platform);
}
