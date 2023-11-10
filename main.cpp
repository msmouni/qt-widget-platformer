#include <QApplication>
#include "app.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

//    PlayView game;

//    game.setWindowTitle("Qt platformer");
//    game.show();

    MainApp game;
    game.show();
    game.setVisible(true);

    return app.exec();
}
