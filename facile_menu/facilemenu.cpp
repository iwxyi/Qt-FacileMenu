#include "facilemenu.h"

QColor FacileMenu::normal_bg = QColor(255, 255, 255);
QColor FacileMenu::hover_bg = QColor(64, 64, 64, 64);
QColor FacileMenu::press_bg = QColor(128, 128, 128, 128);
QColor FacileMenu::text_fg = QColor(0, 0, 0);

FacileMenu::FacileMenu(QWidget *parent) : QWidget(parent)
{
    setObjectName("FacileMenu");
    setAttribute(Qt::WA_DeleteOnClose, true);
    setFocusPolicy(Qt::StrongFocus); // 获取焦点，允许按键点击
    setWindowFlag(Qt::Popup, true);
    setAutoFillBackground(false);  //这个不设置的话就背景变黑
    setAttribute(Qt::WA_StyledBackground);

    main_vlayout = new QVBoxLayout(this);
    setLayout(main_vlayout);
    main_vlayout->setEnabled(true);
    main_vlayout->setMargin(0);
    main_vlayout->setSpacing(0);

    setStyleSheet("background: "+QVariant(normal_bg).toString()+"; border: none; border-radius:5px;");

    setMouseTracking(true);
}

FacileMenu::FacileMenu(bool, QWidget *parent) : FacileMenu(parent)
{
    setAttribute(Qt::WA_DeleteOnClose, false);
    setFocusPolicy(Qt::NoFocus);

    // 保存父菜单的指针，判断鼠标移动的位置
    parent_menu = static_cast<FacileMenu*>(parent);
}

FacileMenuItem *FacileMenu::addAction(QIcon icon, QString text, FuncType func)
{
    auto key = getShortcutByText(text);
    text.replace("&", "");
    FacileMenuItem* item = new FacileMenuItem(icon, text, this);
    item->setKey(key);

    setActionButton(item);
    main_vlayout->addWidget(item);
    items.append(item);

    connect(item, &InteractiveButtonBase::clicked, this, [=]{
        func();
        emit signalActionTriggered(item);
        toHide(items.indexOf(item));
    });
    connect(item, &InteractiveButtonBase::signalMouseEnter, this, [=]{
       if (current_sub_menu) // 进入这个action，展开的子菜单隐藏起来
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
    auto key = getShortcutByText(text);
    text.replace("&", "");
    FacileMenuItem* item = new FacileMenuItem(icon, text, this);
    item->setKey(key);

    setActionButton(item);
    main_vlayout->addWidget(item);
    items.append(item);

    // 子菜单项是否可点击
    if (func != nullptr)
    {
        connect(item, &InteractiveButtonBase::clicked, this, [=]{
            func();
            emit signalActionTriggered(item);
            toHide(items.indexOf(item));
        });
    }
    else // 点击出现子项
    {
        connect(item, &InteractiveButtonBase::clicked, this, [=]{
            // 显示子菜单
            showSubMenu(item);
        });
    }

    connect(item, &InteractiveButtonBase::signalMouseEnterLater, [=]{
        // 显示子菜单
        showSubMenu(item);
    });

    FacileMenu* menu = new FacileMenu(true, this);
    menu->hide();
    item->setSubMenu(menu);
    connect(menu, &FacileMenu::signalHidden, item, [=]{
        // 子菜单隐藏，当前按钮强制取消hover状态
        item->discardHoverPress();
    });
    connect(menu, &FacileMenu::signalActionTriggered, this, [=](FacileMenuItem* action){
        // 子菜单被点击了，副菜单依次隐藏
        emit signalActionTriggered(action);
        toHide(items.indexOf(item));
    });
    return menu;
}

FacileMenu *FacileMenu::addMenu(QString text, FuncType func)
{
    return addMenu(QIcon(), text, func);
}

FacileMenuItem *FacileMenu::addSeparator()
{
    FacileMenuItem* item = new FacileMenuItem(this);
    item->setNormalColor(QColor(64, 64, 64, 64));
    item->setFixedHeight(1);
    item->setPaddings(32, 32, 0, 0);

    main_vlayout->addWidget(item);
    items.append(item);

    return item;
}

void FacileMenu::addTipArea(int x)
{
    addin_tip_area = x;
}

void FacileMenu::addTipArea(QString longestTip)
{
    QFontMetrics fm(this->font());
    addin_tip_area = fm.horizontalAdvance(longestTip+"Ctrl");
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

    // 设置背景为圆角矩形
    if (height() > 0) // 没有菜单项的时候为0
    {
        QPixmap pixmap(width(), height());
        pixmap.fill(Qt::transparent);
        QPainter pix_ptr(&pixmap);
        pix_ptr.setRenderHint(QPainter::Antialiasing, true);
        QPainterPath path;
        path.addRoundedRect(0, 0, width(), height(), 5, 5);
        pix_ptr.fillPath(path, Qt::white);
        setMask(pixmap.mask());
    }

    // 显示、动画
    QWidget::show();
    setFocus();

    startAnimationOnShowed();
}

bool FacileMenu::isCursorInArea(QPoint pos)
{
    return geometry().contains(pos);
}

void FacileMenu::toHide(int focusIndex)
{
    startAnimationOnHidden(focusIndex);
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
    btn->setPaddings(8, 48 + addin_tip_area, 8, 8);

    // 设置颜色
    btn->setNormalColor(Qt::transparent);
    btn->setHoverColor(hover_bg);
    btn->setPressColor(press_bg);
    btn->setTextColor(text_fg);
}

void FacileMenu::showSubMenu(FacileMenuItem *item)
{
    if (current_sub_menu)
    {
        if (item->subMenu() == current_sub_menu && !current_sub_menu->isHidden()) // 当前显示的就是这个子菜单
            return ;
        current_sub_menu->hide();
    }

    current_sub_menu = item->subMenu();
    current_sub_menu->execute();
}

/**
 * 菜单出现动画
 * 从光标位置依次出现
 */
void FacileMenu::startAnimationOnShowed()
{
    main_vlayout->setEnabled(false);
    _showing_animation = true;

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
        _showing_animation = false;
    });
}

