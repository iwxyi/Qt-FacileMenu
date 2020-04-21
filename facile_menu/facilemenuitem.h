#ifndef FACILEMENUITEM_H
#define FACILEMENUITEM_H

#include "interactivebuttonbase.h"

typedef std::function<void()> const FuncType;

class FacileMenu;

class FacileMenuItem : public InteractiveButtonBase
{
public:
    FacileMenuItem(QWidget* parent = nullptr);
    FacileMenuItem(QString t, QWidget* parent = nullptr);
    FacileMenuItem(QIcon i, QString t, QWidget* parent = nullptr);
    FacileMenuItem(QPixmap p, QString t, QWidget* parent = nullptr);

    FacileMenuItem* setChecked(bool c);
    bool isChecked();
    FacileMenuItem* setKey(Qt::Key key);
    bool isKey(Qt::Key key);
    FacileMenuItem* setSubMenu(FacileMenu* menu);
    bool isSubMenu();
    FacileMenuItem* triggered(FuncType func);
    FacileMenuItem* disable();
    FacileMenu* subMenu();

protected:
    void drawIconBeforeText(QPainter &painter, QRect icon_rect) override;

private:
    Qt::Key key;
    bool checkable = false;
    FacileMenu* sub_menu;
};

#endif // FACILEMENUITEM_H
