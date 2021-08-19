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
        ->setSplitInRow(true)
        ->setSubMenuShowOnCursor(false);

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

    auto subMenu2 = menu->addMenu("单选菜单");
    {
        auto ac1 = subMenu2->addAction(QIcon(":/icons/run"), "带图标")->check()->linger();
        auto ac2 = subMenu2->addAction("无图标")->uncheck()->linger();
        auto ac3 = subMenu2->split()->addAction("全不选")->uncheck()->linger();

        // 连接点击事件
        ac1->triggered([=]{
            subMenu2->singleCheck(ac1); // 用于单选，表示只选中ac1
            // 这里可以用于处理其他操作
        });
        ac2->triggered([=]{
            subMenu2->singleCheck(ac2);
        });
        ac3->triggered([=]{
            subMenu2->uncheckAll(); // 全不选
        });
    }

    auto subMenu4 = menu->addMenu("快速批量单选项");
    {
        QStringList texts;
        QList<int> values;
        for (int i = 0; i < 10; i++)
        {
            int val = qrand() % 1000;
            texts << "项目"+QString::number(val);
            values << val;
        }
        static int selected = 2;

        subMenu4->addOptions(texts, selected, [=](int index){
            qDebug() << "选中了：" << (selected = index) << "，值：" << values.at(index);
        });
        // 这里不建议（也没有必要）修改checked状态，因为点了就隐藏掉了
    }

    auto subMenu5 = menu->addMenu("多选菜单");
    {
        // 假装是某一个需要多选的属性
        QList<int>* list = new QList<int>();

        for (int i = 0; i < 10; i++)
        {
            int val = qrand() % 1000; // 放入菜单项的自定义数据
            auto action = subMenu5->addAction("选项"+QString::number(val))->setData(val)->uncheck()->autoToggle()->linger();
            action->triggered([=]{
                // 自己的处理流程，例如调用某个外部的方法
                if (action->isChecked())
                    list->append(action->getData().toInt());
                else
                    list->removeOne(action->getData().toInt());
                qDebug() << "当前选中的有：" << *list;
            });
        }

        subMenu5->finished([=]{
            if (list->size())
                qDebug() << "多选 最终选中：" << *list;
            delete list;
        });
    }

    auto subMenu6 = menu->addMenu("快速批量多选项");
    {
        // 假装是某一个需要多选的属性
        QList<int>* list = new QList<int>();
        subMenu6->addNumberedActions("选项%1", 0, 10)
            ->setMultiCheck([=](int index, bool checked){
                if (checked)
                    list->append(index);
                else
                    list->removeOne(index);
                qDebug() << "当前选中的有：" << *list;
            });
    }

    auto subMenu7 = menu->addMenu("极简批量多选项");
    {
        QList<QString>* list = new QList<QString>();
        subMenu7->addNumberedActions("选项%1", 0, 15)->setMultiCheck()
                ->finished([=]{
            *list = subMenu7->checkedItemTexts();
            if (list->size())
                qDebug() << "极简多选项 最终选中：" << *list;
        });
    }

    auto subMenu8 = menu->addMenu("批量数字项");
    {
        subMenu8->addNumberedActions("选项%1", 3, 13, [&](FacileMenuItem* item, int i){
            item->setChecked(i%5==0)->linger()->autoToggle();
        })->finished([=]{
            auto list = subMenu8->checkedItemTexts();
            if (list.size() != 2)
                qDebug() << "批量数字项 最终选中：" << list;
        });
    }

    auto subMenu3 = menu->addMenu("判断语句");
    {
        static bool b = false;
        subMenu3->addAction("if else 判断")
                ->tooltip("试一下 ifer elser")
                ->text(b, "当前：if", "当前：else")
                ->triggered([=]{b = !b;})
                ->ifer(b)
                ->check()
                ->elser()
                ->uncheck();
    }

    // 导入QMenu和QAction
    {
        QMenu* m = new QMenu("QMenu菜单", this);
        QAction* action1 = new QAction("子action1", this);
        QAction* action2 = new QAction("子action2", this);
        QAction* action3 = new QAction("子action3", this);
        m->addAction(action1);
        m->addAction(action2);
        m->addAction(action3);
        QMenu* m2 = new QMenu("QMenu菜单2", this);
        QAction* action21 = new QAction("子action21", this);
        QAction* action22 = new QAction("子action22", this);
        QAction* action23 = new QAction("子action23", this);
        m2->addAction(action21);
        m2->addAction(action22);
        m2->addAction(action23);
        action2->setMenu(m2);
        connect(action1, &QAction::triggered, this, [=]{ qDebug() << "action1.triggered"; });
        connect(action21, &QAction::triggered, this, [=]{ qDebug() << "action21.triggered"; });

        menu->addMenu(m);
    }

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
    })->tip("Ctrl+S")->fgColor(Qt::blue);

    menu->addAction(QIcon(":/icons/pause"), "暂停 (&P)", [=]{
        qDebug() << "=>暂停";
    })->tip("Ctrl+P")->bgColor(true, Qt::gray);

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

void MainWindow::on_pushButton_3_clicked()
{

    FacileMenu* menu = (new FacileMenu(this));

    for (int i = 0; i < 80; i++)
    {
        menu->addAction("这是一个菜单" + QString::number(i));
    }

    menu->exec(QCursor::pos());
}
