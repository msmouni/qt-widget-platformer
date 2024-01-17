#ifndef FIND_H
#define FIND_H

#include <QObject>
#include <QVector>
#include <QPoint>
#include "platform.h"

const int MAX_JUMP_COUNT = 4;

enum class PathFindingTileType : uint8_t
{
    Empty,
    Solid,
    JumpThrough,
    FallThrough,
    VisitedUpWay,
    VisitedDownWay,
    Visited,
};

class PathFindingTile
{
private:
    QPoint m_pos;
    QVector<QPoint> m_parents;
    bool m_can_jump;
    int m_jump_count;

public:
    PathFindingTile(QPoint pos = QPoint(0, 0)) : m_pos(pos)
    {
        m_jump_count = 0;
        m_can_jump = false;
    }

    PathFindingTile(QPoint pos, PathFindingTile &parent_tile) : m_pos(pos)
    {
        m_parents = parent_tile.getPath();

        if (parent_tile.m_pos.y() > pos.y())
        {
            m_can_jump = true;
            m_jump_count = parent_tile.getJumpCount() + 1;
        }
        else
        {
            m_can_jump = false;
            m_jump_count = 0;
        }
    }

    const QPoint &getPos() const
    {
        return m_pos;
    }

    const QVector<QPoint> &getParents() const
    {
        return m_parents;
    }

    bool canJump()
    {
        return m_can_jump && m_jump_count < MAX_JUMP_COUNT;
    }

    void setCanJump()
    {
        m_jump_count = 0;
        m_can_jump = true;
    }

    const int &getJumpCount()
    {
        return m_jump_count;
    }

    bool isReachedUpWay()
    {
        return m_jump_count != 0;
    }

    QVector<QPoint> getPath()
    {
        QVector<QPoint> path = getParents();
        path.append(getPos());

        return path;
    }
};

enum class AdjacentTile
{
    Left,
    Right,
    Top,
    Bottom,
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight,
};

struct PathFindingMap
{
private:
    int m_nb_rows;
    int m_nb_columns;
    QSizeF m_tile_size;
    QVector<QVector<PathFindingTileType>> m_tiles_type;
    QPoint m_start_pos;
    QPoint m_target_pos;

    QPoint getTilePos(const QPointF &pos_f)
    {
        return QPoint(pos_f.x() / m_tile_size.width(), pos_f.y() / m_tile_size.height());
    }

public:
    PathFindingMap(const Platform &platform)
    {
        m_nb_rows = platform.getNbRows();
        m_nb_columns = platform.getNbColumns();
        m_tile_size = platform.getTileSize();

        for (int idx_x = 0; idx_x < m_nb_columns; idx_x++)
        {
            QVector<PathFindingTileType> tile_line;
            for (int idx_y = 0; idx_y < m_nb_rows; idx_y++)
            {
                tile_line.append((PathFindingTileType)platform.getTileType(idx_x, idx_y));
            }

            m_tiles_type.append(tile_line);
        }
    }

    const int &getNbRows()
    {
        return m_nb_rows;
    }

    const int &getNbColumns()
    {
        return m_nb_columns;
    }

    const PathFindingTileType &getTileType(const int &pos_x, const int &pos_y)
    {
        return m_tiles_type[pos_x][pos_y];
    }

    const PathFindingTileType &getTileType(const QPoint &pos)
    {
        return getTileType(pos.x(), pos.y());
    }

    bool isTileSolid(const int &pos_x, const int &pos_y)
    {
        const PathFindingTileType &tile_type = getTileType(pos_x, pos_y);
        return (tile_type == PathFindingTileType::Solid || tile_type == PathFindingTileType::FallThrough || tile_type == PathFindingTileType::JumpThrough);
    }

    void setTileType(const int &pos_x, const int &pos_y, PathFindingTileType tile_type)
    {
        m_tiles_type[pos_x][pos_y] = tile_type;
    }
    void setTileType(const QPoint &pos, PathFindingTileType tile_type)
    {
        setTileType(pos.x(), pos.y(), tile_type);
    }
    void setStartPos(const QPointF &pos)
    {
        m_start_pos = getTilePos(pos);
    }
    const QPoint getStartPos()
    {
        return m_start_pos;
    }
    void setTargetPos(const QPointF &pos)
    {
        m_target_pos = getTilePos(pos);
    }
    const QPoint getTargetPos()
    {
        return m_target_pos;
    }
    void clear()
    {
        for (QVector<PathFindingTileType> &tile_column : m_tiles_type)
        {
            for (PathFindingTileType &tile_type : tile_column)
            {
                if (tile_type != PathFindingTileType::Solid && tile_type != PathFindingTileType::JumpThrough && tile_type != PathFindingTileType::FallThrough)
                {
                    tile_type = PathFindingTileType::Empty;
                }
            }
        }
    }
    bool containsTile(const int &pos_x, const int &pos_y)
    {
        return 0 <= pos_x && pos_x < getNbColumns() && 0 <= pos_y && pos_y < getNbRows();
    }
    bool containsTile(const QPoint &pos)
    {
        return containsTile(pos.x(), pos.y());
    }
};

class PathFinding : public QObject
{
public:
    explicit PathFinding(QObject *parent, const Platform &platform);

    virtual void init() = 0;
    QVector<QPoint> find(const QPointF &start_pos, const QPointF &target_pos);

private:
    void tryToProcessTile(const int &x, const int &y, AdjacentTile which_tile);
    void setCurrentTileVisited();

protected:
    PathFindingMap m_map;
    PathFindingTile m_current_tile;

    virtual void reset() = 0;
    virtual void initSearch() = 0;
    virtual void updateCurrentTile() = 0;
    virtual bool isQueueEmpty() = 0;
    virtual void processTile(const int &tile_idx_x, const int &tile_idx_y) = 0;
    virtual void processJumpTile(const int &tile_idx_x, const int &tile_idx_y) = 0;
    void processAdjacentTiles(const QPoint &tile_idx);
};

#endif // FIND_H
