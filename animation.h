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

// To adjust later (NOte: QObject for SIGNAL and SLOT)
class SpriteAnimation : public QObject
{
    Q_OBJECT
public:
    explicit SpriteAnimation(const QString& folder_path, int frame_dur_ms)
        : QObject(), current_frame(0)
    {
        loadFramesFromFolder(folder_path);

        if (!frames.isEmpty()) {
//            setPixmap(frames.first());

            // Timer for controlling the animation
            animation_timer = new QTimer;
            connect(animation_timer, &QTimer::timeout, this, &SpriteAnimation::nextFrame);
            animation_timer->start(frame_dur_ms);
        }

        m_rect=frames[0].rect().toRectF();

    }


    QPixmap& getPixmap(){
        return frames[current_frame];
    }

    QRectF& getRect(){
        return m_rect;
    }

private slots:
    void nextFrame() {
        current_frame = (current_frame + 1) % frames.size();
        emit updatePixmap();
    }
signals:
    void updatePixmap();

private:
    QList<QPixmap> frames;
    int current_frame;
    QTimer* animation_timer;
    QRectF m_rect;

//    void updatePixmap() {
////        setPixmap(frames[current_frame]);
//    }


    void loadFramesFromFolder(const QString& folder_path) {
        QDir dir(folder_path);
        QStringList name_filters;
        name_filters << "*.png";  // Assuming sprite frames are PNG files, adjust accordingly
        QStringList file_names = dir.entryList(name_filters, QDir::Files | QDir::NoDotAndDotDot);

        foreach (const QString& file_name, file_names) {
            frames.append(QPixmap(dir.filePath(file_name)));
        }
    }
};

#endif // SPRITEANIMATION_H
