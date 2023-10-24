#include "app.h"
#include "./ui_app.h"

MainApp::MainApp(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainApp)
{
    ui->setupUi(this);
}

MainApp::~MainApp()
{
    delete ui;
}

