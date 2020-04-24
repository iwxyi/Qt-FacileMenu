#include "facilemenuitem.h"

FacileMenuItem::FacileMenuItem(QWidget *parent) : InteractiveButtonBase(parent)
{

}

FacileMenuItem::FacileMenuItem(QString t, QWidget *parent) : InteractiveButtonBase(t, parent)
{

}

FacileMenuItem::FacileMenuItem(QIcon i, QWidget *parent) : InteractiveButtonBase(i, parent)
{

}

FacileMenuItem::FacileMenuItem(QIcon i, QString t, QWidget *parent) : InteractiveButtonBase(i, t, parent)
{

}

FacileMenuItem::FacileMenuItem(QPixmap p, QString t, QWidget *parent) : InteractiveButtonBase(p, t, parent)
{

}

FacileMenuItem *FacileMenuItem::setCheckable(bool c)
{
    checkable = c;
    update();
    return this;
}

bool FacileMenuItem::isCheckable() const
{
    return checkable;
}

FacileMenuItem *FacileMenuItem::setChecked(bool c)
{
    checkable = true;
    _state = c;
    if (InteractiveButtonBase::icon.isNull())
        model = IconText; // 强制显示check空白部分
    update();
    return this;
}

inline bool FacileMenuItem::isChecked()
{
    return getState();
}

FacileMenuItem *FacileMenuItem::setKey(Qt::Key key)
{
    this->key = key;
    return this;
}

bool FacileMenuItem::isKey(Qt::Key key) const
{
    return key == this->key;
}

FacileMenuItem *FacileMenuItem::setSubMenu(FacileMenu *menu)
{
    sub_menu = menu;
    return this;
}

bool FacileMenuItem::isSubMenu() const
{
    return sub_menu != nullptr;
}

bool FacileMenuItem::isLinger() const
{
    return trigger_linger;
}

FacileMenuItem *FacileMenuItem::tip(QString sc)
{
    shortcut_tip = sc;
    return this;
}

FacileMenuItem *FacileMenuItem::tooltip(QString tt)
{
    setToolTip(tt);
    return this;
}

FacileMenuItem *FacileMenuItem::triggered(FuncType func)
{
    connect(this, &InteractiveButtonBase::clicked, this, [=]{
        func();
    });
    return this;
}

FacileMenuItem *FacileMenuItem::disable(bool dis)
{
    if (dis)
        setDisabled(true);
    return this;
}

FacileMenuItem *FacileMenuItem::enable(bool en)
{
    if (en)
        setEnabled(true);
    return this;
}

FacileMenuItem *FacileMenuItem::hide(bool hi)
{
    if (hi)
        InteractiveButtonBase::hide();
    return this;
}

FacileMenuItem *FacileMenuItem::visible(bool vi)
{
    if (vi)
        InteractiveButtonBase::setVisible(true);
    return this;
}

FacileMenuItem *FacileMenuItem::check(bool ch)
{
    setCheckable(true);
    if (ch)
        setChecked(true);
    else if (InteractiveButtonBase::icon.isNull())
        model = IconText; // 强制显示check空白部分
    return this;
}

FacileMenuItem *FacileMenuItem::uncheck(bool uc)
{
    setCheckable(true);
    if (uc)
        setChecked(false);
    return this;
}

FacileMenuItem *FacileMenuItem::text(bool te, QString str)
{
    if (te)
    {
        // 去掉快捷键符号
        // 注意：这里设置文字不会改变原来的快捷键！
        setText(str.replace(QRegExp("&([\\w\\d])\\b"), "\\1"));
        // 调整大小
        setFixedForeSize();
    }
    return this;
}

FacileMenuItem *FacileMenuItem::icon(bool ic, QIcon ico)
{
    if (ic)
        setIcon(ico);
    return this;
}

FacileMenuItem *FacileMenuItem::borderR(int radius, QColor co)
{
    setRadius(radius);
    if (co != Qt::transparent)
        setBorderColor(co);
    else
        setBorderColor(press_bg);
    return this;
}

FacileMenuItem *FacileMenuItem::linger()
{
    trigger_linger = true;
}

FacileMenu *FacileMenuItem::subMenu()
{
    return sub_menu;
}

void FacileMenuItem::paintEvent(QPaintEvent *event)
{
    InteractiveButtonBase::paintEvent(event);

    int right = width() - icon_text_size - 8;

    QPainter painter(this);
    if (isSubMenu())
    {
        // 画右边箭头的图标
        QRect rect(right, fore_paddings.top, icon_text_size, icon_text_size);
        painter.drawPixmap(rect, QPixmap(":/icons/sub_menu_arrow"));
    }

    if (!shortcut_tip.isEmpty())
    {
        // 画右边的文字
        QFontMetrics fm(this->font());
        int width = fm.horizontalAdvance(shortcut_tip);
        painter.save();
        auto c = painter.pen().color();
        c.setAlpha(c.alpha() / 2);
        painter.setPen(c);
        painter.drawText(QRect(right-width, fore_paddings.top, width, height()-fore_paddings.top-fore_paddings.bottom),
                         Qt::AlignRight, shortcut_tip);
        painter.restore();
    }
}

void FacileMenuItem::drawIconBeforeText(QPainter &painter, QRect icon_rect)
{
    // 选中
    if (checkable)
    {
        QPainterPath path;
        QRect expand_rect = icon_rect;
        expand_rect.adjust(-2, -2, 2, 2);
        path.addRoundedRect(expand_rect, 3, 3);
        if (InteractiveButtonBase::icon.isNull())
        {
            // 绘制√
            if (isChecked())
                painter.drawText(icon_rect, "√");
        }
        else // 有图标，使用
        {
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
    }

    InteractiveButtonBase::drawIconBeforeText(painter, icon_rect);
}
