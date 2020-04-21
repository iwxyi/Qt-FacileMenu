#ifndef FACILEMENU_H
#define FACILEMENU_H

#include <QObject>
#include <QDialog>
#include <functional>
#include <QRegularExpression>
#include <QVBoxLayout>
#include "facilemenuitem.h"

typedef std::function<void()> const FuncType;

class FacileMenu : public QWidget
{
    Q_OBJECT
public:
    FacileMenu(QWidget *parent = nullptr);
    FacileMenu(bool sub, QWidget* parent = nullptr);

    void addAction(QIcon icon, QString text, FuncType func);
    void addAction(QString text, FuncType func);
    void addAction(QIcon icon, QString text, bool check, FuncType func);
    void addAction(QString text, bool check, FuncType func);
    FacileMenu* addMenu(QIcon icon, QString text, FuncType func);
    FacileMenu* addMenu(QString text, FuncType func);

    void execute(QPoint pos = QPoint(-1, -1));

signals:
    void signalActionTriggered(FacileMenuItem* action);

public slots:

private:
    Qt::Key getShortcutByText(QString text);
    void setActionButton(InteractiveButtonBase* btn);

private:
    QList<InteractiveButtonBase*> buttons;
    QList<FacileMenuItem*> items;
    QVBoxLayout* main_vlayout;

    FacileMenu* current_sub_menu = nullptr;
    bool _is_sub_menu; // 是否是子菜单
};

#endif // FACILEMENU_H
