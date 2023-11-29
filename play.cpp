#include "play.h"

PlayView::PlayView(QWidget *parent) : QGraphicsView(parent)
{
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_scene = new QGraphicsScene(this);
    this->setScene(m_scene);

    m_scene->setBackgroundBrush(Qt::blue); // TMP

    //    // Reverse the y-axis by setting a vertical flip transformation
    //    this->setTransform(QTransform(1, 0, 0, -1, 0, height()));

    // Load an image and add it to the scene (Resource File (.qrc))
    //    m_map_img=m_scene->addPixmap(QPixmap(":Pirate_bomb/map_64_64_w30_h20.png"));
    //    QPixmap pix(":Pirate_bomb/map_64_64_w30_h20.png");

    QHash<int, TileType> tiles_hash;

    tiles_hash.insert(-1, TileType::Empty);
    for (int i = 0; i <= 21; i++)
    {
        tiles_hash.insert(i, TileType::Solid);
    }
    tiles_hash.insert(22, TileType::JumpThrough);
    tiles_hash.insert(23, TileType::JumpThrough);
    tiles_hash.insert(24, TileType::Solid);
    tiles_hash.insert(25, TileType::Solid);
    tiles_hash.insert(26, TileType::Solid);
    tiles_hash.insert(27, TileType::Solid);
    tiles_hash.insert(28, TileType::JumpThrough);
    tiles_hash.insert(29, TileType::JumpThrough);

    m_platform = new Platform(QSizeF(64, 64), ":Pirate_bomb/map_64_64_w30_h20.csv", ":/Pirate_bomb/8-Tile-Sets/Tile-Sets (64-64).png", tiles_hash);

    m_platform->setData(0, "Platform");

    //    m_scene->addItem(m_platform);

    m_scene->addItem(m_platform);

    m_player = new Player(QRectF(200, 200, 58, 58), Qt::red);

    m_player->setData(0, "Player");

    m_scene->addItem(m_player);

    m_camera_pos = m_player->pos();
    m_player_rect = m_player->sceneBoundingRect();

    Enemy* enemy = new Enemy(QRectF(200, 200, 58, 58), Qt::red, m_player_rect);
    enemy->setData(0, "Enemy");
    m_scene->addItem(enemy);
    m_enemies.append( enemy);

    //    m_scene->add
    m_update_timer = new QTimer;

    m_update_timeout_ms = 50;

    m_update_timer->setInterval(m_update_timeout_ms);
    m_update_timer->start();
    connect(m_update_timer, SIGNAL(timeout()), this, SLOT(updateItems()));
}

void PlayView::wheelEvent(QWheelEvent *event)
{
    event->ignore();
}

void PlayView::updateCam()
{
    QPointF player_pos = m_player->pos();
    m_camera_pos.setX(m_camera_pos.x() + (player_pos.x() - m_camera_pos.x()) / 5);
    m_camera_pos.setY(m_camera_pos.y() + (player_pos.y() - m_camera_pos.y()) / 5);
    this->centerOn(m_camera_pos);
}

void PlayView::updateItems()
{
    //    qDebug()<<"update PlayView";
    m_platform->update();
    m_player->gameUpdate(m_platform);

    m_player_rect = m_player->sceneBoundingRect();
    for (Enemy* enemy: m_enemies){
        enemy->gameUpdate(m_platform);
    }

    updateCam();
}
