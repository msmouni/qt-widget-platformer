#ifndef ENEMY_H
#define ENEMY_H

#include "character.h"
#include "path_finding.h"
#include <QMutex>

class Enemy : public Character
{
    Q_OBJECT
public:
    Enemy(const QPointF &pos, const QString &res_path, const Platform &platform);

    void updateKinematics() override;
    void gameUpdate() override;

protected slots:
    void setPathFindingResult(QVector<QPoint>);

private:
    const int M_PATH_FOLLOW_PRECISION = 3; // pxl
    PathFinder m_path_finder;
    QVector<QPoint> m_path_tiles;
    // The mutex is locked when QMutexLocker is created. If locked, the mutex will be unlocked when the QMutexLocker is destroyed.
    QMutex m_path_mutex;
    bool m_idle_pos_set;
    QPointF m_idle_pos;

    // TODO: move to another class later
    QGraphicsRectItem m_attack_zone;
    QPointF m_go_to_pos;

    void findPath();
    void checkPathNodeReached();
    void followPath();
    bool checkAttackZone();
};

#endif // ENEMY_H
