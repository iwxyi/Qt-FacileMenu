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
    static bool faster_checked = true;
    static bool slower_checked = false;

    FacileMenu* menu = new FacileMenu(this);
    menu->addAction(QIcon(":/icons/run"), "开始播放", [=]{
        qDebug() << "=>开始播放";
    });
    menu->addAction(QIcon(":/icons/pause"), "暂停", [=]{
        qDebug() << "=>暂停";
    });
    menu->addAction(QIcon(":/icons/faster"), "加速", faster_checked, [=]{
        qDebug() << "=>加速";
        faster_checked = !faster_checked;
    });
    menu->addAction(QIcon(":/icons/slower"), "减速", slower_checked, [=]{
        qDebug() << "=>减速";
        slower_checked = !slower_checked;
    });
    auto subMenu = menu->addMenu("子菜单", [=]{});

    menu->execute(QCursor::pos());
}
