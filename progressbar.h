#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include <QGraphicsItem>
#include <QRectF>
#include <QPainter>

class ProgressBar : public QGraphicsItem
{
public:
    ProgressBar(QGraphicsItem *parent = nullptr, QRectF bounding_rect = {},
                qreal progress = 0, QColor bar_color = Qt::blue, QColor background_color = Qt::lightGray);

    void setProgress(qreal progress);

    void setBarColor(const QColor &color);

    void setBackgroundColor(const QColor &color);

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
    QRectF m_bounding_rect;
    qreal m_progress;
    QColor m_barColor;
    QColor m_backgroundColor;
};
#endif // PROGRESSBAR_H
