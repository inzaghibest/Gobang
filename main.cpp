#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("欢乐五子棋");
    w.setWindowIcon(QIcon(":image/images/exeIcon.jfif"));
    w.show();

    return a.exec();
}
