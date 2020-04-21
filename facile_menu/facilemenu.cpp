#include "facilemenu.h"

QColor FacileMenu::normal_bg = QColor(238, 238, 238);
QColor FacileMenu::hover_bg = QColor(64, 64, 64, 64);
QColor FacileMenu::press_bg = QColor(128, 128, 128, 128);
QColor FacileMenu::text_fg = QColor(0, 0, 0);

FacileMenu::FacileMenu(QWidget *parent) : QWidget(parent)
{
    setObjectName("FacileMenu");
    setAttribute(Qt::WA_DeleteOnClose, true);
    setFocusPolicy(Qt::StrongFocus); // 获取焦点，允许按键点击
    setWindowFlag(Qt::Popup, true);

    main_vlayout = new QVBoxLayout(this);
    setLayout(main_vlayout);
    main_vlayout->setEnabled(true);
    main_vlayout->setMargin(0);
    main_vlayout->setSpacing(0);
}

FacileMenu::FacileMenu(bool sub, QWidget *parent) : FacileMenu(parent)
{
    _is_sub_menu = sub;
    setAttribute(Qt::WA_DeleteOnClose, false);
}

FacileMenuItem *FacileMenu::addAction(QIcon icon, QString text, FuncType func)
{
    FacileMenuItem* item = new FacileMenuItem(icon, text, this);
    item->setKey(getShortcutByText(text));

    setActionButton(item);
    main_vlayout->addWidget(item);
    items.append(item);

    connect(item, &InteractiveButtonBase::clicked, this, [=]{
        func();
        close();
    });
    connect(item, &InteractiveButtonBase::signalMouseEnter, this, [=]{
       if (current_sub_menu)
       {
           current_sub_menu->hide();
           current_sub_menu = nullptr;
       }
    });
    return item;
}

FacileMenuItem *FacileMenu::addAction(QString text, FuncType func)
{
    return addAction(QIcon(), text, func);
}

/**
 * 添加一项子菜单
 * 鼠标浮在上面展开
 * 同时也可以设置点击事件
 */
FacileMenu *FacileMenu::addMenu(QIcon icon, QString text, FuncType func)
{
    FacileMenuItem* item = new FacileMenuItem(icon, text, this);
    item->setKey(getShortcutByText(text));

    setActionButton(item);
    main_vlayout->addWidget(item);
    items.append(item);

    connect(item, &InteractiveButtonBase::clicked, this, [=]{
        func();
        close();
    });
    connect(item, &InteractiveButtonBase::signalMouseEnterLater, [=]{
        if (current_sub_menu)
        {
            if (item->subMenu() == current_sub_menu)
                return ;
            current_sub_menu->hide();
        }

        current_sub_menu = item->subMenu();
        current_sub_menu->move(this->geometry().center());
        current_sub_menu->show();
//        current_sub_menu->execute();
    });

    FacileMenu* menu = new FacileMenu(true, this);
    menu->hide();
    item->setSubMenu(menu);
    connect(menu, &FacileMenu::signalHidden, item, [=]{
        item->discardHoverPress();
    });
    return menu;
}

FacileMenu *FacileMenu::addMenu(QString text, FuncType func)
{
   return addMenu(QIcon(), text, func);
}

void FacileMenu::execute(QPoint pos)
{
    if (pos == QPoint(-1,-1))
        pos = QCursor::pos();
    main_vlayout->activate(); // 先调整所有控件大小

    // 获取屏幕大小
    QDesktopWidget* desktop = QApplication::desktop();
    QRect avai = desktop->availableGeometry();
    // 如果超过范围，则调整位置
    if (pos.x() >= width() && pos.x() + width() > avai.width())
        pos.setX(pos.x() - width());
    if (pos.y() >= height() && pos.y() + height() > avai.height())
        pos.setY(pos.y() - height());

    // 移动窗口
    move(pos);

    // 显示、动画
    QWidget::show();
    setFocus();

    startAnimationOnShowed();
}

Qt::Key FacileMenu::getShortcutByText(QString text)
{
    Qt::Key key = Qt::Key_Exit;
    QRegularExpression re("&([\\d\\w])");
    auto match = re.match(text);
    if (match.hasMatch())
    {
        const QChar ch = match.capturedTexts().at(1).at(0); // 快捷键字符串
        if (ch >= '0' && ch <= '9')
            key = (Qt::Key)(Qt::Key_0 + (ch.toLatin1() - '0'));
        else if (ch >= 'a' && ch <= 'z')
            key = (Qt::Key)(Qt::Key_A + (ch.toUpper().toLatin1() - 'A'));
        else if (ch >= 'A' && ch <= 'Z')
            key = (Qt::Key)(Qt::Key_A + (ch.toUpper().toLatin1() - 'A'));
    }
    return key;
}

void FacileMenu::setActionButton(InteractiveButtonBase *btn)
{
    // 设置尺寸
    btn->setPaddings(8, 48, 8, 8);

    // TODO:设置颜色
    btn->setNormalColor(normal_bg);
    btn->setHoverColor(hover_bg);
    btn->setPressColor(press_bg);
    btn->setTextColor(text_fg);
}

void FacileMenu::startAnimationOnShowed()
{
    main_vlayout->setEnabled(false);

    // 从上往下的动画
    QPoint start_pos = mapFromGlobal(QCursor::pos());
    for (int i = 0; i < items.size(); i++)
    {
        InteractiveButtonBase* btn = items.at(i);
        btn->setBlockHover(true);
        QPropertyAnimation* ani = new QPropertyAnimation(btn, "pos");
        ani->setStartValue(start_pos);
        ani->setEndValue(btn->pos());
        ani->setEasingCurve(QEasingCurve::OutBack);
        ani->setDuration(300);
        connect(ani, SIGNAL(finished()), ani, SLOT(deleteLater()));
        connect(ani, &QPropertyAnimation::finished, btn, [=]{
            btn->setBlockHover(false);
        });
        ani->start();
    }

    QTimer::singleShot(300, this, [=]{
        main_vlayout->setEnabled(true);
    });
}

void FacileMenu::hideEvent(QHideEvent *event)
{
    emit signalHidden();
    return QWidget::hideEvent(event);
}
