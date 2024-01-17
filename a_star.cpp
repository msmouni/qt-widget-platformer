#include "a_star.h"
#include <QThread>

Astar::Astar(QObject *parent, const Platform &platform)
    : PathFinding{parent, platform}
{
    init();
}

void Astar::init()
{
    m_weight_map.clear();

    for (int i = 0; i < m_map.getNbColumns(); i++)
    {
        QVector<AstarCost> tile_line;
        for (int j = 0; j < m_map.getNbRows(); j++)
        {
            tile_line.append(AstarCost(MAX_WEIGHT_VALUE, MAX_JUMP_COUNT));
        }

        m_weight_map.append(tile_line);
    }

    reset();
}

void Astar::reinitWeightMap()
{
    for (int i = 0; i < m_map.getNbColumns(); i++)
    {
        for (int j = 0; j < m_map.getNbRows(); j++)
        {
            m_weight_map[i][j].setJumpCost(MAX_WEIGHT_VALUE, MAX_JUMP_COUNT);
        }
    }
}

qreal Astar::getEstimatedTargetCost(const QPoint &idx)
{
    return getEstimatedTargetCost(idx.x(), idx.y());
}

qreal Astar::getEstimatedTargetCost(const int &idx_x, const int &idx_y)
{
    QPoint target_idx = m_map.getTargetPos();

    // TO change later (maybe)
    return sqrt(pow(target_idx.rx() - idx_x, 2) + pow(target_idx.ry() - idx_y, 2));
}

void Astar::reset()
{
    m_map.clear();
    reinitWeightMap();

    while (!m_priority_queue.empty())
    {
        m_priority_queue.pop();
    }
}

void Astar::initSearch()
{
    QPoint start_idx = m_map.getStartPos();

    m_weight_map[start_idx.x()][start_idx.y()].setJumpCost(0, 0);

    m_priority_queue.push(AstarTile(start_idx, 0, getEstimatedTargetCost(start_idx)));
}

void Astar::updateCurrentTile()
{
    m_current_tile = m_priority_queue.top().getTile();
    m_current_tile_cost = m_priority_queue.top().getCost();

    m_priority_queue.pop();
}

bool Astar::isQueueEmpty()
{
    return m_priority_queue.empty();
}

void Astar::processTile(const int &tile_idx_x, const int &tile_idx_y)
{
    const QPoint &current_tile_idx = m_current_tile.getPos();

    qreal cost = sqrt(pow(current_tile_idx.x() - tile_idx_x, 2) + pow(current_tile_idx.y() - tile_idx_y, 2)) + m_current_tile_cost;

    if (m_weight_map[tile_idx_x][tile_idx_y].getCost() > cost)
    {
        m_weight_map[tile_idx_x][tile_idx_y].setCost(cost);

        /*
         NOTE: When a new element is pushed into the priority queue,
              it may lead to reallocation and invalidation of references or pointers to elements in the container,
              including the references obtained from the previous calls of top().
        */
        m_priority_queue.push(AstarTile(QPoint(tile_idx_x, tile_idx_y), cost, getEstimatedTargetCost(tile_idx_x, tile_idx_y), m_current_tile));
    }
}

void Astar::processJumpTile(const int &tile_idx_x, const int &tile_idx_y)
{
    const QPoint &current_tile_idx = m_current_tile.getPos();

    qreal cost = sqrt(pow(current_tile_idx.x() - tile_idx_x, 2) + pow(current_tile_idx.y() - tile_idx_y, 2)) + m_current_tile_cost;

    if (m_weight_map[tile_idx_x][tile_idx_y].getCost() > cost || m_weight_map[tile_idx_x][tile_idx_y].getNbJump() > m_current_tile.getJumpCount())
    {
        m_weight_map[tile_idx_x][tile_idx_y].setJumpCost(cost, m_current_tile.getJumpCount());

        /*
        NOTE: When a new element is pushed into the priority queue,
              it may lead to reallocation and invalidation of references or pointers to elements in the container,
              including the references obtained from the previous calls of top().
        */
        m_priority_queue.push(AstarTile(QPoint(tile_idx_x, tile_idx_y), cost, getEstimatedTargetCost(tile_idx_x, tile_idx_y), m_current_tile));
    }
}
