#ifndef SPRITEANIMATION_H
#define SPRITEANIMATION_H

#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QList>
#include <QDir>
#include <QStringList>
#include <QObject>
#include <QHash>

class SpriteAnimation : public QObject
{
    Q_OBJECT
public:
    explicit SpriteAnimation(QHash<uint8_t, QString> paths, int frame_dur_ms);

    void setId(uint8_t id);

    const QPixmap &getPixmap();

    const QRectF &getRect();

private slots:
    void nextFrame();
signals:
    void updatePixmap();

private:
    QHash<uint8_t, QList<QPixmap>> m_frames_id;
    uint8_t m_current_id;
    int m_current_frame;
    int m_frame_dur_ms;
    QTimer *m_animation_timer;
    QRectF m_rect;

    void loadFramesFromFolder(uint8_t frame_id, const QString &folder_path);
};

#endif // SPRITEANIMATION_H
