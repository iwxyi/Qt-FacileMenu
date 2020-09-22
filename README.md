FacileMenu
====

## 介绍

非常飘逸的 Qt 菜单控件，带有各种动画效果，用起来也十分方便。

无限层级，响应键盘、鼠标单独操作，支持单快捷键。

允许添加自定义 widget、layout，当做特殊的 QDialog 使用。



## 简单使用

1. 放入源代码
   将 `facile_menu` 文件夹放入 Qt 程序，pro 文件的 `INCLUDEPATH` 加上对应路径，`resources` 里的资源文件 `sub_menu_arrow.png` （子菜单箭头）也导入，前缀别名为：`:/icons/sub_menu_arrow`（或按需修改）

2. 包含头文件
   `#include "facile_menu.h"`

3. 创建并显示菜单

   ```C++
   // 创建菜单
   FacileMenu* menu = new FacileMenu(this);
   
   // 添加动作
   menu->addAction(QIcon(":/icons/run"), "开始播放 (&S)", [=]{
       /* 某操作 */
   })->tip("Ctrl+S")->disable()->hide();
   
   // 显示菜单
   menu->execute(QCursor::pos());
   ```




## 常用操作

### 连续设置

```C++
menu->addAction(QIcon(":/icons/run"), "开始播放 (&S)", [=]{})
    ->tip("Ctrl+S")
    ->disable(playing/*如果满足某条件(默认true)则disable，不满足跳过，下同*/)
    ->hide(/*条件表达式*/)
    ->uncheck(false);
```



### 子菜单

```C++
auto subMenu = menu->addMenu("子菜单2");

subMenu->addAction("继续", [=]{});

subMenu3->addAction("停止", [=]{})
        ->disable(!playing);
```



### 横向菜单

方式一：一口气添加

```C++
menu->addRow([=]{
    menu->addAction("按钮1");
    menu->addAction("按钮2");
    menu->addAction("按钮3");
});
```

方式二：逐个添加

```C++
menu->beginRow();
menu->addAction(QIcon(":/icons/run"));
menu->addAction(QIcon(":/icons/pause"));
menu->split();
menu->addAction(QIcon(":/icons/resume"));
menu->addAction(QIcon(":/icons/stop"))->disable();
menu->endRow();
```

两种方式都支持横向布局 widget



### 添加 QAction

支持在菜单关闭时自动 delete 传入的 action，避免内存泄漏（默认关闭）

```C++
QAction* action = ...;
menu->addAction(action, true/*是否在菜单关闭一起时delete*/);
```



### 添加 Widget/Layout

添加任意 widget 至菜单中，和菜单项并存。layout 同理。

```C++
QPushButton* button = new QPushButton("外部添加的按钮", this);
menu->addWidget(button);
```



### 添加单选菜单

> 如果要设置为checkable，请在创建时调用一次其以下任一方法：
>
> setCheckable(bool) / setChecked(bool) / check(bool) / uncheck(bool)

```C++
auto ac1 = subMenu2->addAction(QIcon(":/icons/run"), "带图标")->check()->linger();
auto ac2 = subMenu2->addAction("无图标")->uncheck()->linger();
auto ac3 = subMenu2->split()->addAction("全不选")->uncheck()->linger();

// 连接点击事件
ac1->triggered([=]{
    subMenu2->singleCheck(ac1); // 用于单选，表示只选中ac1
    // 这里可以用于处理其他操作
});
ac2->triggered([=]{
    subMenu2->singleCheck(ac2);
});
ac3->triggered([=]{
    subMenu2->uncheckAll(); // 全不选
});
```



### 添加多选菜单

```C++
// 假装是某一个需要多选的属性
QList<QString>* list = new QList<QString>();

for (int i = 0; i < 10; i++)
{
    auto action = subMenu5->addAction("选项"+QString::number(i))->uncheck()->linger()->autoAlter()/*点击自动切换选中状态*/;
    action->triggered([=]{
        // 自己的处理流程，例如调用某个外部的方法
        if (action->isChecked())
            list->append(action->getText());
        else
            list->removeOne(action->getText());
        qDebug() << "当前选中的有：" << *list;
    });
}
```



### 快速批量单选项

```C++
QStringList texts;
for (int i = 0; i < 10; i++)
    texts << "项目"+QString::number(i);
static int selected = 2;

menu->addOptions(texts, selected, [=](int index){
    qDebug() << "选中了：" << (selected = index) << texts.at(index);
});
```



