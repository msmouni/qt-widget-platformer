#ifndef PLAYVIEW_H
#define PLAYVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include "player.h"

class PlayView : public QGraphicsView
{
    Q_OBJECT
public:
    PlayView(QWidget *parent = nullptr);

private:
    QGraphicsScene *m_scene;
    Player *m_player;
    QTimer *m_update_timer;
    int m_update_timeout_ms;
private slots:
    void updateItems();
};

#endif // PLAYVIEW_H
