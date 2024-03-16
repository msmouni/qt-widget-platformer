#include "character.h"
#include "weapon.h"
#include <QGraphicsScene>

Character::Character(const QPointF &pos, const QString &res_path, const Platform &platform) : m_platform(platform)
{
    QHash<uint8_t, QString> animations_ids;
    animations_ids.insert((uint8_t)CharacterState::Idle, res_path + "/Idle");
    animations_ids.insert((uint8_t)CharacterState::Run, res_path + "/Run");
    animations_ids.insert((uint8_t)CharacterState::Jump, res_path + "/Jump");
    animations_ids.insert((uint8_t)CharacterState::Fall, res_path + "/Fall");
    animations_ids.insert((uint8_t)CharacterState::Ground, res_path + "/Ground");
    animations_ids.insert((uint8_t)CharacterState::Hit, res_path + "/Hit");
    animations_ids.insert((uint8_t)CharacterState::DeadHit, res_path + "/Dead Hit");

    m_animation = new SpriteAnimation(animations_ids, 50);

    m_bounding_rect = m_animation->getRect();
    this->setPos(pos);

    m_state = CharacterState::Idle;
    m_direction = CharacterDirection::Right;

    m_animation->setId(static_cast<uint8_t>(m_state));

    setPixmap(m_animation->getPixmap());

    connect(m_animation, SIGNAL(updatePixmap()), this, SLOT(updateView()));

    m_speed_x = 0;
    m_speed_y = 0;
    m_acc_x = 0;
    m_acc_y = 0;

    m_acc_max = 15;
    m_friction = 0.5;
    m_gravity = 13;

//    m_animation->stop();

//    coverage_rect=QGraphicsRectItem(m_bounding_rect,this);
}

QRectF Character::boundingRect() const
{
    return m_bounding_rect;
}

void Character::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(m_bounding_rect, pixmap(), this->shape().boundingRect());

    // Set the pen and brush for the rectangle
    QPen pen(Qt::green);
    pen.setWidth(2);
    painter->setPen(pen);
    painter->drawRect(this->boundingRect());

    painter->drawPath(this->shape());

    QPen pen2(Qt::yellow);
    pen.setWidth(2);
    painter->setPen(pen2);
    painter->drawRect(coverage_rect.boundingRect());
    ///////////////////////////////////////////::
    painter->drawRect(this->sceneBoundingRect());

    pen.setWidth(9);
    painter->setPen(pen);
    painter->drawPoint(boundingRect().topLeft());
    painter->drawPoint(sceneBoundingRect().topLeft());
}

void Character::updateShapes()
{
    m_speed_x *= m_friction; // Friction
    m_speed_x += m_acc_x;

    m_speed_y *= m_friction; // Friction
    m_speed_y += m_acc_y + m_gravity;


    m_collision_rect.m_old_rect=sceneBoundingRect();

    // Note: m_bounding_rect should match the shape bounding rect: shape is at pos=(x,y) reg this->pos()
    m_bounding_rect = this->shape().boundingRect();//.translated(-this->shape().boundingRect().topLeft());

//    QPointF scene_adjustmnt = m_bounding_rect.topLeft();
    //    if (m_direction == CharacterDirection::Left)
    //    {
    //        scene_adjustmnt.setX(-scene_adjustmnt.x());
    //    }

//    this->setPos(sceneBoundingRect().topLeft() - scene_adjustmnt);
//    m_bounding_rect.translate(-scene_adjustmnt);
//    this->moveBy(scene_adjustmnt.x(),-scene_adjustmnt.y());


//    qDebug()<<sceneBoundingRect().topLeft() - scene_adjustmnt;

//    qDebug()<<m_bounding_rect;

//    qDebug()<<sceneBoundingRect()<<m_bounding_rect.translated(pos());//.topLeft()<<pos()<<scene_adjustmnt;
    m_collision_rect.m_new_rect=sceneBoundingRect();
    m_collision_rect.m_speed_x=m_speed_x;
    m_collision_rect.m_speed_y=m_speed_y;

    /*if (m_speed_x > 1)
    {
        this->setTransform(QTransform().scale(1, 1));
        m_direction = CharacterDirection::Right;
    }
    else if (m_speed_x < -1 || m_direction == CharacterDirection::Left)
    {
        this->setTransform(QTransform().scale(-1, 1).translate(-sceneBoundingRect().width(), 0)); ////////// Width change ...
        m_direction = CharacterDirection::Left;
    }*/

//    this->moveBy(m_speed_x, m_speed_y);

//    this->setPos(sceneBoundingRect().translated(m_speed_x, m_speed_y).topLeft());

//    sceneBoundingRect().translated(m_speed_x, m_speed_y)
//    updateAnimation();


}

const CharacterDirection &Character::getDirection() const
{
    return m_direction;
}

void Character::updateView()
{
    setPixmap(m_animation->getPixmap());
}

