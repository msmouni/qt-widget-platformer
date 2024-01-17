#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <QThread>
#include "a_star.h"

class PathFinder : public QThread
{
    Q_OBJECT
public:
    explicit PathFinder(const Platform &platform);

    void run() override;

    void findPath(const QPointF &start_pos, const QPointF &target_pos);

signals:
    void pathFindingRes(QVector<QPoint>);

private:
    Astar m_algo;
    QPointF m_start_pos;
    QPointF m_target_pos;
};

#endif // PATHFINDING_H
