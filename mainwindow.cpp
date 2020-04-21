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
    menu->addAction(QIcon(":/icons/run"), "开始播放", [=]{
        qDebug() << "=>开始播放";
    });
    menu->addAction(QIcon(":/icons/pause"), "暂停", [=]{
        qDebug() << "=>暂停";
    });
    menu->addAction(QIcon(":/icons/faster"), "加速", [=]{
        qDebug() << "=>加速";
    });
    menu->addAction(QIcon(":/icons/slower"), "减速", [=]{
        qDebug() << "=>减速";
    });
    auto subMenu = menu->addMenu("子菜单", [=]{});

    menu->execute(QCursor::pos());
}
