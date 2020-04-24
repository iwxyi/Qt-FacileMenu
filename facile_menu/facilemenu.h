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

    FacileMenuItem* addAction(QIcon icon, QString text, FuncType func = nullptr);
    FacileMenuItem* addAction(QIcon icon, FuncType func = nullptr);
    FacileMenuItem* addAction(QString text, FuncType func = nullptr);
    FacileMenuItem* addAction(QAction* action, bool deleteWithMenu = true);
    FacileMenuItem* addAction(QIcon icon, QString text, void (*func)());
    template <class T>
    FacileMenuItem* addAction(QIcon icon, QString text, T *obj, void (T::*func)());

    FacileMenu* addRow(FuncType func = []{});
    FacileMenu* beginRow();
    FacileMenu* endRow();
    QBoxLayout* currentLayout() const;

    FacileMenu* addMenu(QIcon icon, QString text, FuncType func = nullptr);
    FacileMenu* addMenu(QString text, FuncType func = nullptr);

    FacileMenu* addLayout(QLayout *layout, int stretch = 0);
    FacileMenu* addLayoutItem(QLayoutItem *item);
    FacileMenu* addSpacerItem(QSpacerItem *spacerItem);
    FacileMenu* addSpacing(int size);
    FacileMenu* addStretch(int stretch = 0);
    FacileMenu* addStrut(int size);
    FacileMenu* addWidget(QWidget *widget, int stretch = 0, Qt::Alignment alignment = Qt::Alignment());
    FacileMenu* setSpacing(int spacing);
    FacileMenu* setStretchFactor(QWidget *widget, int stretch);
    FacileMenu* setStretchFactor(QLayout *layout, int stretch);

    FacileMenu* addOptions(QList<QString>texts, QList<bool>states, FuncIntType func);
    FacileMenu* addOptions(QList<QString>texts, int select, FuncIntType func);

    FacileMenuItem* addSeparator();
    FacileMenu* split();

    void exec(QPoint pos = QPoint(-1, -1));
    void execute(QPoint pos = QPoint(-1, -1));
    void toHide(int focusIndex = -1);

    FacileMenu* uncheckAll(FacileMenuItem* except = nullptr, int begin = -1, int end = -1);

    FacileMenu* setTipArea(int x = 48);
    FacileMenu* setTipArea(QString longestTip);
    FacileMenu* setSplitInRow(bool split = true);

signals:
    void signalActionTriggered(FacileMenuItem* action);
    void signalHidden();

private slots:
    void itemMouseEntered(FacileMenuItem* item);

protected:
    FacileMenuItem* createMenuItem(QIcon icon, QString text);
    Qt::Key getShortcutByText(QString text) const;
    void setActionButton(InteractiveButtonBase* btn, bool isChip = false);
    void showSubMenu(FacileMenuItem* item);
    bool isCursorInArea(QPoint pos, FacileMenu* child = nullptr);
    void setKeyBoardUsed(bool use = true);
    bool isSubMenu() const;
    FacileMenuItem* addVSeparator();
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
    QList<QWidget*> other_widgets; // 手动添加的widget
    QVBoxLayout* main_vlayout;
    QList<QHBoxLayout*> row_hlayouts;
    QList<QAction*> import_actions;
    QPixmap bg_pixmap;

    FacileMenu* current_sub_menu = nullptr; // 当前打开（不一定显示）的子菜单
    FacileMenu* parent_menu = nullptr; // 父类的菜单

    int addin_tip_area = 0; // 右边用来显示提示文字的区域
    bool adding_horizone = false; // 是否正在添加横向菜单
    bool align_mid_if_alone = false; // 是否居中对齐，如果只有icon或text
    bool _showing_animation = false;
    int current_index = -1; // 当前索引
    bool using_keyboard = false; // 是否正在使用键盘挑选菜单
    bool split_in_row = false; // 同一行是否默认添加分割线
    int blur_bg_alpha = 33; // 背景图显示程度，0禁用，1~100为模糊透明度
};

#endif // FACILEMENU_H
