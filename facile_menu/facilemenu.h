#ifndef FACILEMENU_H
#define FACILEMENU_H

#include <QObject>
#include <QDialog>
#include <functional>
#include <QRegularExpression>
#include <QVBoxLayout>
#include <QDesktopWidget>
#include <QApplication>
#include "facilemenuitem.h"

class FacileMenu : public QWidget
{
    Q_OBJECT
public:
    FacileMenu(QWidget *parent = nullptr);
    FacileMenu(bool sub, QWidget* parent = nullptr);

    FacileMenuItem* addAction(QIcon icon, QString text, FuncType func = []{});
    FacileMenuItem* addAction(QString text, FuncType func = []{});
    FacileMenu* addMenu(QIcon icon, QString text, FuncType func = []{});
    FacileMenu* addMenu(QString text, FuncType func = []{});

    void execute(QPoint pos = QPoint(-1, -1));

signals:
    void signalActionTriggered(FacileMenuItem* action);
    void signalHidden();

public slots:

protected:
    Qt::Key getShortcutByText(QString text);
    void setActionButton(InteractiveButtonBase* btn);
    void startAnimationOnShowed();
    void hideEvent(QHideEvent *event) override;

public:
    static QColor normal_bg; // 普通背景
    static QColor hover_bg;  // 悬浮背景
    static QColor press_bg;  // 按下背景
    static QColor text_fg;   // 字体/变色图标颜色

private:
    QList<FacileMenuItem*> items;
    QVBoxLayout* main_vlayout;

    FacileMenu* current_sub_menu = nullptr;
    bool _is_sub_menu; // 是否是子菜单
};

#endif // FACILEMENU_H
