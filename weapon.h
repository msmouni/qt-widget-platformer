#ifndef WEAPON_H
#define WEAPON_H

#include <QGraphicsPixmapItem>
#include "platform.h"
#include "animation.h"

enum class WeaponState{
    Idle,
    Starting,
    Active,
};

class Weapon : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Weapon(int id, const QRectF &scene_rect, const QRectF &active_rect, qreal power_x, qreal power_y, const Platform &platform, const QString &res_path=nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    QPainterPath shape() const override;

    virtual void start();
    void activate();
    void desactivate();

    bool isActive() const;
    qreal getPowerX() const;
    qreal getPowerY() const;
    int getId();

    virtual void updateWeapon()=0;

signals:
    void terminate(Weapon *);

protected slots:
    void updateView();

protected:
    int m_id;
    WeaponState m_state;

    QRectF m_scene_rect;
    QRectF m_active_rect;

    qreal m_power_x;
    qreal m_power_y;

    qreal m_hit_power; // To rename

    const Platform &m_platform;

    bool m_visible;
    SpriteAnimation *m_animation;
};

#endif // WEAPON_H
