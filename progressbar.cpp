#include "progressbar.h"

ProgressBar::ProgressBar(QGraphicsItem *parent, QRectF bounding_rect, qreal progress, QColor bar_color, QColor background_color)
    : QGraphicsItem(parent), m_bounding_rect(bounding_rect), m_barColor(bar_color), m_backgroundColor(background_color)
{
    m_progress = qBound(0.0, progress, 1.0);
}

void ProgressBar::setProgress(qreal progress)
{
    m_progress = qBound(0.0, progress, 1.0);
    update(); // Request a repaint
}

void ProgressBar::setBarColor(const QColor &color)
{
    m_barColor = color;
    update();
}

void ProgressBar::setBackgroundColor(const QColor &color)
{
    m_backgroundColor = color;
    update();
}

QRectF ProgressBar::boundingRect() const
{
    return m_bounding_rect;
}

void ProgressBar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing);

    painter->setBrush(m_backgroundColor);
    painter->drawRect(boundingRect());

    qreal progressWidth = boundingRect().width() * m_progress;
    painter->setBrush(m_barColor);
    painter->drawRect(QRectF(boundingRect().topLeft(), QSize(progressWidth, boundingRect().height())));
}
