#ifndef FACILEMENUITEM_H
#define FACILEMENUITEM_H

#include "interactivebuttonbase.h"

class FacileMenu;

class FacileMenuItem : public InteractiveButtonBase
{
public:
    FacileMenuItem(QWidget* parent = nullptr);
    FacileMenuItem(QString t, QWidget* parent = nullptr);
    FacileMenuItem(QIcon i, QString t, QWidget* parent = nullptr);
    FacileMenuItem(QPixmap p, QString t, QWidget* parent = nullptr);

    void setChecked(bool c);
    bool isChecked();
    void setKey(Qt::Key key);
    bool isKey(Qt::Key key);
    void setSubMenu(FacileMenu* menu);
    bool isSubMenu();

protected:
    void drawIconBeforeText(QPainter &painter, QRect icon_rect) override;

private:
    Qt::Key key;
    bool checkable = false;
    FacileMenu* sub_menu;
};

#endif // FACILEMENUITEM_H
