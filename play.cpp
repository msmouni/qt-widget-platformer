#include "play.h"

PlayView::PlayView(QWidget* parent) : QGraphicsView(parent)
{
   // Create a QGraphicsScene
   scene = new QGraphicsScene(this);
   this->setScene(scene);

   //        scene->

   // Load an image and add it to the scene
   QPixmap pixmap("/home/marwan/Projects/Qt/First_game_v2/First_game_v2/ressources/background.jpg");
   imageItem = scene->addPixmap(pixmap);

   //        imageItem->setScale(2.0);

   // Create a movable rectangle and add it to the scene
   redItem = new Character(QRectF(100, 100, 50, 50), Qt::red);//new QGraphicsRectItem(100, 100, 50, 50);
//           redItem->setPos(100, 100);
//   redItem->setBrush(QColor(255, 0, 0));  // Red color
   scene->addItem(redItem);

   blueItem = new QGraphicsRectItem(-50, -100, 100, 200); // we use center
   blueItem->setPos(150,300); // Relative position to the one at which it was created (center)
   qDebug()<<"HERRE B"<<blueItem->pos();
   blueItem->setBrush(QColor(0, 0, 255));  // Blue color
   // add to group later
   blueItem->setData(0,"Platform");
   scene->addItem(blueItem);

   /*QRectF recPos=redItem->sceneBoundingRect();//>mapRectToScene(;//mapToScene(redItem->rect());
qDebug()<<recPos;
this->setSceneRect(QRectF(recPos.x()-recPos.width(),recPos.y()-recPos.height(),recPos.width()*3,recPos.height()*3));*/
   //        this->scene->setFocusItem(rectItem);
   //        centerOn(QPointF(500,500));//rectItem);
   //        qDebug()<<"HERE"<<this->rect();

   /////////////////////////////////////////////////// NOTE: To centerOn Item see example below (keep scene size, and set view geometry (.<scene size)  and remove scroll)

   // QGraphicsView can be used to visualize a whole scene, or only parts of it.

   //        redItem->pos()

   //        this->

   isMoving = false;
}


void PlayView::mousePressEvent(QMouseEvent* event)
{
   if (event->button() == Qt::LeftButton)
   {
       //            qDebug()<<redItem->contains( redItem->mapFromScene(mapToScene(event->pos())));

       //            qDebug()<<"mouse"<<event->pos()<<"map_parent"<<redItem->mapFromParent(event->pos())<<"map_scene"<<redItem->mapFromScene(mapToScene(event->pos()));
       //            if (redItem->contains(redItem->mapFromParent(event->pos())) ){
       if (redItem->contains(redItem->mapFromScene(mapToScene(event->pos())))){
           // Start tracking the position for dragging
           lastMousePos = mapToScene(event->pos());
           isMoving = true;
       }

   }
}

void PlayView::mouseReleaseEvent(QMouseEvent* event)
{
   if (event->button() == Qt::LeftButton)
   {
       isMoving = false;
   }
}

void PlayView::mouseMoveEvent(QMouseEvent* event)
{
   if (isMoving)
   {
       // Calculate the movement offset
       QPointF newMousePos = mapToScene(event->pos());
       QPointF delta = newMousePos - lastMousePos;

       // Move the rectangle
       redItem->moveBy(delta.x(), delta.y());

       QRectF recPos=redItem->sceneBoundingRect();

       QRectF rect_test=QRectF(recPos.x()-recPos.width(),recPos.y()-recPos.height(),recPos.width()*3,recPos.height()*3);

       //            rect_test.

       /*this->scene->setSceneRect(rect_test);
            this->setSceneRect(rect_test);*/
       ////            this->centerOn(rectItem);

       //            qDebug()<<"recPos"<<recPos<<"newMousePos"<<newMousePos<<"scene rect"<<scene->sceneRect()<<"rect_test"<<rect_test;
       lastMousePos = newMousePos;
   }
}

/*void PlayView::keyPressEvent(QKeyEvent* event)
{

   qreal step = 10.0; // Adjust the step size as needed.

   int dx=0,dy=0;

   if (event->key() == Qt::Key_Left)
       //            redItem->moveBy(-step, 0);
       dx=-step;
   else if (event->key() == Qt::Key_Right)
       //            redItem->moveBy(step, 0);
       dx=step;
   else if (event->key() == Qt::Key_Up){
       //            redItem->moveBy(0, -step);
       dy=-step;
        redItem->jump();
   }else if (event->key() == Qt::Key_Down)
       //            redItem->moveBy(0, step);
       dy=step;

   // Move to Character::advance
   /*redItem->moveBy(dx, dy); // maybe create a other object so we don't move this
   QList<QGraphicsItem *> collidingItems=redItem->collidingItems();


   // Handle collisions with other items
   foreach (QGraphicsItem *item, collidingItems) {
       // Check if item is not itself
       if (item->data(0) == "Enemy") {

           QRectF redRect=redItem->sceneBoundingRect();//boundingRect();
           //                redRect.translate(dx,dy);
           QRectF otherRect=item->sceneBoundingRect();// >boundingRect();

           QRectF interRect=redRect.intersected(otherRect);

           //                qDebug()<<"Check"<<"redRect"<<redRect<<"otherRect"<<otherRect<<"interRect"<<interRect;

           if (dx > 0){
               dx=-interRect.width()-1;
           }else if (dx < 0){
               dx=interRect.width()+1;
           }

           if (dy>0){
               dy=-interRect.height()-1;
           }else if (dy<0){
               dy=interRect.height()+1;
           }

           // TODO: handle both dx+dy
       }
   }

   redItem->moveBy(dx, dy);

   this->centerOn(redItem);

}*/

void PlayView::keyPressEvent(QKeyEvent* event)
{
   if (event->key() == Qt::Key_Left){
       redItem->left_pressed();
   }
   else if (event->key() == Qt::Key_Right){
        redItem->right_pressed();
   }
   else if (event->key() == Qt::Key_Up){
        redItem->up_pressed();
   }else if (event->key() == Qt::Key_Down){
        redItem->down_pressed();
   }

}

void PlayView::keyReleaseEvent(QKeyEvent *event)
{
   if (event->key() == Qt::Key_Left){
        redItem->left_released();
   }
   else if (event->key() == Qt::Key_Right){
        redItem->right_released();
   }
   else if (event->key() == Qt::Key_Up){
        redItem->up_released();
   }else if (event->key() == Qt::Key_Down){
        redItem->down_released();
   }

}

void PlayView::resizeEvent(QResizeEvent *event) {

   QSizeF size=event->size();
   QSizeF old_size=event->oldSize();

   scene->setSceneRect(0,0,size.width(),size.height());

   if (old_size != QSize(-1,-1)){
       // Size
       qreal scale = std::max(size.height()/m_init_height,size.width()/m_init_width); // scale regarding initial size

       imageItem->setScale(scale);
       redItem->setScale(scale);
       blueItem->setScale(scale);

       // Position
       redItem->setPos(redItem->pos()*(scale/m_old_scale)); // size / old_size : max ...
       blueItem->setPos(blueItem->pos()*(scale/m_old_scale));
       m_old_scale=scale;

   }else {
       //            m_init_size=size;
       m_init_height=size.height();
       m_init_width=size.width();
       m_init_pos=redItem->pos();
   }

   /*QRectF recPos=redItem->sceneBoundingRect();
        this->setSceneRect(QRectF(recPos.x()-recPos.width(),recPos.y()-recPos.height(),recPos.width()*3,recPos.height()*3));*/
   //        this->centerOn(rectItem);


}

