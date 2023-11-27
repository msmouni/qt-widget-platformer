#ifndef SPRITEANIMATION_H
#define SPRITEANIMATION_H

#include <QGraphicsPixmapItem>

/*
#include <QGraphicsPixmapItem>
#include <QTimer>

class SpriteAnimation : public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit SpriteAnimation(const QPixmap& spriteSheet, int frameCount, int frameDuration, QGraphicsItem* parent = nullptr)
        : QGraphicsPixmapItem(parent), frameCount(frameCount), currentFrame(0)
    {
        setPixmap(spriteSheet.copy(0, 0, spriteSheet.width() / frameCount, spriteSheet.height()));

        // Timer for controlling the animation
        animationTimer = new QTimer(this);
        connect(animationTimer, &QTimer::timeout, this, &SpriteAnimation::nextFrame);
        animationTimer->start(frameDuration);
    }

private slots:
    void nextFrame() {
        currentFrame = (currentFrame + 1) % frameCount;
        updatePixmap();
    }

private:
    int frameCount;
    int currentFrame;
    QTimer* animationTimer;

    void updatePixmap() {
        int frameWidth = pixmap().width() / frameCount;
        setPixmap(pixmap().copy(currentFrame * frameWidth, 0, frameWidth, pixmap().height()));
    }
};
//////////////////////////////////////////////////////////////
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QList>

class SpriteAnimation : public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit SpriteAnimation(const QList<QPixmap>& frames, int frameDuration, QGraphicsItem* parent = nullptr)
        : QGraphicsPixmapItem(parent), frames(frames), currentFrame(0)
    {
        if (!frames.isEmpty()) {
            setPixmap(frames.first());

            // Timer for controlling the animation
            animationTimer = new QTimer(this);
            connect(animationTimer, &QTimer::timeout, this, &SpriteAnimation::nextFrame);
            animationTimer->start(frameDuration);
        }
    }

private slots:
    void nextFrame() {
        currentFrame = (currentFrame + 1) % frames.size();
        updatePixmap();
    }

private:
    QList<QPixmap> frames;
    int currentFrame;
    QTimer* animationTimer;

    void updatePixmap() {
        setPixmap(frames[currentFrame]);
    }
};

*/

#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QList>
#include <QDir>
#include <QStringList>
#include <QObject>
#include <QHash>

// To adjust later (NOte: QObject for SIGNAL and SLOT)
class SpriteAnimation : public QObject
{
    Q_OBJECT
public:
    explicit SpriteAnimation(QHash<uint8_t, QString> paths, int frame_dur_ms)
        : QObject()
    {
        if (!paths.isEmpty()){
            for (uint8_t id: paths.keys()) {
                loadFramesFromFolder(id,paths[id]);
            }

            m_current_id =paths.keys().first();
            m_current_frame=0;

            if (!m_frames_id[m_current_id].isEmpty()) {
                //            setPixmap(frames.first());

                // Timer for controlling the animation
                m_animation_timer = new QTimer;
                connect(m_animation_timer, &QTimer::timeout, this, &SpriteAnimation::nextFrame);
                m_animation_timer->start(frame_dur_ms);

                m_frame_dur_ms=frame_dur_ms;

                m_rect=m_frames_id[m_current_id][0].rect().toRectF();
            }
        }

    }


    void setId(uint8_t id){
        if (m_frames_id.keys().contains(id)){
            if (m_current_id !=id){
                m_current_id=id;
                m_current_frame=0;
            }
        }

    }
    QPixmap& getPixmap(){
        return m_frames_id[m_current_id][m_current_frame];
    }

    QRectF& getRect(){
        return m_rect;
    }

private slots:
    void nextFrame() {
        m_current_frame = (m_current_frame + 1) % m_frames_id[m_current_id].size();
        emit updatePixmap();
    }
signals:
    void updatePixmap();

private:
    QHash<uint8_t, QList<QPixmap>> m_frames_id;
    uint8_t m_current_id;
    int m_current_frame;
    int m_frame_dur_ms;
    QTimer* m_animation_timer;
    QRectF m_rect;

//    void updatePixmap() {
////        setPixmap(frames[current_frame]);
//    }


    void loadFramesFromFolder(uint8_t frame_id, const QString& folder_path) {
        QList<QPixmap> frames;
        QDir dir(folder_path);
        QStringList name_filters;
        name_filters << "*.png";  // Assuming sprite frames are PNG files, adjust accordingly
        QStringList file_names = dir.entryList(name_filters, QDir::Files | QDir::NoDotAndDotDot);

        foreach (const QString& file_name, file_names) {
            frames.append(QPixmap(dir.filePath(file_name)));
        }

        m_frames_id.insert(frame_id, frames);
    }
};

#endif // SPRITEANIMATION_H
