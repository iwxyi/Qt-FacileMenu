#include "facilemenubar.h"

FacileMenuBar::FacileMenuBar(QWidget *parent) : QWidget(parent)
{
    hlayout = new QHBoxLayout(this);
    hlayout->setAlignment(Qt::AlignLeft);
    hlayout->setSpacing(0);
    hlayout->setMargin(0);
    setLayout(hlayout);
}

/// 鼠标是否在这个区域内
/// @param pos 绝对位置
/// @return 所在的 MenuButton 索引，-1表示不在
int FacileMenuBar::isCursorInArea(QPoint pos) const
{
    QPoint mPos = mapFromGlobal(pos);
    if (!rect().contains(mPos))
        return -1;
    for (int i = 0; i < buttons.size(); i++)
    {
        if (buttons.at(i)->geometry().contains(mPos))
            return i;
    }
    return -1;
}

int FacileMenuBar::currentIndex() const
{
    return _currentIndex;
}

/// 准备激活第index个菜单
/// 如果不是参数menu，则激活该菜单
bool FacileMenuBar::triggerIfNot(int index, void *menu)
{
    // 非菜单栏中的按钮，隐藏全部
    FacileMenu* m = static_cast<FacileMenu*>(menu);
    qDebug() << "triggerIfNot:" << index << m << currentIndex();
    if (index < 0 || index >= buttons.size())
    {
        if (m)
            m->hide();
        return true;
    }

    // 切换按钮
    if (menus.at(index) == menu)
    {
        return false;
    }

    if (m)
        m->hide();

    trigger(index);

    return true;
}

void FacileMenuBar::addMenu(QString name, FacileMenu *menu)
{
    InteractiveButtonBase* btn = new InteractiveButtonBase(name, this);
    // TODO: name 识别并注册快捷键
    connect(btn, &InteractiveButtonBase::clicked, this, [=]{
        int index = buttons.indexOf(btn);
        if (!menus.at(index)->isHidden())
        {
            // 如果是正在显示状态
            // 但是理论上来说不可能，因为这种情况是点不到按钮的
            menus.at(index)->hide();
        }
        else
        {
            // 点击触发菜单
            trigger(buttons.indexOf(btn));
        }
    });

    btn->adjustMinimumSize();
    btn->setRadius(5);
    btn->setFixedForePos();

    menu->setMenuBar(this);
    menu->setAttribute(Qt::WA_DeleteOnClose, false);

    buttons.append(btn);
    menus.append(menu);
    hlayout->addWidget(btn);
}

/// 显示菜单
void FacileMenuBar::trigger(int index)
{
    if (index < 0 || index >= buttons.size())
        return ;

    InteractiveButtonBase* btn = buttons.at(index);
    QRect rect(btn->mapToGlobal(QPoint(0, 0)), btn->size());
    menus.at(index)->exec(rect, true);
    _currentIndex = index;
}
