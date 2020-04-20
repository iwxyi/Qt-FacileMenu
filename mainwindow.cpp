#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    FacileMenu* menu = new FacileMenu(this);
    menu->addAction(QIcon(), "测试", [=]{
        qDebug() << "11111111111";
    });
    menu->execute(QCursor::pos());
}
