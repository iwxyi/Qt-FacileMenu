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

    FacileMenu* menu = (new FacileMenu(this))
        ->setTipArea("Ctrl+P")
        ->setSplitInRow(true);

    menu->addAction(QIcon(":/icons/run"), "开始播放 (&S)", [=]{
        qDebug() << "=>开始播放";
    })->tip("Ctrl+S");

    menu->addAction(QIcon(":/icons/pause"), "暂停 (&P)", [=]{
        qDebug() << "=>暂停";
    })->tip("Ctrl+P")->suffix(true, "☆");

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
    })->check(faster_checked)->linger()
            ->ifer(false)
            ->prefix("1")
            ->elser()
            ->prefix("2");

    int number = 3;
    menu->addAction(QIcon(":/icons/slower"), "减速", [=]{
        qDebug() << "=>减速";
        slower_checked = !slower_checked;
    })->check(slower_checked)->linger()
            ->switcher(number)
            ->caser(1)
            ->suffix("1")
            ->breaker()
            ->caser(2)
            ->suffix("2")
            // ->breaker()
            ->caser(3)
            ->suffix(" caser")
            ->breaker()
            ->caser(4)
            ->suffix("4")
            ->breaker()
            ->caser(6, [=]{ qDebug() << "(^_−)☆"; })
            ->defaulter()
            ->suffix("5")
            ->breaker();

    menu->addRow([=]{
        menu->addAction("按钮1");
        menu->addAction("按钮2")->borderR();
        menu->addAction("按钮3");
    });

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

    auto subMenu2 = menu->addMenu("可选菜单");
    {
        auto ac1 = subMenu2->addAction(QIcon(":/icons/run"), "选中1")->check()->linger();
        auto ac2 = subMenu2->addAction("选中2")->uncheck()->linger();
        auto ac3 = subMenu2->addAction("选中3")->uncheck()->linger();

        // 点击事件
        ac1->triggered([=]{
            subMenu2->uncheckAll(ac1);
        });
        ac2->triggered([=]{
            subMenu2->uncheckAll(ac2);
        });
        ac3->triggered([=]{
            subMenu2->uncheckAll(ac3);
        });
    }

    auto subMenu3 = menu->addMenu("判断语句");
    {
        static bool b = false;
        subMenu3->addAction("if else 判断")
                ->tooltip("试一下 ifer elser")
                ->triggered([=]{b = !b;})
                ->ifer(b)
                ->check()
                ->elser()
                ->uncheck();
    }

    auto subMenu4 = menu->addMenu("批量菜单");
    QStringList texts;
    for (int i = 0; i < 10; i++)
        texts << "项目"+QString::number(i);
    static int selected = 2;
    subMenu4->addOptions(texts, selected, [=](int index){
        qDebug() << "选中了：" << (selected = index);
    });

    menu->exec(QCursor::pos());
}

void MainWindow::on_pushButton_2_clicked()
{
    static bool faster_checked = true;
    static bool slower_checked = false;

    FacileMenu* menu = new FacileMenu(this);
    menu->setTipArea("Ctrl+P");

    menu->addRow([=]{
        menu->addAction("按钮1")->disable();
        menu->addAction("按钮2")->disable();
        menu->addAction("按钮3")->text(true, "按钮3 (&K)");
    });

    menu->addAction(QIcon(":/icons/run"), "开始播放 (&S)", [=]{
        qDebug() << "=>开始播放";
    })->tip("Ctrl+S");

    menu->addAction(QIcon(":/icons/pause"), "暂停 (&P)", [=]{
        qDebug() << "=>暂停";
    })->tip("Ctrl+P");

    QPushButton* button = new QPushButton("外部添加的按钮", this);
    menu->addWidget(button);

    QHBoxLayout* btn_hlayout = new QHBoxLayout;
    QPushButton* btn1= new QPushButton("按钮1", this);
    QPushButton* btn2 = new QPushButton("按钮2", this);
    btn_hlayout->addSpacing(40);
    btn_hlayout->addWidget(btn1);
    btn_hlayout->addItem(new QSpacerItem(4, 0));
    btn_hlayout->addWidget(btn2);
    menu->addLayout(btn_hlayout);

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

    menu->beginRow();
    menu->addAction(QIcon(":/icons/run"));
    menu->addAction(QIcon(":/icons/pause"));
    menu->split()->addAction(QIcon(":/icons/resume"));
    menu->addAction(QIcon(":/icons/stop"))->disable();
    menu->endRow();

    menu->addAction(QIcon(":/icons/faster"), "加速", [=]{
        qDebug() << "=>加速";
        faster_checked = !faster_checked;
    })->setChecked(faster_checked)->disable();

    menu->addAction(QIcon(":/icons/slower"), "减速", [=]{
        qDebug() << "=>减速";
        slower_checked = !slower_checked;
    })->setChecked(slower_checked)->disable();

    menu->beginRow();
    menu->addAction("按钮1");
    menu->addAction("按钮2")->disable();
    menu->addAction("按钮3")->disable();
    menu->endRow();

    menu->exec(QRect(mapToGlobal(ui->pushButton_2->pos()), ui->pushButton_2->size()), true, QCursor::pos());
}

void MainWindow::staticFunction()
{
    qDebug() << "静态方法回调";
}

void MainWindow::classFunction()
{
    qDebug() << "类内方法回调";
}
