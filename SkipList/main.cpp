//main.cpp
#include "mainwindow.h"
#include "skiplist.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();                       //显示主窗口
    return a.exec();
}