### 快速批量多选项

监听每一项改变的结果

```C++
// 假装是某一个需要多选的属性
QList<int>* list = new QList<int>();
subMenu6->addNumberedActions("选项%1", 0, 10)
    ->setMultiCheck([=](int index, bool checked){
        if (checked)
            list->append(index);
        else
            list->removeOne(index);
        qDebug() << "当前选中的有：" << *list;
    });
```



### 极简批量多选项

直接读取多选项结果，而不是监听多选项每一项（也可以两者结合）

在`finished()`中获取`checkedItems()`，即为选中项

```C++
QList<QString>* list = new QList<QString>();
subMenu7->addNumberedActions("选项%1", 0, 15)->setMultiCheck()
    ->finished([=]{
        *list = subMenu7->checkedItemTexts();
        qDebug() << "最终选中：" << *list;
    });
```



### 菜单项 API

`addAction()`后，可直接设置菜单项的一些属性，包括以下：

> 第一个参数为`bool`类型的，表示**满足此条件才修改设置**，例如：
>
> ```C++
> bool needHide = false;
> action->hide(needHide); // 不满足隐藏条件，即无视此语句
> ```

```C++
// 菜单项右边快捷键区域的文字
// 如果要使用，建议用：setTipArea 来额外添加设置右边空白宽度
FacileMenuItem* tip(QString sc);
FacileMenuItem* tip(bool exp, QString sc);

// 鼠标悬浮提示
FacileMenuItem* tooltip(QString tt);
FacileMenuItem* tooltip(bool exp, QString tt);

// 触发（单击、回车键）后，参数为 Lambda 表达式
FacileMenuItem* triggered(FuncType func);
FacileMenuItem* triggered(bool exp, FuncType func);

// 当参数表达式为true时生效，false时忽略，下同
FacileMenuItem* disable(bool exp = true);
FacileMenuItem* enable(bool exp = true);

FacileMenuItem* hide(bool exp = true);
FacileMenuItem* visible(bool exp = true);

FacileMenuItem* check(bool exp = true);
FacileMenuItem* uncheck(bool exp = true);
FacileMenuItem* alter(bool exp = true);

// 设置data，一般用于单选、多选
FacileMenuItem* setData(QVariant data);
QVariant getData();

FacileMenuItem* text(bool exp, QString str);
// 当表达式为true时，设置为tru文字，否则设置为fal文字
FacileMenuItem* text(bool exp, QString tru, QString fal);

FacileMenuItem* fgColor(QColor color);
FacileMenuItem* fgColor(bool exp, QColor color);

FacileMenuItem* bgColor(QColor color);
FacileMenuItem* bgColor(bool exp, QColor color);

// 插入前缀
FacileMenuItem* prefix(bool exp, QString pfix);
FacileMenuItem* prefix(QString pfix);

// 插入后缀，参数3支持类似 "action后缀 (K)" 这样的格式
FacileMenuItem* suffix(bool exp, QString sfix, bool inLeftParenthesis = true);
FacileMenuItem* suffix(QString sfix, bool inLeftParenthesis = true);

FacileMenuItem* icon(bool ic, QIcon icon);

// 设置边界：半径、颜色
FacileMenuItem* borderR(int radius = 3, QColor co = Qt::transparent);

// 点击后是否保持菜单显示（默认点一下就隐藏菜单）
FacileMenuItem* linger();

// 满足 exp 时执行 trueLambda 表达式，否则执行 falseLambda 表达式
FacileMenuItem* ifer(bool exp, trueLambda, falseLambda = nullptr);

// 逻辑控制
FacileMenuItem* ifer(bool exp); // 满足条件时才继续，下同
FacileMenuItem* elifer(bool exp);
FacileMenuItem* elser();

FacileMenuItem* switcher(int value);
FacileMenuItem* caser(int value, matchedLambda); // 匹配时执行lambda，无需break
FacileMenuItem* caser(int value); // 结束记得breaker（允许忘掉~）
FacileMenuItem* breaker();
FacileMenuItem* defaulter();

// 取消后面所有命令（无视层级，相当于函数中return）
FacileMenuItem* exiter(bool ex = true);
```


> 注意：由于加了一些容错处理（例如caser可以不用写breaker），无法进行if/switch的多层嵌套（较多的逻辑运算不建议放在菜单中）



## 截图

![](picture.gif)
