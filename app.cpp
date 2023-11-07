#include "app.h"
#include "play.h"
#include "./ui_app.h"


MainApp::MainApp(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainApp)
{
    ui->setupUi(this);

////    ui->playView->setBackgroundBrush(QPixmap("/home/marwan/Projects/Qt/First_game_v2/First_game_v2/ressources/sprites/enemy.png"));
////    ui->playView->show();
//    ///////////////
////    m_scene.setSceneRect(-300, -300, 600, 600);
//    //! [1] //! [2]
//    m_scene.setItemIndexMethod(QGraphicsScene::NoIndex);

//    ui->playView->setv ->setScene(&m_scene);

//    //! [4]
//    ui->playView->setRenderHint(QPainter::Antialiasing);
//    // Load a QPixmap from an image file
//    QPixmap pixmap("/home/marwan/Projects/Qt/First_game_v2/First_game_v2/ressources/background.jpg");

//    // Set the texture mode to stretch the pixmap
//    QBrush brush(pixmap);
//    brush.setTexture(QPixmap(pixmap));
////    brush.setStyle(Qt::BrushStyle::);
////    brush.setStyle(Qt::BrushStyle::):
////    brush.setTransform(QTransform .setTextureMode(Qt:: TextureMode::TextureMode_Stretch);
//    ui->playView->setBackgroundBrush(QPixmap("/home/marwan/Projects/Qt/First_game_v2/First_game_v2/ressources/background.jpg"));
//    //! [4] //! [5]
////    ui->playView->setCacheMode(QGraphicsView::CacheBackground);
////    ui->playView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
////    ui->playView->setDragMode(QGraphicsView::ScrollHandDrag);
//    //! [5] //! [6]
////    ui->playView->setWindowTitle(QT_TRANSLATE_NOOP(QGraphicsView, "Colliding Mice"));
////    ui->playView->resize(400, 300);
////    ui->playView->show();
}

MainApp::~MainApp()
{
    delete ui;
}

