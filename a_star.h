#ifndef ASTAR_H
#define ASTAR_H

#include "find.h"
#include <queue>

struct AstarCost
{
private:
    qreal m_cost;
    int m_nb_jump;

public:
    AstarCost(qreal cost = 0, int nb_jump = MAX_JUMP_COUNT) : m_cost(cost), m_nb_jump(nb_jump) {}

    void setCost(qreal new_cost)
    {
        m_cost = new_cost;
    }

    qreal getCost() const
    {
        return m_cost;
    }

    void setJumpCost(qreal new_cost, int nb_jump)
    {
        m_cost = new_cost;
        m_nb_jump = nb_jump;
    }

    int getNbJump() const
    {
        return m_nb_jump;
    }
};

struct AstarTile
{
private:
    PathFindingTile m_tile;
    qreal m_cost_from_start;
    qreal m_estimated_target_cost;

public:
    AstarTile(QPoint pos, qreal cost, qreal target_cost, PathFindingTile &parent) : m_cost_from_start(cost), m_estimated_target_cost(target_cost), m_tile(pos, parent) {}

    AstarTile(QPoint pos, qreal cost, qreal target_cost) : m_cost_from_start(cost), m_estimated_target_cost(target_cost), m_tile(pos) {}

    const QPoint &getPos() const
    {
        return m_tile.getPos();
    }

    const PathFindingTile &getTile() const
    {
        return m_tile;
    }

    qreal getCost() const
    {
        return m_cost_from_start;
    }

    qreal getWeight() const
    {
        return m_cost_from_start + m_estimated_target_cost;
    }

    // operator used in priority_queue
    bool operator<(const AstarTile &other) const
    {
        return getWeight() > other.getWeight();
    }
};

class Astar : public PathFinding
{
public:
    explicit Astar(QObject *parent, const Platform &platform);

    void init();

private:
    const qreal MAX_WEIGHT_VALUE = 99999;
    QVector<QVector<AstarCost>> m_weight_map;
    qreal m_current_tile_cost;

    // Note reg std::multiset : https://stackoverflow.com/questions/5895792/why-is-using-a-stdmultiset-as-a-priority-queue-faster-than-using-a-stdpriori
    std::priority_queue<AstarTile> m_priority_queue;

    // from parent
    void reset();
    void initSearch();
    void updateCurrentTile();
    bool isQueueEmpty();
    void processTile(const int &tile_idx_x, const int &tile_idx_y);
    void processJumpTile(const int &tile_idx_x, const int &tile_idx_y);

    void reinitWeightMap();

    qreal getEstimatedTargetCost(const QPoint &idx);
    qreal getEstimatedTargetCost(const int &idx_x, const int &idx_y);
};

#endif // ASTAR_H