void Character::updateCharacter()
{
    // with this model: S_n = S_{n-1} * f + Acc => Sn = Acc *(1 - f^n)/(1-f) => #with f < 1 and n >> 0: Sn = Acc/(1-f)
    /*m_speed_x *= m_friction; // Friction
    m_speed_x += m_acc_x;

    m_speed_y *= m_friction; // Friction
    m_speed_y += m_acc_y + m_gravity;

    // TODO: Better handle for State and direction

    QRectF prev_rect = sceneBoundingRect();
    m_bounding_rect = this->shape().boundingRect();*/

    /*QRectF res = m_platform.handleCollision(prev_rect, sceneBoundingRect().translated(m_speed_x, m_speed_y));

    m_speed_x = res.center().x() - sceneBoundingRect().center().x();
    m_speed_y = res.center().y() - sceneBoundingRect().center().y();

    if ((0 < m_speed_x && m_speed_x <= 1e-2) || (0 > m_speed_x && m_speed_x >= -1e-2))
    {
        m_speed_x = 0;
    }

    if ((0 < m_speed_y && m_speed_y <= 1e-2) || (0 > m_speed_y && m_speed_y >= -1e-2))
    {
        m_speed_y = 0;
    }

    if (m_speed_x > 1)
    {
        this->setTransform(QTransform().scale(1, 1));
        m_direction = CharacterDirection::Right;
    }
    else if (m_speed_x < -1 || m_direction == CharacterDirection::Left)
    {
        this->setTransform(QTransform().scale(-1, 1).translate(-sceneBoundingRect().width(), 0)); ////////// Width change ...
        m_direction = CharacterDirection::Left;
    }*/

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /*if (no_collision_speed_y > m_speed_y && m_state == CharacterState::Fall)
    {
        m_state = CharacterState::Ground;
    }
    else if (m_speed_y > 0.01)
    {
        m_state = CharacterState::Fall;
    }
    else */
    if (abs(m_speed_x) > 1)
    {
        m_state = CharacterState::Run;
    }
    else if (abs(m_speed_y) <= 0.01)
    {
        m_state = CharacterState::Idle;
    }

    // TMP
   /*QVector<QRectF> colliding_rects=m_platform.getCollidingRects(prev_rect, sceneBoundingRect().translated(m_speed_x, m_speed_y));

    /*QPointF local_prev_rect_tl=this->mapFromScene(prev_rect.topLeft());
    QPointF local_prev_rect_br=this->mapFromScene(prev_rect.bottomRight());
    QPointF local_new_rect_tf=this->mapFromScene(sceneBoundingRect().translated(m_speed_x*5, m_speed_y*5).topLeft());
    QPointF local_new_rect_br=this->mapFromScene(sceneBoundingRect().translated(m_speed_x*5, m_speed_y*5).bottomRight());
//    QPointF local_new_rect_tf=this->mapFromScene(sceneBoundingRect().translated(m_speed_x, m_speed_y).topLeft());
//    QPointF local_new_rect_br=this->mapFromScene(sceneBoundingRect().translated(m_speed_x, m_speed_y).bottomRight());

    qreal left = fmin(local_prev_rect_tl.x(), local_new_rect_tf.x());//+10;
    qreal right= fmax(local_prev_rect_br.x(), local_new_rect_br.x());//-10;
    qreal top = fmin(local_prev_rect_tl.y(), local_new_rect_tf.y());//+3;
    qreal bottom = fmax(local_prev_rect_br.y(), local_new_rect_br.y());//-3;

//    qDebug()<<left<<right<<top<<bottom;
//    if (left>right){
//        qreal tmp=right;
//        right=left;
//        left=tmp;
//    }



    QRectF cov_rect=QRectF(left, top, right-left, bottom-top);//boundingRect().translated(m_speed_x, m_speed_y);
//    QGraphicsRectItem coverage_rect=QGraphicsRectItem(cov_rect, this);
    coverage_rect.setRect(cov_rect);*/


    /*if (m_type == CharacterType::Player){
        colliding_rects.append(QRectF(159,580.999,39,59));
    }*/

    // TMP
    QVector<QRectF> colliding_rects=m_platform.getCollidingRects(m_collision_rect.m_old_rect, sceneBoundingRect().translated(m_speed_x, m_speed_y));
//    qDebug()<<m_speed_x<<m_speed_y<<m_collision_rect.m_old_rect<< getBoundingRect().translated(m_speed_x, m_speed_y)<<sceneBoundingRect();
//    QRectF res=m_collision_handler.handle(m_collision_rect.m_old_rect, sceneBoundingRect().translated(m_speed_x, m_speed_y),colliding_rects);

//    qDebug()<<coverage_rect.sceneBoundingRect()<<cov_rect;
//    this->scene()->addItem(&coverage_rect);
    /*for (QGraphicsItem *item : coverage_rect.collidingItems())
    {
        if (item->data(0) == "Enemy" && m_type!=CharacterType::Enemy || item->data(0) == "Player" && m_type!=CharacterType::Player)
        {
//            qDebug()<<"Enemy";
            Character *chara = static_cast<Character *>(item);
//            qDebug()<<chara->sceneBoundingRect();
//            chara->shape().intersected(coverage_rect).
            colliding_rects.append(chara->sceneBoundingRect());

//            this->m_collision_rect.handleCollision(chara->m_collision_rect);


        }
    }*/

//    qDebug()<<colliding_rects;

//    QRectF res=m_collision_handler.handle(m_collision_rect.m_old_rect, sceneBoundingRect().translated(m_speed_x, m_speed_y),colliding_rects);//getBoundingRect().translated(m_speed_x, m_speed_y);//

//    m_collision_rect.m_new_rect=res; // Speed after static collision
//    QRectF cov_rect= QRectF(left, top, right-left, bottom-top);//boundingRect().translated(m_speed_x, m_speed_y);
    coverage_rect.setRect(QRectF(-250,-250,500,500));
    for (QGraphicsItem *item : coverage_rect.collidingItems())
    {
        if (item->data(0) == "Enemy" && m_type!=CharacterType::Enemy || item->data(0) == "Player" && m_type!=CharacterType::Player)
        {
            //            qDebug()<<"Enemy";
            Character *chara = static_cast<Character *>(item);
            this->m_collision_rect.handleCollision(chara->m_collision_rect);


        }else if (item->data(0) =="Tile"){
            Tile *tile = static_cast<Tile *>(item);

            if (tile->isSolid()){
//               qDebug()<<"Tile";
                this->m_collision_rect.handleCollision(tile->m_collision_rect);
            }


        }
    }

    QRectF res=m_collision_rect.m_new_rect.translated(m_collision_rect.m_speed_x,m_collision_rect.m_speed_y);
    m_speed_x=m_collision_rect.m_speed_x;
    m_speed_y=m_collision_rect.m_speed_y;

    /*m_speed_x = res.center().x() - sceneBoundingRect().center().x();
    m_speed_y = res.center().y() - sceneBoundingRect().center().y();
    qDebug()<<m_speed_x<<m_speed_y;*/

//    this->moveBy(m_speed_x, m_speed_y);



//    this->shape().

    /*for (QGraphicsItem *item : this->collidingItems())
    {
        if (item->data(0) == "Weapon")
        {
            Weapon *weapon = static_cast<Weapon *>(item);
            if (weapon->isActive())
            {
                m_state = CharacterState::Hit;

                // Note: Weapon rect + pos -> shape
                QPointF diff_pos = this->sceneBoundingRect().center() - weapon->sceneBoundingRect().center();
                qreal dir_x = 1;
                qreal dir_y = -1;

                if (diff_pos.x() < 0)
                {
                    dir_x = -1;
                }

                if (diff_pos.y() > 0)
                {
                    dir_y = 1;
                }

                m_speed_x += weapon->getPowerX() * dir_x;

                m_speed_y += weapon->getPowerY() * dir_y;
            }
        }else if (item->data(0) == "Enemy")
        {
//            qDebug()<<"Enemy";
//            Character *chara = static_cast<Character *>(item);
//            colliding_rects.append(chara->sceneBoundingRect());
        }

    }*/

    /*if (m_type == CharacterType::Player){
        qDebug()<<colliding_rects;
    }*/

//    QRectF res=m_collision_handler.handle(prev_rect, sceneBoundingRect().translated(m_speed_x, m_speed_y),colliding_rects);



//    res = m_platform.handleCollision(prev_rect,res);//sceneBoundingRect().translated(m_speed_x, m_speed_y));//

    /*m_speed_x = res.center().x() - sceneBoundingRect().center().x();
    m_speed_y = res.center().y() - sceneBoundingRect().center().y();

    if ((0 < m_speed_x && m_speed_x <= 1e-2) || (0 > m_speed_x && m_speed_x >= -1e-2))
    {
        m_speed_x = 0;
    }

    if ((0 < m_speed_y && m_speed_y <= 1e-2) || (0 > m_speed_y && m_speed_y >= -1e-2))
    {
        m_speed_y = 0;
    }

    if (m_speed_x > 1)
    {
        this->setTransform(QTransform().scale(1, 1));
        m_direction = CharacterDirection::Right;
    }
    else if (m_speed_x < -1 || m_direction == CharacterDirection::Left)
    {
//        this->setTransform(QTransform().scale(-1, 1).translate(-sceneBoundingRect().width(), 0)); ////////// Width change ...
//        coverage_rect.setTransform(QTransform().scale(-1, 1));
        m_direction = CharacterDirection::Left;
    }*/

    ////////////////////////////////////////////////////////////////////////////

    QPointF scene_adjustmnt = m_bounding_rect.topLeft();
//    if (m_direction == CharacterDirection::Left)
//    {
//        scene_adjustmnt.setX(-scene_adjustmnt.x());
//    }

    this->setPos(res.topLeft()- scene_adjustmnt);//0,0);//res.topLeft());//

//    this->moveBy(m_speed_x,m_speed_y);
    updateAnimation();
}

void Character::updateAnimation()
{
    m_animation->setId(static_cast<uint8_t>(m_state));
}

bool Character::isOnGround()
{
    return m_state == CharacterState::Ground | m_state == CharacterState::Idle | m_state == CharacterState::Run;
}

/*QRectF Character::getBoundingRect()
{
    QRectF res= m_bounding_rect;

    res.moveTopLeft(pos());

//    qDebug()<<res<<sceneBoundingRect();

    return res;
}*/
