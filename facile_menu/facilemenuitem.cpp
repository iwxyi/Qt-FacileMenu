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

void FacileMenuItem::setChecked(bool c)
{
    checkable = true;
    _state = c;
    update();
}

bool FacileMenuItem::isChecked()
{
    return getState();
}

void FacileMenuItem::setKey(Qt::Key key)
{
    this->key = key;
}

bool FacileMenuItem::isKey(Qt::Key key)
{
    return key == this->key;
}

void FacileMenuItem::setSubMenu(FacileMenu *menu)
{
    sub_menu = menu;
}

bool FacileMenuItem::isSubMenu()
{
    return sub_menu != nullptr;
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
