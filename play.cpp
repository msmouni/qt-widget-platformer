#include "play.h"

PlayView::PlayView(QWidget *parent) : QGraphicsView(parent)
{
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Scene
    m_scene = new QGraphicsScene(this);
    this->setScene(m_scene);
    m_scene->setBackgroundBrush(Qt::blue); // TMP

    //    // Reverse the y-axis by setting a vertical flip transformation
    //    this->setTransform(QTransform(1, 0, 0, -1, 0, height()));

    // Load an image and add it to the scene (Resource File (.qrc))
    //    m_map_img=m_scene->addPixmap(QPixmap(":Pirate_bomb/map_64_64_w30_h20.png"));
    //    QPixmap pix(":Pirate_bomb/map_64_64_w30_h20.png");

    // Tilesthis->collidingItems()
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

    // Platform
    m_platform = new Platform(QSizeF(64, 64), ":Pirate_bomb/map_64_64_w30_h20.csv", ":/Pirate_bomb/Tile-Sets/Tile-Sets (64-64).png", tiles_hash);
    m_scene->addItem(m_platform);

    // Player
    m_player = new Player(QPointF(200, 200), ":/Pirate_bomb/Player-Bomb Guy", *m_platform);
    m_scene->addItem(m_player);
    m_camera_pos = m_player->pos();
    m_player_rect = m_player->sceneBoundingRect();

//    // Enemies
//    Enemy *enemy = new Enemy(QPointF(150, 200), ":/Pirate_bomb/Enemy-Bald Pirate", *m_platform, m_player_rect);
//    m_scene->addItem(enemy);
//    m_enemies.append(enemy);

    m_weapons_count = 0;

    // Update
    m_update_timer = new QTimer(this);
    m_update_timeout_ms = 50;
    m_update_timer->setInterval(m_update_timeout_ms);
    m_update_timer->start();
    connect(m_update_timer, SIGNAL(timeout()), this, SLOT(updateItems()));
}

void PlayView::wheelEvent(QWheelEvent *event)
{
    event->ignore();
}

void PlayView::mousePressEvent(QMouseEvent *event)
{
    event->ignore();
}

void PlayView::mouseDoubleClickEvent(QMouseEvent *event)
{
    event->ignore();
}

void PlayView::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();

    if (key == Qt::Key_B)
    {
        qDebug() << "B";
        // TMP
        qreal dir_x = 1;
        if (m_player->getDirection() == CharacterDirection::Left)
        {
            dir_x = -1;
        }

        // Note: pixmap pos (-50 ...)
        Bomb *wpn = new Bomb(m_weapons_count, m_player->sceneBoundingRect().topLeft() - QPointF(50, 50), dir_x * 100, -100, 250, 250, *m_platform, ":/Pirate_bomb/Objects/BOMB");

        wpn->start();
        m_scene->addItem(wpn);
        connect(wpn, SIGNAL(terminate(Weapon *)), this, SLOT(dropWeapon(Weapon *)));
        m_weapons.insert(m_weapons_count, wpn);

        m_weapons_count += 1;
    }

    QGraphicsView::keyPressEvent(event);
}

void PlayView::keyReleaseEvent(QKeyEvent *event)
{
    QGraphicsView::keyPressEvent(event);
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
    m_platform->update();

    m_player->updateShapes();
    for (Enemy *enemy : m_enemies)
    {
        enemy->updateShapes();
    }

    m_player->gameUpdate();

    m_player_rect = m_player->sceneBoundingRect();

    //    qDebug()<<"pl"<<m_player->sceneBoundingRect()<<"en"<<m_enemies.first()->sceneBoundingRect()<<" => int:"<<m_player->sceneBoundingRect().intersects(m_enemies.first()->sceneBoundingRect());
    for (Enemy *enemy : m_enemies)
    {
        enemy->gameUpdate();
    }

    //    for (Weapon* weapon: m_weapons.values()){
    //        weapon->updateWeapon();
    //    }

    for (QHash<int, Weapon *>::const_iterator weapons_it = m_weapons.constBegin(); weapons_it != m_weapons.constEnd(); ++weapons_it)
    {
        weapons_it.value()->updateWeapon();
    }

    updateCam();
}

void PlayView::dropWeapon(Weapon *weapon)
{
    m_weapons.remove(weapon->getId());

    weapon->deleteLater();
}
