#ifndef FACILEMENUITEM_H
#define FACILEMENUITEM_H

#include "interactivebuttonbase.h"

typedef std::function<void()> const FuncType;
typedef std::function<void(int)> const FuncIntType;

class FacileMenu;

class FacileMenuItem : public InteractiveButtonBase
{
public:
    FacileMenuItem(QWidget* parent = nullptr);
    FacileMenuItem(QString t, QWidget* parent = nullptr);
    FacileMenuItem(QIcon i, QWidget* parent = nullptr);
    FacileMenuItem(QIcon i, QString t, QWidget* parent = nullptr);
    FacileMenuItem(QPixmap p, QString t, QWidget* parent = nullptr);

    FacileMenuItem* setCheckable(bool c);
    bool isCheckable() const;
    FacileMenuItem* setChecked(bool c);
    bool isChecked();
    FacileMenuItem* setKey(Qt::Key key);
    bool isKey(Qt::Key key) const;
    FacileMenuItem* setSubMenu(FacileMenu* menu);
    bool isSubMenu() const;
    bool isLinger() const;

    FacileMenuItem* tip(QString sc);
    FacileMenuItem* triggered(FuncType func);
    FacileMenuItem* disable(bool dis = true); // 满足情况下触发，不满足不变，下同
    FacileMenuItem* enable(bool en = true);
    FacileMenuItem* hide(bool hi = true);
    FacileMenuItem* visible(bool vi = true);
    FacileMenuItem* check(bool ch = true);
    FacileMenuItem* uncheck(bool uc = true);
    FacileMenuItem* text(bool te, QString str);
    FacileMenuItem* icon(bool ic, QIcon icon);
    FacileMenuItem* linger();

    FacileMenu* subMenu();

protected:
    void paintEvent(QPaintEvent *event) override;
    void drawIconBeforeText(QPainter &painter, QRect icon_rect) override;

private:
    Qt::Key key;
    bool checkable = false;
    bool trigger_linger = false; // 点击后是否保存菜单
    FacileMenu* sub_menu = nullptr;
    QString shortcut_tip = ""; // 快捷键提示
};

#endif // FACILEMENUITEM_H
