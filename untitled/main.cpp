#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("IM View");
    MainWindow w;
    w.showMaximized();
    return a.exec();
}
