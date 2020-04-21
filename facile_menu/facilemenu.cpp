#include "facilemenu.h"

FacileMenu::FacileMenu(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose, true);
    setFocusPolicy(Qt::StrongFocus); // 获取焦点，允许按键点击
    setWindowFlag(Qt::Popup, true);

    main_vlayout = new QVBoxLayout(this);
    setLayout(main_vlayout);
    main_vlayout->setEnabled(true);
    main_vlayout->setMargin(0);
    main_vlayout->setSpacing(0);
}

FacileMenu::FacileMenu(bool sub, QWidget *parent) : FacileMenu(parent)
{
    _is_sub_menu = sub;
}

FacileMenuItem *FacileMenu::addAction(QIcon icon, QString text, FuncType func)
{
    FacileMenuItem* item = new FacileMenuItem(icon, text, this);
    item->setKey(getShortcutByText(text));

    setActionButton(item);
    main_vlayout->addWidget(item);

    connect(item, &InteractiveButtonBase::clicked, this, [=]{
        func();
        close();
    });
    return item;
}

FacileMenuItem *FacileMenu::addAction(QString text, FuncType func)
{
    FacileMenuItem* item = new FacileMenuItem(text, this);
    item->setKey(getShortcutByText(text));

    setActionButton(item);
    main_vlayout->addWidget(item);

    connect(item, &InteractiveButtonBase::clicked, this, [=]{
        func();
        close();
    });
    return item;
}

/**
 * 添加一项子菜单
 * 鼠标浮在上面展开
 * 同时也可以设置点击事件
 */
FacileMenu *FacileMenu::addMenu(QIcon icon, QString text, FuncType func)
{
    FacileMenuItem* item = new FacileMenuItem(icon, text, this);
    item->setKey(getShortcutByText(text));

    setActionButton(item);
    main_vlayout->addWidget(item);

    connect(item, &InteractiveButtonBase::clicked, this, [=]{
        func();
        close();
    });

    FacileMenu* menu = new FacileMenu(this);
    menu->hide();
    item->setSubMenu(menu);
    return menu;
}

FacileMenu *FacileMenu::addMenu(QString text, FuncType func)
{
    FacileMenuItem* item = new FacileMenuItem(text, this);
    item->setKey(getShortcutByText(text));

    setActionButton(item);
    main_vlayout->addWidget(item);

    connect(item, &InteractiveButtonBase::clicked, this, [=]{
        func();
        close();
    });

    FacileMenu* menu = new FacileMenu(this);
    menu->hide();
    item->setSubMenu(menu);
    return menu;
}

void FacileMenu::execute(QPoint pos)
{
    if (pos == QPoint(-1,-1))
        pos = QCursor::pos();
    // TODO: 判断有没有超出屏幕边界

    move(pos);
    QWidget::show();
    setFocus();
}

Qt::Key FacileMenu::getShortcutByText(QString text)
{
    Qt::Key key = Qt::Key_Exit;
    QRegularExpression re("&([\\d\\w])");
    auto match = re.match(text);
    if (match.hasMatch())
    {
        const QChar ch = match.capturedTexts().at(1).at(0); // 快捷键字符串
        if (ch >= '0' && ch <= '9')
            key = (Qt::Key)(Qt::Key_0 + (ch.toLatin1() - '0'));
        else if (ch >= 'a' && ch <= 'z')
            key = (Qt::Key)(Qt::Key_A + (ch.toUpper().toLatin1() - 'A'));
        else if (ch >= 'A' && ch <= 'Z')
            key = (Qt::Key)(Qt::Key_A + (ch.toUpper().toLatin1() - 'A'));
    }
    return key;
}

void FacileMenu::setActionButton(InteractiveButtonBase *btn)
{
    // 设置尺寸
    btn->setPaddings(8, 48, 8, 8);

    // TODO:设置颜色
}
