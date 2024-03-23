#include "enemy.h"

Enemy::Enemy(const QPointF &pos, const QString &res_path, const Platform &platform, const QRectF &player_rect) : Character(pos, res_path, platform), m_path_finder(platform), m_player_rect(player_rect)
{
    setData(0, "Enemy");
    m_type = CharacterType::Enemy;

    m_path_iter = M_NB_PATH_ITER;

    //    m_animation->stop();

    connect(&m_path_finder, SIGNAL(pathFindingRes(QVector<QPoint>)), this, SLOT(setPathFindingResult(QVector<QPoint>)));
}

void Enemy::updateKinematics()
{
    followPath();

    Character::updateKinematics();
}

void Enemy::gameUpdate()
{
    QPointF pos = m_dynamics->getEntityBoxPos();
    QPoint tile_below = m_platform.getTileIdx(pos);
    tile_below.setY(tile_below.y() + 1);

    bool on_ground = m_platform.isInMap(tile_below) && (m_platform.getTileType(tile_below) == TileType::Solid || m_platform.getTileType(tile_below) == TileType::JumpThrough);

    if (on_ground && (m_path_tiles.isEmpty() || m_path_length != m_path_tiles.length())) // m_path_tiles.isEmpty() &&
    {
        //        qDebug()<<"Check";
        m_path_finder.findPath(pos, m_player_rect.topLeft()); // .center());
        m_path_iter = M_NB_PATH_ITER;
    }

    updateCharacter();
}

void Enemy::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (!m_path_tiles.isEmpty())
    {
        QPainterPath path;

        path.moveTo(mapFromScene(m_platform.getRectInTile(m_path_tiles[0], this->boundingRect()))); // QPointF((m_path_tiles[0].x() + 0.5) * m_platform.getTileSize().width(), (m_path_tiles[0].y() + 0.5) * m_platform.getTileSize().height()))); // Move to the first point

        for (int i = 1; i < m_path_tiles.size(); ++i)
        {
            //            QPointF pnt = QPointF((m_path_tiles[i].x() + 0.5) * m_platform.getTileSize().width(), (m_path_tiles[i].y() + 0.5) * m_platform.getTileSize().height());
            path.lineTo(mapFromScene(m_platform.getRectInTile(m_path_tiles[i], this->boundingRect()))); // pnt)); // Connect subsequent points with lines
        }

        QPen pen(Qt::red);
        pen.setWidth(20);
        painter->setPen(pen);

        painter->drawPath(path);

        QPen pen2(Qt::yellow);

        pen2.setWidth(9);
        painter->setPen(pen2);

        painter->drawPoint(this->mapFromScene(m_dynamics->getEntityBoxPos()));
        //        painter->drawPoint(this->mapFromScene(boundingRect().topLeft()));
        //        painter->drawRect(mapRectFromScene(m_platform.getTileRect(m_path_tiles.first())));
    }

    Character::paint(painter, option, widget);
}

void Enemy::setPathFindingResult(QVector<QPoint> path)
{
    QMutexLocker ml(&m_path_mutex);

    //    m_path_tiles = path;

    // TODO: Move to another function
    QVector<QPoint> new_path;
    if (!path.isEmpty())
    {
        new_path.append(path.first());

        if (path.length() > 1)
        {
            QPoint last_dir = path[1] - path[0];
            for (int idx = 1; idx <= path.length(); idx++)
            {
                QPoint dir = path[idx] - path[idx - 1];

                if (dir != last_dir)
                {
                    new_path.append(path[idx - 1]);
                    last_dir = dir;
                }
                else if (idx == path.length() - 1)
                {
                    new_path.append(path[idx]);
                }

                /*if (new_path.last().y() != path[idx].y() && new_path.last().x() != path[idx-1].x()  || new_path.last().x() != path[idx].x() && new_path.last().y() != path[idx-1].y()){
                    new_path.append(path[idx-1]);
                }else if(idx==path.length()-1){
                    new_path.append(path[idx]);
                }*/
            }
        }
        new_path.pop_front();

        //        // TMP
        //        path.pop_front();
        /*QPoint enemy_tile= QPoint(round(pos().x()/m_platform.getNbRows()), round(pos().y()/m_platform.getNbColumns()));

        if (enemy_tile != path.first()){
            path.insert(0,enemy_tile);
        }*/
        m_path_length = path.length();
    }

    //    qDebug()<<path<<" >> "<<new_path;

    m_path_tiles = path; // new_path;
}

