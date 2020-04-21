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
    menu->addAction(QIcon(":/icons/resume"), "继续", [=]{
        qDebug() << "=>继续";
    })->disable();
    menu->addAction(QIcon(":/icons/stop"), "停止", [=]{
        qDebug() << "=>停止";
    });
    menu->addAction(QIcon(":/icons/faster"), "加速", [=]{
        qDebug() << "=>加速";
        faster_checked = !faster_checked;
    })->setChecked(faster_checked);
    menu->addAction(QIcon(":/icons/slower"), "减速", [=]{
        qDebug() << "=>减速";
        slower_checked = !slower_checked;
    })->setChecked(slower_checked);
    auto subMenu = menu->addMenu("子菜单", [=]{});

    menu->execute(QCursor::pos());
}
