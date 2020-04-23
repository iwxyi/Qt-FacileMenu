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
    menu->addTipArea("Ctrl+P");

    menu->addAction(QIcon(":/icons/run"), "开始播放 (&S)", [=]{
        qDebug() << "=>开始播放";
    })->tip("Ctrl+S");

    menu->addAction(QIcon(":/icons/pause"), "暂停 (&P)", [=]{
        qDebug() << "=>暂停";
    })->tip("Ctrl+P");

    menu->addAction(QIcon(":/icons/resume"), "继续 (&R)", [=]{
        qDebug() << "=>继续";
    })->disable()->tip("Ctrl+R");

    menu->addAction(QIcon(":/icons/stop"), "停止 (&T)", [=]{
        qDebug() << "=>停止";
    })->tip("Ctrl+T");
    menu->addSeparator();

    menu->addAction(QIcon(":/icons/faster"), "加速", [=]{
        qDebug() << "=>加速";
        faster_checked = !faster_checked;
    })->setChecked(faster_checked);

    menu->addAction(QIcon(":/icons/slower"), "减速", [=]{
        qDebug() << "=>减速";
        slower_checked = !slower_checked;
    })->setChecked(slower_checked);

    menu->addChipLayout();
    menu->addChip("按钮1");
    menu->addChip("按钮2");
    menu->addChip("按钮3");

    auto subMenu = menu->addMenu("子菜单0");
    {
        subMenu->addAction(QIcon(":/icons/run"), "开始播放", [=]{
            qDebug() << "=>开始播放";
        });

        subMenu->addAction(QIcon(":/icons/pause"), "暂停", [=]{
            qDebug() << "=>暂停";
        });

        subMenu->addAction(QIcon(":/icons/resume"), "继续", [=]{
            qDebug() << "=>继续";
        })->disable();

        subMenu->addAction(QIcon(":/icons/stop"), "停止", [=]{
            qDebug() << "=>停止";
        });

        subMenu->addAction(QIcon(":/icons/faster"), "加速", [=]{
            qDebug() << "=>加速";
            faster_checked = !faster_checked;
        })->setChecked(faster_checked);

        auto subMenu2 = subMenu->addMenu("子菜单");
        {
            subMenu2->addAction(QIcon(":/icons/run"), "开始播放", [=]{
                qDebug() << "=>开始播放";
            });

            subMenu2->addAction(QIcon(":/icons/pause"), "暂停", [=]{
                qDebug() << "=>暂停";
            });

            subMenu2->addAction(QIcon(":/icons/resume"), "继续", [=]{
                qDebug() << "=>继续";
            })->disable();

            subMenu2->addAction(QIcon(":/icons/stop"), "停止", [=]{
                qDebug() << "=>停止";
            });

            subMenu2->addAction(QIcon(":/icons/faster"), "加速", [=]{
                qDebug() << "=>加速";
                faster_checked = !faster_checked;
            })->setChecked(faster_checked);

            auto subMenu3 = subMenu2->addMenu("子菜单");
            subMenu3->addAction("没有子菜单了");
        }
    }

    auto subMenu2 = menu->addMenu("子菜单1");
    {
        subMenu2->addAction(QIcon(":/icons/run"), "开始播放", [=]{
            qDebug() << "=>开始播放";
        });

        subMenu2->addAction(QIcon(":/icons/pause"), "暂停", [=]{
            qDebug() << "=>暂停";
        });
    }

    auto subMenu3 = menu->addMenu("子菜单2");
    {

        subMenu3->addAction(QIcon(":/icons/resume"), "继续", [=]{
            qDebug() << "=>继续";
        })->disable();

        subMenu3->addAction(QIcon(":/icons/stop"), "停止", [=]{
            qDebug() << "=>停止";
        });
    }

    auto subMenu4 = menu->addMenu("子菜单3");
    {
        subMenu4->addAction(QIcon(":/icons/faster"), "加速", [=]{
            qDebug() << "=>加速";
            faster_checked = !faster_checked;
        })->setChecked(faster_checked);
    }


    menu->exec(QCursor::pos());
}

void MainWindow::on_pushButton_2_clicked()
{
    static bool faster_checked = true;
    static bool slower_checked = false;

    FacileMenu* menu = new FacileMenu(this);
    menu->addTipArea("Ctrl+P");

    menu->addChipLayout();
    menu->addChip("按钮1")->disable();
    menu->addChip("按钮2")->disable();
    menu->addChip("按钮3");

    menu->addAction(QIcon(":/icons/run"), "开始播放 (&S)", [=]{
        qDebug() << "=>开始播放";
    })->tip("Ctrl+S");

    menu->addAction(QIcon(":/icons/pause"), "暂停 (&P)", [=]{
        qDebug() << "=>暂停";
    })->tip("Ctrl+P");

    menu->addAction(QIcon(":/icons/resume"), "继续 (&R)", [=]{
        qDebug() << "=>继续";
    })->disable()->tip("Ctrl+R");

    menu->addAction(QIcon(":/icons/stop"), "停止 (&T)", [=]{
        qDebug() << "=>停止";
    })->tip("Ctrl+T");
    menu->addSeparator();

    menu->addAction(QIcon(), "静态回调方法", staticFunction);
//    menu->fun2(1, "类内回调方法", this, &MainWindow::classFunction);

    menu->addAction(QIcon(":/icons/faster"), "加速", [=]{
        qDebug() << "=>加速";
        faster_checked = !faster_checked;
    })->setChecked(faster_checked);

    menu->addAction(QIcon(":/icons/slower"), "减速", [=]{
        qDebug() << "=>减速";
        slower_checked = !slower_checked;
    })->setChecked(slower_checked);

    menu->addChipLayout();
    menu->addChip(QIcon(":/icons/run"));
    menu->addChip(QIcon(":/icons/pause"));
    menu->addChip(QIcon(":/icons/resume"));
    menu->addChip(QIcon(":/icons/stop"))->disable();

    menu->addAction(QIcon(":/icons/faster"), "加速", [=]{
        qDebug() << "=>加速";
        faster_checked = !faster_checked;
    })->setChecked(faster_checked)->disable();

    menu->addAction(QIcon(":/icons/slower"), "减速", [=]{
        qDebug() << "=>减速";
        slower_checked = !slower_checked;
    })->setChecked(slower_checked)->disable();

    menu->addChipLayout();
    menu->addChip("按钮1");
    menu->addChip("按钮2")->disable();
    menu->addChip("按钮3")->disable();


    menu->exec(QCursor::pos());
}

void MainWindow::staticFunction()
{
    qDebug() << "静态方法回调";
}

void MainWindow::classFunction()
{
    qDebug() << "类内方法回调";
}
