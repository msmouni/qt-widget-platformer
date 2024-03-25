#ifndef ENEMY_H
#define ENEMY_H

#include "character.h"
#include "path_finding.h"
#include <QMutex>

class Enemy : public Character
{
    Q_OBJECT
public:
    Enemy(const QPointF &pos, const QString &res_path, const Platform &platform, const QRectF &player_rect);

    void updateKinematics() override;
    void gameUpdate() override;

    // TMP
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

protected slots:
    void setPathFindingResult(QVector<QPoint>);

private:
    const int M_PATH_FOLLOW_PRECISION = 3; // pxl
    const QRectF &m_player_rect;
    PathFinder m_path_finder;
    QVector<QPoint> m_path_tiles;
    // The mutex is locked when QMutexLocker is created. If locked, the mutex will be unlocked when the QMutexLocker is destroyed.
    QMutex m_path_mutex;

    void findPath();
    void checkPathNodeReached();
    void followPath();
//    void updateState() override;
};

#endif // ENEMY_H
