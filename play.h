#ifndef PLAYVIEW_H
#define PLAYVIEW_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QMouseEvent>
#include "character.h"


class PlayView : public QGraphicsView
{
public:
    PlayView(QWidget* parent = nullptr);
protected:
    void mousePressEvent(QMouseEvent* event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

    void keyPressEvent(QKeyEvent* event) override;

    void resizeEvent(QResizeEvent *event) override;

private:
    QGraphicsScene* scene;
//    QGraphicsRectItem* redItem;
    Character* redItem;
    QGraphicsRectItem* blueItem;
    QGraphicsPixmapItem* imageItem;
    bool isMoving;
    QPointF lastMousePos;
    //    QSize m_init_size;
    qreal m_init_height;
    qreal m_init_width;
    QPointF m_init_pos;
    qreal m_old_scale=1;

};


#endif // PLAYVIEW_H
