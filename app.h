#ifndef MAINAPP_H
#define MAINAPP_H

#include <QWidget>
#include <QGraphicsScene>


QT_BEGIN_NAMESPACE
namespace Ui { class MainApp; }
QT_END_NAMESPACE

class MainApp : public QWidget
{
    Q_OBJECT

public:
    MainApp(QWidget *parent = nullptr);
    ~MainApp();

private:
    Ui::MainApp *ui;
    QGraphicsScene m_scene;
};
#endif // MAINAPP_H
