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

FacileMenuItem *FacileMenuItem::setEnabled(bool e)
{
    InteractiveButtonBase::setEnabled(e);
    return this;
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

/**
 * 默认就是show状态
 * 为了和show区分开
 */
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

/**
 * 设置字符串，成立时 tru，不成立时 fal
 * 注意：这里是直接设置完整的文字，不会去掉快捷键&符号
 */
FacileMenuItem *FacileMenuItem::text(bool exp, QString tru, QString fal)
{
    if (exp)
        setText(tru);
    else
        setText(fal);
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
    return this;
}

/**
 * 绑定某一布尔类型的变量
 * 点击即切换值
 */
FacileMenuItem *FacileMenuItem::bind(bool &val)
{
    connect(this, &InteractiveButtonBase::clicked, this, [&]{
        val = !val;
    });
    return this;
}

/**
 * 适用于连续设置
 * 当 iff 成立时继续
 * 否则取消后面所有设置
 */
FacileMenuItem *FacileMenuItem::ifer(bool iff)
{
    if (iff)
        return this;

    // 返回一个无用item，在自己delete时也delete掉
    return createTempItem();
}

FacileMenuItem *FacileMenuItem::elser()
{
    if (parent_menu_item_in_if)
        return parent_menu_item_in_if;
    return createTempItem();
}

/**
 * 适用于连续设置action时，满足条件则退出
 * 相当于一个控制语句
 * 当br成立时，取消后面所有设置
 */
FacileMenuItem *FacileMenuItem::breaker(bool br)
{
    if (!br)
        return this;

    // 返回一个无用item，在自己delete时也delete掉
    auto useless = new FacileMenuItem("", this);
    useless->hide();
    useless->setEnabled(false);
    useless->setMinimumSize(0, 0);
    useless->setFixedSize(0, 0);
    useless->move(-999, -999);
    return useless;
}

/**
 * 适用于连续设置
 * 满足某一条件则执行 func(this)
 */
FacileMenuItem *FacileMenuItem::ifer(bool iff, FuncItemType func, FuncItemType elseFunc)
{
    if (iff)
        func(this);
    else if (elseFunc != nullptr)
        elseFunc(this);
    return this;
}

/**
 * 满足条件时，text添加前缀
 */
FacileMenuItem *FacileMenuItem::prefix(bool exp, QString pfix)
{
    if (exp)
        setText(pfix + getText());
    return this;
}

/**
 * 满足条件时，text添加后缀
 * @param inLeftParenthesis 支持 text(xxx) 形式，会在左括号前添加后缀
 */
FacileMenuItem *FacileMenuItem::suffix(bool exp, QString sfix, bool inLeftParenthesis)
{
    if (exp)
    {
        if (!inLeftParenthesis)
        {
            setText(getText() + sfix);
        }
        else
        {
            QString text = getText();
            int index = -1;
            if ((index = text.lastIndexOf("(")) > -1)
            {
                while (index > 0 && text.mid(index-1, 1) == " ")
                    index--;
            }
            if (index <= 0) // 没有左括号或者以空格开头，直接加到最后面
            {
                setText(getText() + sfix);
            }
            else
            {
                setText(text.left(index) + sfix + text.right(text.length()-index));
            }
        }
    }
    return this;
}

/**
 * 返回自己的子菜单对象
 */
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

FacileMenuItem *FacileMenuItem::createTempItem()
{
    auto useless = new FacileMenuItem(QIcon(), "", this);
    useless->parent_menu_item_in_if = this;
    useless->hide();
    useless->setEnabled(false);
    useless->setMinimumSize(0, 0);
    useless->setFixedSize(0, 0);
    useless->move(-999, -999);
    return useless;
}
