#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // Mac窗口
    qputenv("QT_MAC_WANTS_LAYER", "1");
    MainWindow w;
    w.show();
    return a.exec();
}
