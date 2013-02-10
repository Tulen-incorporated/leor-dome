#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stdfix.h"
#include "windows.h"
#include "iostream"
#include <QObject>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
a=110; //основание вн
b=110; //основание пл
c=110; //локоть вн
d=110; //кисть вн
e=110; //кисть лп
f=110; //зохават
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::keyPressEvent(QKeyEvent *event)
{


QString str;

str = QString::number(a, 10);
ui->label_7->setText(str);
str = QString::number(b, 10);
ui->label_8->setText(str);
str = QString::number(c, 10);
ui->label_9->setText(str);
str = QString::number(d, 10);
ui->label_10->setText(str);
str = QString::number(e, 10);
ui->label_11->setText(str);
str = QString::number(f, 10);
ui->label_12->setText(str);


    switch (event->key())
    {
    case Qt::Key_W:
    a=a+1;
    str = QString::number(a, 10);
    ui->label_7->setText(str);
    break;

    case Qt::Key_S:
    a=a-1;
    str = QString::number(a, 10);
    ui->label_7->setText(str);
    break;
//......................................... верх низ основание

    case Qt::Key_A:
    b=b+1;
    str = QString::number(b, 10);
    ui->label_8->setText(str);
    break;

    case Qt::Key_D:
    b=b-1;
    str = QString::number(b, 10);
    ui->label_8->setText(str);
    break;
//......................................... право лево основание
    case Qt::Key_Up:
    c=c+1;
    str = QString::number(c, 10);
    ui->label_9->setText(str);
    break;

    case Qt::Key_Down:
    c=c-1;
    str = QString::number(c, 10);
    ui->label_9->setText(str);
    break;
//......................................... верх низ локоть
    case Qt::Key_PageUp:
    d=d+1;
    str = QString::number(d, 10);
    ui->label_10->setText(str);
    break;

    case Qt::Key_PageDown:
    d=d-1;
    str = QString::number(d, 10);
    ui->label_10->setText(str);
    break;
//......................................... кисть верх низ
    case Qt::Key_Left:
    e=e+1;
    str = QString::number(e, 10);
    ui->label_11->setText(str);
    break;

    case Qt::Key_Right:
    e=e-1;
    str = QString::number(e, 10);
    ui->label_11->setText(str);
    break;
//......................................... кисть левоправо
    case Qt::Key_Space:
    f=f+1;
    str = QString::number(f, 10);
    ui->label_12->setText(str);
    break;

    case Qt::Key_X:
    f=f-1;
    str = QString::number(f, 10);
    ui->label_12->setText(str);
    break;
//......................................... захват +-
     }

}
