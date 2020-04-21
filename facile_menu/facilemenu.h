#ifndef FACILEMENU_H
#define FACILEMENU_H

#include <QObject>
#include <QDialog>
#include <functional>
#include <QRegularExpression>
#include <QVBoxLayout>
#include <QDesktopWidget>
#include <QApplication>
//#include <QGraphicsOpacityEffect>
#include <QScreen>
#include <QGraphicsDropShadowEffect>
#include "facilemenuitem.h"

class FacileMenu : public QWidget
{
    Q_OBJECT
public:
    FacileMenu(QWidget *parent = nullptr);
    FacileMenu(bool sub, QWidget* parent = nullptr);

    FacileMenuItem* addAction(QIcon icon, QString text, FuncType func = []{});
    FacileMenuItem* addAction(QString text, FuncType func = []{});
    FacileMenu* addMenu(QIcon icon, QString text, FuncType func = nullptr);
    FacileMenu* addMenu(QString text, FuncType func = nullptr);
    FacileMenuItem* addSeparator();
    void addTipArea(int x = 48);
    void addTipArea(QString longestTip);

    void execute(QPoint pos = QPoint(-1, -1));
    bool isCursorInArea(QPoint pos, FacileMenu* child = nullptr);
    void toHide(int focusIndex = -1);

signals:
    void signalActionTriggered(FacileMenuItem* action);
    void signalHidden();

public slots:

protected:
    Qt::Key getShortcutByText(QString text);
    void setActionButton(InteractiveButtonBase* btn);
    void showSubMenu(FacileMenuItem* item);
    void startAnimationOnShowed();
    void startAnimationOnHidden(int focusIndex);

    void hideEvent(QHideEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

public:
    static QColor normal_bg; // 普通背景（用作全局是为了方便设置）
    static QColor hover_bg;  // 悬浮背景
    static QColor press_bg;  // 按下背景
    static QColor text_fg;   // 字体/变色图标颜色
    static bool blue_bg;     // 背景是否透明

private:
    QList<FacileMenuItem*> items;
    QVBoxLayout* main_vlayout;
    QPixmap bg_pixmap;

    FacileMenu* current_sub_menu = nullptr; // 当前打开（不一定显示）的子菜单
    FacileMenu* parent_menu = nullptr; // 父类的菜单

    int addin_tip_area = 0;
    bool _showing_animation = false;
    int current_index = -1; // 当前索引
};

#endif // FACILEMENU_H
