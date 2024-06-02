#include "app.h"
#include "./ui_app.h"

MainApp::MainApp(QWidget *parent)
    : QWidget(parent), ui(new Ui::MainApp)
{
    ui->setupUi(this);

    ui->stackedWidget->setCurrentWidget(ui->play_page);
}

MainApp::~MainApp()
{
    delete ui;
}
