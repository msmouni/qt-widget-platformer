#include "path_finding.h"

PathFinder::PathFinder(const Platform &platform) : m_algo(this, platform)
{
}

void PathFinder::run()
{
    QVector<QPoint> res = m_algo.find(m_start_pos, m_target_pos);

    emit pathFindingRes(res);
}

void PathFinder::findPath(const QPointF &start_pos, const QPointF &target_pos)
{
    m_start_pos = start_pos;
    m_target_pos = target_pos;

    if (isRunning())
    {
        terminate();
        wait();
    }
    start();
}
