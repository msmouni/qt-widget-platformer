#include "find.h"
#include <QQueue>
#include <QCoreApplication>
#include <QElapsedTimer>
#include <QThread>

PathFinding::PathFinding(QObject *parent, const Platform &platform)
    : QObject{parent}, m_map(platform)
{
}

QVector<QPoint> PathFinding::find(const QPointF &start_pos, const QPointF &target_pos)
{
    reset();

    m_map.setStartPos(start_pos);
    m_map.setTargetPos(target_pos);

    initSearch();

    while (!isQueueEmpty())
    {
        updateCurrentTile();

        const QPoint &current_pos = m_current_tile.getPos();

        PathFindingTileType tile_type = m_map.getTileType(current_pos);

        // Note: When adding a Tile with lower cost to the priority_queue, the old Tile with greater cost remains in the priority_queue
        if (tile_type != PathFindingTileType::Visited)
        {
            if (current_pos == m_map.getTargetPos())
            {
                return m_current_tile.getPath();
            }

            if (m_current_tile.getPos().y() + 1 < m_map.getNbRows() && m_map.isTileSolid(m_current_tile.getPos().x(), m_current_tile.getPos().y() + 1))
            {
                m_current_tile.setCanJump();
            }

            processAdjacentTiles(m_current_tile.getPos()); // new elements are pushed to the priority_queue, so references obtained by m_priority_queue.top() will be invalid

            setCurrentTileVisited();
        }
    }

    return QVector<QPoint>();
}

void PathFinding::tryToProcessTile(const int &tile_idx_x, const int &tile_idx_y, AdjacentTile which_tile)
{
    if (m_map.containsTile(tile_idx_x, tile_idx_y))
    {
        PathFindingTileType tile_type = m_map.getTileType(tile_idx_x, tile_idx_y);

        bool going_up = which_tile == AdjacentTile::Top || which_tile == AdjacentTile::TopLeft || which_tile == AdjacentTile::TopRight;

        bool is_tile_valid = tile_type == PathFindingTileType::Empty || (tile_type == PathFindingTileType::VisitedDownWay && going_up) || (tile_type == PathFindingTileType::VisitedUpWay);

        if (is_tile_valid)
        {
            switch (which_tile)
            {
            case AdjacentTile::Left:
            {
                // Current tile is on platform or will land on platform
                if ((tile_idx_y + 1 < m_map.getNbRows() && (m_map.isTileSolid(tile_idx_x + 1, tile_idx_y + 1) || m_current_tile.isReachedUpWay() && m_map.isTileSolid(tile_idx_x, tile_idx_y + 1))))
                {
                    processTile(tile_idx_x, tile_idx_y);
                }
                break;
            }
            case AdjacentTile::Right:
            {
                // Current tile is on platform or will land on platform
                if ((tile_idx_y + 1 < m_map.getNbRows() && (m_map.isTileSolid(tile_idx_x - 1, tile_idx_y + 1) || m_current_tile.isReachedUpWay() && m_map.isTileSolid(tile_idx_x, tile_idx_y + 1))))
                {
                    processTile(tile_idx_x, tile_idx_y);
                }
                break;
            }
            case AdjacentTile::Top:
            {
                if (m_current_tile.canJump())
                {
                    processJumpTile(tile_idx_x, tile_idx_y);
                }
                break;
            }
            case AdjacentTile::TopLeft:
            {
                // Is accessible
                bool is_not_corner = !m_map.isTileSolid(tile_idx_x, tile_idx_y + 1) && !m_map.isTileSolid(tile_idx_x + 1, tile_idx_y);

                if (is_not_corner && m_current_tile.canJump())
                {
                    processJumpTile(tile_idx_x, tile_idx_y);
                }
                break;
            }
            case AdjacentTile::TopRight:
            {
                // Is accessible
                bool is_not_corner = !m_map.isTileSolid(tile_idx_x, tile_idx_y + 1) && !m_map.isTileSolid(tile_idx_x - 1, tile_idx_y);

                if (is_not_corner && m_current_tile.canJump())
                {
                    processJumpTile(tile_idx_x, tile_idx_y);
                }
                break;
            }
            case AdjacentTile::Bottom:
            {
                processTile(tile_idx_x, tile_idx_y);
                break;
            }
            case AdjacentTile::BottomLeft:
            {
                // Is accessible
                bool is_not_corner = !m_map.isTileSolid(tile_idx_x, tile_idx_y - 1) && !m_map.isTileSolid(tile_idx_x + 1, tile_idx_y);

                if (is_not_corner)
                {
                    processTile(tile_idx_x, tile_idx_y);
                }
                break;
            }
            case AdjacentTile::BottomRight:
            {
                // Is accessible
                bool is_not_corner = !m_map.isTileSolid(tile_idx_x, tile_idx_y - 1) && !m_map.isTileSolid(tile_idx_x - 1, tile_idx_y);

                if (is_not_corner)
                {
                    processTile(tile_idx_x, tile_idx_y);
                }
                break;
            }
            default:
                break;
            }
        }
    }
}

void PathFinding::setCurrentTileVisited()
{
    const QPoint &current_pos = m_current_tile.getPos();
    const PathFindingTileType &current_type = m_map.getTileType(current_pos);

    if ((current_type == PathFindingTileType::VisitedDownWay && m_current_tile.isReachedUpWay()) || (current_type == PathFindingTileType::VisitedUpWay && !m_current_tile.isReachedUpWay()))
    {
        m_map.setTileType(current_pos, PathFindingTileType::Visited);
    }
    else if (m_current_tile.isReachedUpWay())
    {
        m_map.setTileType(current_pos, PathFindingTileType::VisitedUpWay);
    }
    else
    {
        m_map.setTileType(current_pos, PathFindingTileType::VisitedDownWay);
    }
}

void PathFinding::processAdjacentTiles(const QPoint &tile_idx)
{
    int tile_x = tile_idx.x();
    int tile_y = tile_idx.y();

    tryToProcessTile(tile_x - 1, tile_y, AdjacentTile::Left);
    tryToProcessTile(tile_x + 1, tile_y, AdjacentTile::Right);
    tryToProcessTile(tile_x, tile_y - 1, AdjacentTile::Top);
    tryToProcessTile(tile_x, tile_y + 1, AdjacentTile::Bottom);
    // Note: since we are using a queue the order of insertion matters (First being inserted, first to be checked), so we prioritize horizantal and vertical mvmt over diagonal ones
    tryToProcessTile(tile_x - 1, tile_y - 1, AdjacentTile::TopLeft);
    tryToProcessTile(tile_x - 1, tile_y + 1, AdjacentTile::BottomLeft);
    tryToProcessTile(tile_x + 1, tile_y - 1, AdjacentTile::TopRight);
    tryToProcessTile(tile_x + 1, tile_y + 1, AdjacentTile::BottomRight);
}
