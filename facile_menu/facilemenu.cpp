#include "facilemenu.h"

QColor FacileMenu::normal_bg = QColor(255, 255, 255);
QColor FacileMenu::hover_bg = QColor(64, 64, 64, 64);
QColor FacileMenu::press_bg = QColor(128, 128, 128, 128);
QColor FacileMenu::text_fg = QColor(0, 0, 0);
bool FacileMenu::blue_bg = true;

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
        int index = items.indexOf(item);
        if (using_keyboard && current_index > -1 && current_index < items.size() && current_index != index) // 屏蔽键盘操作
            items.at(current_index)->discardHoverPress(true);
        current_index = index;

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

void FacileMenu::addChipLayout()
{
    QHBoxLayout* layout = new QHBoxLayout;
    chip_hlayouts.append(layout);
    main_vlayout->addLayout(layout);
}

/**
 * 添加小部件
 */
FacileMenuItem *FacileMenu::addChip(QIcon icon, QString text, FuncType func)
{
    auto key = getShortcutByText(text);
    text.replace("&", "");
    FacileMenuItem* item = new FacileMenuItem(icon, text, this);
    item->setKey(key);

    setActionButton(item, true);
    if (chip_hlayouts.size())
        chip_hlayouts.last()->addWidget(item);
    else
        main_vlayout->addWidget(item);
    items.append(item);

    if (func != nullptr)
    {
        connect(item, &InteractiveButtonBase::clicked, this, [=]{
            func();
            emit signalActionTriggered(item);
            toHide(items.indexOf(item));
        });
    }
    connect(item, &InteractiveButtonBase::signalMouseEnter, this, [=]{
        int index = items.indexOf(item);
        if (using_keyboard && current_index > -1 && current_index < items.size() && current_index != index) // 屏蔽键盘操作
            items.at(current_index)->discardHoverPress(true);
        current_index = index;

        if (current_sub_menu) // 进入这个action，展开的子菜单隐藏起来
        {
           current_sub_menu->hide();
           current_sub_menu = nullptr;
        }
    });
    return item;
}

FacileMenuItem *FacileMenu::addChip(QIcon icon, FuncType func)
{
    return addChip(icon, "", func);
}

