#include "mainwindow.h"
#include "serialport.h"
#include <QApplication>
#include <QTextCodec>

#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
    return 0;
}
