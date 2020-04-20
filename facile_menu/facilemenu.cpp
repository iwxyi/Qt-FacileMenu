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

void FacileMenu::addAction(QIcon icon, QString text, FuncType func)
{
    actions.append(FacileAction{icon, text, getShortcutByText(text), false});

    InteractiveButtonBase* btn = new InteractiveButtonBase(text, this);
    btn->setIcon(icon);
    setActionButton(btn);
    main_vlayout->addWidget(btn);

    connect(btn, &InteractiveButtonBase::clicked, this, [=]{
        func();
        close();
    });
}

void FacileMenu::addAction(QString text, FuncType func)
{
    actions.append(FacileAction{QIcon(), text, getShortcutByText(text), false});

    InteractiveButtonBase* btn = new InteractiveButtonBase(text, this);
    setActionButton(btn);
    main_vlayout->addWidget(btn);

    connect(btn, &InteractiveButtonBase::clicked, this, [=]{
        func();
    });
}

/**
 * 添加一项子菜单
 * 鼠标浮在上面展开
 * 同时也可以设置点击事件
 */
FacileMenu *FacileMenu::addMenu(QIcon icon, QString text, FuncType &func)
{
    actions.append(FacileAction{QIcon(), text, getShortcutByText(text), true});

    InteractiveButtonBase* btn = new InteractiveButtonBase(text, this);
    btn->setIcon(icon);
    setActionButton(btn);
    main_vlayout->addWidget(btn);

    connect(btn, &InteractiveButtonBase::clicked, this, [=]{
        func();
    });
}

FacileMenu *FacileMenu::addMenu(QString text, FuncType &&func)
{
    actions.append(FacileAction{QIcon(), text, getShortcutByText(text), true});

    InteractiveButtonBase* btn = new InteractiveButtonBase(text, this);
    setActionButton(btn);
    main_vlayout->addWidget(btn);

    connect(btn, &InteractiveButtonBase::clicked, this, [=]{
        func();
    });
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
    btn->setPaddings(32, 48, 8, 8);

    // 设置颜色
}
