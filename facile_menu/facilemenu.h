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
#include <QAction>
#include "facilemenuitem.h"

class FacileMenu : public QWidget
{
    Q_OBJECT
public:
    FacileMenu(QWidget *parent = nullptr);
    FacileMenu(bool sub, QWidget* parent = nullptr);
    ~FacileMenu() override;

    FacileMenuItem* addAction(QIcon icon, QString text, FuncType func = []{});
    FacileMenuItem* addAction(QString text, FuncType func = []{});
    FacileMenuItem* addAction(QAction* action, bool deleteWithMenu = true);
    FacileMenuItem* addAction(QIcon icon, QString text, void (*func)());
    template <class T>
    FacileMenuItem* addAction(QIcon icon, QString text, T *obj, void (T::*func)()); // 这个用不来

    FacileMenu* addChipLayout();
    FacileMenuItem* addChip(QIcon icon, QString text, FuncType func = nullptr);
    FacileMenuItem* addChip(QIcon icon, FuncType func = nullptr);
    FacileMenuItem* addChip(QString text, FuncType func = nullptr);

    FacileMenu* addMenu(QIcon icon, QString text, FuncType func = nullptr);
    FacileMenu* addMenu(QString text, FuncType func = nullptr);

    FacileMenuItem* addSeparator();
    FacileMenu* split();

    void addTipArea(int x = 48);
    void addTipArea(QString longestTip);

    void exec(QPoint pos = QPoint(-1, -1));
    void execute(QPoint pos = QPoint(-1, -1));
    void toHide(int focusIndex = -1);

    template <class T>
    void fun2(int j, T *obp, void (T::*p)());

signals:
    void signalActionTriggered(FacileMenuItem* action);
    void signalHidden();

public slots:
    void itemMouseEntered(FacileMenuItem* item);

protected:
    FacileMenuItem* createMenuItem(QIcon icon, QString text);
    Qt::Key getShortcutByText(QString text);
    void setActionButton(InteractiveButtonBase* btn, bool isChip = false);
    void showSubMenu(FacileMenuItem* item);
    bool isCursorInArea(QPoint pos, FacileMenu* child = nullptr);
    void setKeyBoardUsed(bool use = true);
    bool isSubMenu();
    FacileMenu* addVSeparator();
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
    QList<FacileMenuItem*> v_separators, h_separators;
    QVBoxLayout* main_vlayout;
    QList<QHBoxLayout*> chip_hlayouts;
    QList<QAction*> import_actions;
    QPixmap bg_pixmap;

    FacileMenu* current_sub_menu = nullptr; // 当前打开（不一定显示）的子菜单
    FacileMenu* parent_menu = nullptr; // 父类的菜单

    int addin_tip_area = 0; // 右边用来显示提示文字的区域
    bool _showing_animation = false;
    int current_index = -1; // 当前索引
    bool using_keyboard = false; // 是否正在使用键盘挑选菜单
};

#endif // FACILEMENU_H