void Enemy::followPath()
{
    QMutexLocker ml(&m_path_mutex);

    if (m_path_tiles.length() == 1)
    {
        QPointF next_pos = m_platform.getRectInTile(m_path_tiles[0], this->boundingRect());
        QPointF dist = next_pos - m_dynamics->getEntityBoxPos();

        if (sqrt(QPointF::dotProduct(dist, dist)) < 1)
        {
            m_path_tiles.pop_front();
        }
        //        else if (sqrt(QPointF::dotProduct(dist, dist))>150){
        //            m_path_tiles.clear();
        //        }
    }
    else if (m_path_tiles.length() > 1)
    {
        QPointF prev_pos = m_platform.getRectInTile(m_path_tiles[0], this->boundingRect());
        QPointF next_pos = m_platform.getRectInTile(m_path_tiles[1], this->boundingRect());

        QPointF dir = next_pos - prev_pos;
        QPointF dist = next_pos - m_dynamics->getEntityBoxPos();

        if (QPointF::dotProduct(dir, dist) < 0 || sqrt(QPointF::dotProduct(dist, dist)) < 10)
        {
            m_path_tiles.pop_front();
        }
        //        if (sqrt(QPointF::dotProduct(dist, dist))>150){
        //            m_path_tiles.clear();
        //        }
    }

    if (!m_path_tiles.isEmpty())
    {
        QPointF next_pos;
        if (m_path_tiles.length() == 1)
        {
            next_pos = m_platform.getRectInTile(m_path_tiles[0], this->boundingRect());
        }
        else
        {
            next_pos = m_platform.getRectInTile(m_path_tiles[1], this->boundingRect());
        }

        QPointF speed = next_pos - m_dynamics->getEntityBoxPos();

        qreal accel_x = speed.x() - m_dynamics->getSpeedX() * m_dynamics->getFriction();
        qreal accel_y = speed.y() - m_dynamics->getSpeedY() * m_dynamics->getFriction() - m_dynamics->getGravity();

        qDebug() << "accel_x" << accel_x << "accel_y" << accel_y << "speed" << speed;

        m_dynamics->setAccelX(fmin(fmax(accel_x, m_dynamics->getMinAccel()), m_dynamics->getMaxAccel())); // speed.x()>0.01 ? M_ACCEL_MAX: speed.x()<-0.01 ? -M_ACCEL_MAX:0);
                                                                                                          //        m_dynamics->setAccelY(fmin(fmax(accel_y, -20), 20));//M_ACCEL_MAX));
                                                                                                          //        m_dynamics->setAccelY(speed.y()<0 ? M_JUMP_ACCEL:0);

        if (speed.y() < 0)
        {
            jump();
            //            if (isOnGround()){
            //                m_dynamics->setAccelY(-50);//-m_dynamics->getGravity());//30);
            //            }
            //        }else if(m_path_tiles.length()>1){
            //            QPointF next_pos= m_platform.getRectInTile(m_path_tiles[1], this->boundingRect());
        }
        else if (m_path_tiles.length() == 1 || m_path_tiles.length() > 1 && m_path_tiles[1].y() >= m_path_tiles[0].y())
        {
            stopJump();
        }
    }
    else
    {
        m_dynamics->setAccelX(0);
        m_dynamics->setAccelY(0);
    }
}