FacileMenuItem *FacileMenu::addChip(QString text, FuncType func)
{
    return addChip(QIcon(), text, func);
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

    /*connect(item, &InteractiveButtonBase::signalMouseEnter, [=]{
        if (current_index != items.indexOf(item) && current_index >= 0 && current_index < items.size())
            items.at(current_index)->discardHoverPress(true);
        current_index = items.indexOf(item);
    });*/

    connect(item, &InteractiveButtonBase::signalMouseEnterLater, [=]{
        int index = items.indexOf(item);
        if (using_keyboard && current_index > -1 && current_index < items.size() && current_index != index) // 屏蔽键盘操作
            items.at(current_index)->discardHoverPress(true);
        current_index = index;

        // 显示子菜单
        if (current_index == items.indexOf(item))
            showSubMenu(item);
    });

    /*connect(item, &InteractiveButtonBase::signalMouseLeave, [=]{
        if (item->isHovering())
            item->discardHoverPress(true);
    });*/

    FacileMenu* menu = new FacileMenu(true, this);
    menu->hide();
    item->setSubMenu(menu);
    connect(menu, &FacileMenu::signalHidden, item, [=]{
        if (!using_keyboard || current_index != items.indexOf(item))
        {
            // 子菜单隐藏，当前按钮强制取消hover状态
            item->discardHoverPress(true);
        }
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
    item->setDisabled(true);

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
    current_index = -1;

    if (pos == QPoint(-1,-1))
        pos = QCursor::pos();
    main_vlayout->setEnabled(true);
    main_vlayout->activate(); // 先调整所有控件大小

    // 获取屏幕大小
    QRect avai = QApplication::desktop()->availableGeometry();
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

    // 是否捕获背景模糊图片
    if (blue_bg)
    {
        // 获取图片
        QRect rect = this->geometry();
        int radius = qMin(64, qMin(width(), height())); // 模糊半径，也是边界
        rect.adjust(-radius, -radius, +radius, +radius);
        QScreen* screen = QApplication::screenAt(QCursor::pos());
        QPixmap bg = screen->grabWindow(QApplication::desktop()->winId(), rect.left(), rect.top(), rect.width(), rect.height());

        // 开始模糊
        QT_BEGIN_NAMESPACE
          extern Q_WIDGETS_EXPORT void qt_blurImage( QPainter *p, QImage &blurImage, qreal radius, bool quality, bool alphaOnly, int transposed = 0 );
        QT_END_NAMESPACE

        QPixmap pixmap = bg;
        QImage img = pixmap.toImage(); // img -blur-> painter(pixmap)
        QPainter painter( &pixmap );
        // 填充半透明的背景颜色，避免太透
        {
            QColor bg_c(normal_bg);
            bg_c.setAlpha(normal_bg.alpha() / 2);
            painter.fillRect(0, 0, pixmap.width(), pixmap.height(), bg_c);
        }
        qt_blurImage( &painter, img, radius, true, false );
        // 裁剪掉边缘（模糊后会有黑边）
        int c = qMin(bg.width(), bg.height());
        c = qMin(c/2, radius);
        bg_pixmap = pixmap.copy(c, c, pixmap.width()-c*2, pixmap.height()-c*2);
    }


    // 显示、动画
    QWidget::show();
    setFocus();
    startAnimationOnShowed();
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

/**
 * 统一设置Action按钮（尺寸、颜色等）
 * @param btn     按钮
 * @param isChip  是否是小按钮（一行多个，右边不用空白）
 */
void FacileMenu::setActionButton(InteractiveButtonBase *btn, bool isChip)
{
    // 设置尺寸
    if (isChip)
    {
        btn->setPaddings(8, 8, 8, 8);
    }
    else
    {
        btn->setPaddings(8, 48 + addin_tip_area, 8, 8);
    }

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
    QPoint pos(-1, -1);
    if (using_keyboard) // 键盘模式，不是跟随鼠标位置来的
    {
        QRect avai = QApplication::desktop()->availableGeometry();

        // 键盘模式，相对于点击项的右边
        QPoint tl = mapToGlobal(item->pos());
        if (tl.x() + item->width() + current_sub_menu->width() < avai.width())
            pos.setX(tl.x() + item->width());
        else
            pos.setX(tl.x() - current_sub_menu->width());
        if (tl.y() + current_sub_menu->height() < avai.height())
            pos.setY(tl.y());
        else
            pos.setY(tl.y() - current_sub_menu->height());
    }
    current_sub_menu->execute(pos);
    current_sub_menu->setKeyBoardUsed(using_keyboard);
}

bool FacileMenu::isCursorInArea(QPoint pos, FacileMenu *child)
{
    // 不在这范围内
    if (!geometry().contains(pos))
    {
        // 在自己的副菜单那里
        if (isSubMenu() && parent_menu->isCursorInArea(pos, this)) // 如果这也是子菜单（已展开），则递归遍历父菜单
        {
            QTimer::singleShot(0, this, [=]{
                close(); // 把自己也隐藏了
            });
            return true;
        }
        return false;
    }
    // 如果是正展开的这个子项按钮
    if (current_index > -1 && child && items.at(current_index)->subMenu() == child && items.at(current_index)->geometry().contains(mapFromGlobal(pos)))
        return false;
    // 在这个菜单内的其他按钮
    return true;
}

void FacileMenu::setKeyBoardUsed(bool use)
{
    using_keyboard = use;
    if (use && current_index == -1 && items.size()) // 还没有选中
    {
        items.at(current_index = 0)->simulateHover(); // 预先选中第一项
    }
}

bool FacileMenu::isSubMenu()
{
    return parent_menu != nullptr;
}

/**
 * 菜单出现动画
 * 从光标位置依次出现
 */
void FacileMenu::startAnimationOnShowed()
{
    main_vlayout->setEnabled(false);
    _showing_animation = true;
    QEasingCurve curve = QEasingCurve::OutBack;
    if (items.size() <= 1)
        curve = QEasingCurve::OutQuad;

    // 从上往下的动画
    QPoint start_pos = mapFromGlobal(QCursor::pos());
    if (start_pos.x() < 0)
        start_pos.setX(0);
    else if (start_pos.x() > width())
        start_pos.setX(width());
    if (start_pos.y() < 0)
        start_pos.setY(0);
    else if (start_pos.y() > height())
        start_pos.setY(height());
    if (items.size() >= 1)
    {
        if (start_pos.y() == 0) // 最顶上
            start_pos.setY(-items.at(0)->height());
        if (start_pos.x() == width()) // 最右边
            start_pos.setX(width()/2);
    }
    for (int i = 0; i < items.size(); i++)
    {
        InteractiveButtonBase* btn = items.at(i);
        btn->setBlockHover(true);
        QPropertyAnimation* ani = new QPropertyAnimation(btn, "pos");
        ani->setStartValue(start_pos);
        ani->setEndValue(btn->pos());
        ani->setEasingCurve(curve);
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
    // 控件移动动画
    main_vlayout->setEnabled(false);
    int dur_min =100, dur_max = 200;
    int up_flow_count = focusIndex > -1 ? qMax(focusIndex, items.size()-focusIndex-1) : -1;
    int up_end = items.size() ? -items.at(0)->height() : 0;
    int flow_end = height();
    int focus_top = focusIndex > -1 ? items.at(focusIndex)->pos().y() : 0;
//    QList<QPoint> ori_poss;
    for (int i = 0; i < items.size(); i++)
    {
        InteractiveButtonBase* btn = items.at(i);
//        ori_poss.append(btn->pos());
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

    // 变淡动画（针对Popup，一切透明无效）
    /*QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(this);
    effect->setOpacity(1);
    setGraphicsEffect(effect);
    QPropertyAnimation* opa_ani = new QPropertyAnimation(effect, "opacity");
    opa_ani->setDuration(dur_max * 0.8);
    opa_ani->setStartValue(1);
    opa_ani->setEndValue(0);
    connect(opa_ani, &QPropertyAnimation::finished, this, [=]{
        opa_ani->deleteLater();
        effect->deleteLater();
    });
    opa_ani->start();*/

    // 真正关闭
    QTimer::singleShot(dur_max, this, [=]{
        // 挨个还原之前的位置（不知道为什么main_vlayout不能恢复了）
//        for (int i = 0; i < ori_poss.size(); i++)
//            items.at(i)->move(ori_poss.at(i));
        main_vlayout->setEnabled(true);
        main_vlayout->activate(); // 恢复原来的位置

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
    else if (parent_menu && parent_menu->isCursorInArea(pos, this)) // 在父类，自己隐藏
    {
        this->hide();
        parent_menu->setFocus();
    }

    if (using_keyboard)
    {
        using_keyboard = false;
        if (current_index >= 0 && current_index <= items.size())
            items.at(current_index)->discardHoverPress(false);
    }
}

void FacileMenu::keyPressEvent(QKeyEvent *event)
{
    auto key = event->key();
    foreach (auto item, items)
    {
        if (item->isKey((Qt::Key)key))
        {
            item->simulateStatePress(); // 确定是这个action的快捷键
            return ;
        }
    }

    // 菜单按键响应
    if (event->modifiers() != Qt::NoModifier || items.size() == 0)
        return QWidget::keyPressEvent(event);

    switch (key) {
    case Qt::Key_Up:
    {
        if (current_index < 0 || current_index >= items.size())
            current_index = items.size()-1;
        else
        {
            items.at(current_index--)->discardHoverPress(true);
            if (current_index < 0)
                current_index = items.size()-1;
        }
        // 判断 item 是否被禁用
        if (!items.at(current_index)->isEnabled())
        {
            int old_index = current_index;
            do {
                current_index--;
                if (current_index < 0)
                    current_index = items.size()-1;
            } while (current_index != old_index && !items.at(current_index)->isEnabled());
            if (current_index == old_index) // 如果绕了一圈都不能用，取消
                return ;
        }
        // 找到真正的上一项
        items.at(current_index)->simulateHover();
        using_keyboard = true;
        return ;
    }
    case Qt::Key_Down:
        if (current_index < 0 || current_index >= items.size())
            current_index = 0;
        else
        {
            items.at(current_index++)->discardHoverPress(true);
            if (current_index >= items.size())
                current_index = 0;
        }
        // 判断 item 是否被禁用
        if (!items.at(current_index)->isEnabled())
        {
            int old_index = current_index;
            do {
                current_index++;
                if (current_index >= items.size())
                    current_index = 0;
            } while (current_index != old_index && !items.at(current_index)->isEnabled());
            if (current_index == old_index) // 如果绕了一圈都不能用，取消
                return ;
        }
        // 找到真正的上一项
        items.at(current_index)->simulateHover();
        using_keyboard = true;
        return ;
    case Qt::Key_Left:
        // 移动到左边按钮
        if (current_index > 0 && items.at(current_index-1)->pos().y() == items.at(current_index)->pos().y())
        {
            items.at(current_index--)->discardHoverPress(true);
            items.at(current_index)->setHover();
            using_keyboard = true;
        }
        // 退出子菜单
        else if (isSubMenu())
            close();
        return ;
    case Qt::Key_Right:
        // 移动到右边按钮
        if (current_index < items.size()-1 && items.at(current_index+1)->pos().y() == items.at(current_index)->pos().y())
        {
            items.at(current_index++)->discardHoverPress(true);
            items.at(current_index)->setHover();
            using_keyboard = true;
        }
        // 展开子菜单
        else if (current_index >= 0 && current_index < items.size() && items.at(current_index)->isSubMenu())
        {
            showSubMenu(items.at(current_index));
        }
        return ;
    case Qt::Key_Home:
        if (current_index >= 0 || current_index < items.size())
            items.at(current_index)->discardHoverPress(true);
        items.at(current_index = 0)->simulateHover();
        using_keyboard = true;
        return ;
    case Qt::Key_End:
        if (current_index >= 0 || current_index < items.size())
            items.at(current_index)->discardHoverPress(true);
        items.at(current_index = items.size())->simulateHover();
        using_keyboard = true;
        return ;
    case Qt::Key_Enter:
    case Qt::Key_Return:
    case Qt::Key_Space:
        if (current_index >= 0 || current_index < items.size())
        {
            auto item = items.at(current_index);
            item->simulateStatePress(!item->getState(), false);
        }
        return ;
    }

    return QWidget::keyPressEvent(event);
}

void FacileMenu::paintEvent(QPaintEvent *event)
{
    if (!bg_pixmap.isNull())
    {
        QPainter painter(this);
        painter.drawPixmap(QRect(0,0,width(),height()), bg_pixmap);
        return ;
    }
    QWidget::paintEvent(event);
}
