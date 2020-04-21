#include "facilemenuitem.h"

FacileMenuItem::FacileMenuItem(QWidget *parent) : InteractiveButtonBase(parent)
{

}

FacileMenuItem::FacileMenuItem(QString t, QWidget *parent) : InteractiveButtonBase(t, parent)
{

}

FacileMenuItem::FacileMenuItem(QIcon i, QString t, QWidget *parent) : InteractiveButtonBase(i, t, parent)
{

}

FacileMenuItem::FacileMenuItem(QPixmap p, QString t, QWidget *parent) : InteractiveButtonBase(p, t, parent)
{

}

FacileMenuItem *FacileMenuItem::setChecked(bool c)
{
    checkable = true;
    _state = c;
    update();
    return this;
}

bool FacileMenuItem::isChecked()
{
    return getState();
}

FacileMenuItem *FacileMenuItem::setKey(Qt::Key key)
{
    this->key = key;
    return this;
}

bool FacileMenuItem::isKey(Qt::Key key)
{
    return key == this->key;
}

FacileMenuItem *FacileMenuItem::setSubMenu(FacileMenu *menu)
{
    sub_menu = menu;
    return this;
}

bool FacileMenuItem::isSubMenu()
{
    return sub_menu != nullptr;
}

FacileMenuItem *FacileMenuItem::triggered(FuncType func)
{
    connect(this, &InteractiveButtonBase::clicked, this, [=]{
        func();
    });
    return this;
}

FacileMenuItem *FacileMenuItem::disable()
{
    setDisabled(true);
    return this;
}

FacileMenu *FacileMenuItem::subMenu()
{
    return sub_menu;
}

void FacileMenuItem::paintEvent(QPaintEvent *event)
{
    InteractiveButtonBase::paintEvent(event);

    if (isSubMenu())
    {
        // 画右边箭头的图标
        QPainter painter(this);
        QRect rect(width() - icon_text_size - 8, 8, icon_text_size, icon_text_size);
        painter.drawPixmap(rect, QPixmap(":/icons/sub_menu_arrow"));
    }
}

void FacileMenuItem::drawIconBeforeText(QPainter &painter, QRect icon_rect)
{
    // 选中
    if (checkable)
    {
        QPainterPath path;
        path.addRoundedRect(icon_rect, 3, 3);
        if (getState())
        {
            // 绘制选中样式： 圆角矩形
            painter.fillPath(path, press_bg);
        }
        else
        {
            // 绘制未选中样式：空白边框
            painter.save();
            painter.setPen(QPen(press_bg, 1));
            painter.drawPath(path);
            painter.restore();
        }
    }

    InteractiveButtonBase::drawIconBeforeText(painter, icon_rect);
}
