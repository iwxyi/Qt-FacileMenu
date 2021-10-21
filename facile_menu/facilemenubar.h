#ifndef FACILEMENUBAR_H
#define FACILEMENUBAR_H

#include <QObject>
#include <QWidget>
#include "facilemenubarinterface.h"
#include "facilemenu.h"

class FacileMenuBar : public QWidget, public FacileMenuBarInterface
{
    Q_OBJECT
public:
    explicit FacileMenuBar(QWidget *parent = nullptr);

    virtual int isCursorInArea(QPoint pos) const override;
    int currentIndex() const override;
    virtual bool triggerIfNot(int index, void*menu) override;

    void addMenu(QString name, FacileMenu* menu);


signals:
    void triggered();

public slots:
    virtual void trigger(int index) override;

private:
    QList<InteractiveButtonBase*> buttons;
    QList<FacileMenu*> menus;
    QHBoxLayout* hlayout;

    int _currentIndex = -1;
};

#endif // FACILEMENUBAR_H
