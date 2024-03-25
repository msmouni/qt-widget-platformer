#include "animation.h"

SpriteAnimation::SpriteAnimation(QHash<uint8_t, QString> paths, int frame_dur_ms)
    : QObject()
{
    if (!paths.isEmpty())
    {
        for (QHash<uint8_t, QString>::const_iterator path_it = paths.constBegin(); path_it != paths.constEnd(); ++path_it)
        {
            loadFramesFromFolder(path_it.key(), path_it.value());
        }

        m_current_id = paths.constBegin().key();
        m_current_frame = 0;

        if (!m_frames_id[m_current_id].isEmpty())
        {
            m_animation_timer = new QTimer(this);
            connect(m_animation_timer, &QTimer::timeout, this, &SpriteAnimation::nextFrame);
            m_animation_timer->start(frame_dur_ms);

            m_frame_dur_ms = frame_dur_ms;

            m_rect = m_frames_id[m_current_id][0].rect().toRectF();
        }
    }
}

SpriteAnimation::~SpriteAnimation()
{
    stop();
}

void SpriteAnimation::setId(uint8_t id)
{
    if (m_frames_id.contains(id))
    {
        if (m_current_id != id)
        {
            m_current_id = id;
            m_current_frame = 0;
        }
    }else {
//        qDebug()<<m_frames_id.keys()<<id;
    }
}

const QPixmap &SpriteAnimation::getPixmap()
{
    return m_frames_id[m_current_id][m_current_frame];
}

const QRectF &SpriteAnimation::getRect()
{
    return m_rect;
}

void SpriteAnimation::addAnimationState(uint8_t state_id, QString frames_path)
{
    qDebug()<<state_id;
    loadFramesFromFolder(state_id, frames_path);
    qDebug()<<m_frames_id.keys();
}

int SpriteAnimation::getIdDuration(int id)
{
    return m_frame_dur_ms*m_frames_id.value(id).length();
}

void SpriteAnimation::stop()
{
    m_animation_timer->stop();
    m_animation_timer->deleteLater();
}

void SpriteAnimation::nextFrame()
{
    m_current_frame = (m_current_frame + 1) % m_frames_id[m_current_id].size();
    emit updatePixmap();
}

void SpriteAnimation::pause()
{
    m_animation_timer->stop();
}

void SpriteAnimation::resume()
{
    m_animation_timer->start(m_frame_dur_ms);
}

void SpriteAnimation::loadFramesFromFolder(uint8_t frame_id, const QString &folder_path)
{
    QList<QPixmap> frames;
    QDir dir(folder_path);
    QStringList name_filters;
    name_filters << "*.png"; // Assuming sprite frames are PNG files
    QStringList file_names = dir.entryList(name_filters, QDir::Files | QDir::NoDotAndDotDot);

    foreach (const QString &file_name, file_names)
    {
        frames.append(QPixmap(dir.filePath(file_name)));
    }

    m_frames_id.insert(frame_id, frames);
}
