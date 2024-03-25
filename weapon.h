#ifndef WEAPON_H
#define WEAPON_H

#include <QGraphicsPixmapItem>
#include "animation.h"

enum class WeaponState
{
    Idle,
    Starting,
    Active,
};

class Weapon : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Weapon(int id, const QPointF &pos, qreal power_x, qreal power_y, const QString &res_path);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    virtual void start();
    void activate();
    void desactivate();

    bool isActive() const;
    qreal getPowerX() const;
    qreal getPowerY() const;
    int getId();

    virtual void updateKinematics() = 0;
    virtual void updateWeapon() = 0;

signals:
    void terminate(Weapon *);

public slots:
    virtual void pause();
    virtual void resume();

protected slots:
    void updateView();

protected:
    int m_id;
    WeaponState m_state;

    QRectF m_bounding_rect;

    qreal m_power_x;
    qreal m_power_y;

    SpriteAnimation *m_animation;
};

#endif // WEAPON_H
