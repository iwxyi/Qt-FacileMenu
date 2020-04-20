#ifndef FACILEMENU_H
#define FACILEMENU_H

#include <QObject>
#include <QDialog>
#include <functional>
#include <QRegularExpression>
#include <QVBoxLayout>
#include "interactivebuttonbase.h"

typedef std::function<void()> const FuncType;

struct FacileAction {
    QIcon icon;
    QString text;
    Qt::Key key;
    bool isMenu;
};

class FacileMenu : public QWidget
{
    Q_OBJECT
public:
    FacileMenu(QWidget *parent = nullptr);

    void addAction(QIcon icon, QString text, FuncType func);
    void addAction(QString text, FuncType func);
    FacileMenu* addMenu(QIcon icon, QString text, FuncType& func);
    FacileMenu* addMenu(QString text, FuncType&& func);

    void execute(QPoint pos = QPoint(-1, -1));

signals:
    void signalActionTriggered(FacileAction action);

public slots:

private:
    Qt::Key getShortcutByText(QString text);
    void setActionButton(InteractiveButtonBase* btn);

private:
    QList<InteractiveButtonBase*> buttons;
    QList<FacileAction> actions;
    QVBoxLayout* main_vlayout;
};

#endif // FACILEMENU_H
