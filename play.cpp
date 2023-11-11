#include "play.h"

PlayView::PlayView(QWidget *parent) : QGraphicsView(parent)
{
    m_scene = new QGraphicsScene(this);
    this->setScene(m_scene);

    m_scene->setBackgroundBrush(Qt::blue); // TMP

    m_player = new Player(QRectF(100, 100, 50, 90), Qt::red);

    m_scene->addItem(m_player);

    m_update_timer = new QTimer;

    m_update_timeout_ms = 50;

    m_update_timer->setInterval(m_update_timeout_ms);
    m_update_timer->start();
    connect(m_update_timer, SIGNAL(timeout()), this, SLOT(updateItems()));
}

void PlayView::updateItems()
{
    m_player->update();
}