/**
 * 关闭前显示隐藏动画
 * @param focusIndex 聚焦的item，如果不存在则为-1
 */
void FacileMenu::startAnimationOnHidden(int focusIndex)
{
    int dur_min =100, dur_max = 200;
    int up_flow_count = focusIndex > -1 ? qMax(focusIndex, items.size()-focusIndex-1) : -1;
    int up_end = items.size() ? -items.at(0)->height() : 0;
    int flow_end = height();
    int focus_top = focusIndex > -1 ? items.at(focusIndex)->pos().y() : 0;
    for (int i = 0; i < items.size(); i++)
    {
        InteractiveButtonBase* btn = items.at(i);
        btn->setBlockHover(true);
        QPropertyAnimation* ani = new QPropertyAnimation(btn, "pos");
        ani->setStartValue(btn->pos());
        ani->setEasingCurve(QEasingCurve::OutCubic);
        if (focusIndex > -1)
        {
            if (i < focusIndex) // 上面的项
            {
                ani->setEndValue(QPoint(0, up_end - (focus_top - btn->pos().y()) / 8));
                ani->setDuration(dur_max - qAbs(focusIndex-i)*(dur_max-dur_min)/up_flow_count);
            }
            else if (i == focusIndex) // 中间的项
            {
                ani->setEndValue(btn->pos());
                ani->setDuration(dur_max);
            }
            else // 下面的项
            {
                ani->setEndValue(QPoint(0, flow_end + (btn->pos().y()-focus_top) / 8));
                ani->setDuration(dur_max - qAbs(i-focusIndex)*(dur_max-dur_min)/up_flow_count);
            }
        }
        else
        {
            ani->setEndValue(QPoint(0, up_end));
            ani->setDuration(dur_max);
        }
        connect(ani, SIGNAL(finished()), ani, SLOT(deleteLater()));
        connect(ani, &QPropertyAnimation::finished, btn, [=]{
            btn->setBlockHover(false);
        });
        ani->start();
    }

    QTimer::singleShot(dur_max*0.8, this, [=]{
        main_vlayout->setEnabled(true);
        close();
    });
}

void FacileMenu::hideEvent(QHideEvent *event)
{
    emit signalHidden();
    return QWidget::hideEvent(event);
}

void FacileMenu::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);

    QPoint pos = QCursor::pos();
    if (_showing_animation || isCursorInArea(pos)) // 正在出现或在自己的区域内，不管
        ;
    else if (parent_menu && parent_menu->isCursorInArea(pos)) // 在父类，自己隐藏
    {
        this->hide();
        parent_menu->setFocus();
    }
}

void FacileMenu::keyPressEvent(QKeyEvent *event)
{
    auto key = event->key();
    foreach (auto item, items)
    {
        if (item->isKey((Qt::Key)key))
        {
            item->simulateStatePress();
            break;
        }
    }

    return QWidget::keyPressEvent(event);
}
