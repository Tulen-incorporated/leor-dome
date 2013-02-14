#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QObject>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Фиксируем высоту окошка.
    this->setFixedHeight(this->height());

    int i;

    // инициализируем все углы на 110 градусов.
    for (i = 0; i < ANGLES_COUNT; i++){
        angles[i] = 110;
    }

    // Инициализируем уставочные дельты
    // Это углы, на которые надо изменять текущие углы
    // по тику таймера.

    // Сейчас сделаем все дельты на единичку, впоследствии, если понадобиться
    // мы можем сделать их разными для каждого сегмента,
    // если захочется вращать его быстрее или медленнее.
    for (i = 0; i < ANGLES_COUNT; i++)
    {
        absoluteDeltas[i] = 1;
    }

    // Инициализируем текущие дельты, это значения, на которые БУДУТ изменятся
    // наши углы по тику таймера. Поскольку кнопок тут еще никто не нажимает, зануляем их.
    for (i = 0; i < ANGLES_COUNT; i++)
    {
        actualDeltas[i] = 0;
    }

    // Теперь настраиваем таймер.
    timer.setInterval(100); //100 милисекунд на переполнение.
    timer.setSingleShot(false); // выключаем остановку после первого переполнения.
    // А теперь соединяем сигнал переполнения с нашим слотом.
    connect(&timer, SIGNAL(timeout()), this, SLOT(timerSlot()));
    timer.start(); // погнале!
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_W:
        actualDeltas[0] = absoluteDeltas[0];
        break;

    case Qt::Key_S:
        actualDeltas[0] = -absoluteDeltas[0];
        break;
//......................................... верх низ основание

    case Qt::Key_A:
        actualDeltas[1] = absoluteDeltas[1];
        break;

    case Qt::Key_D:
        actualDeltas[1] = -absoluteDeltas[1];
        break;
//......................................... право лево основание
    case Qt::Key_Up:
        actualDeltas[2] = absoluteDeltas[2];
        break;

    case Qt::Key_Down:
        actualDeltas[2] = -absoluteDeltas[2];
        break;
//......................................... верх низ локоть
    case Qt::Key_PageUp:
        actualDeltas[3] = absoluteDeltas[3];
        break;

    case Qt::Key_PageDown:
        actualDeltas[3] = -absoluteDeltas[3];
        break;
//......................................... кисть верх низ
    case Qt::Key_Left:
        actualDeltas[4] = absoluteDeltas[4];
        break;

    case Qt::Key_Right:
        actualDeltas[4] = -absoluteDeltas[4];
        break;
//......................................... кисть левоправо
    case Qt::Key_Space:
        actualDeltas[5] = absoluteDeltas[5];
        break;

    case Qt::Key_X:
        actualDeltas[5] = -absoluteDeltas[5];
        break;
//......................................... захват +-
     } //switch
}


void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    // Все тоже самое, но все дельты зануляем.
    switch (event->key())
    {
    case Qt::Key_W:
    case Qt::Key_S:
        actualDeltas[0] = 0;
        break;
//......................................... верх низ основание

    case Qt::Key_A:
    case Qt::Key_D:
        actualDeltas[1] = 0;
        break;
//......................................... право лево основание
    case Qt::Key_Up:
    case Qt::Key_Down:
        actualDeltas[2] = 0;
        break;
//......................................... верх низ локоть
    case Qt::Key_PageUp:
    case Qt::Key_PageDown:
        actualDeltas[3] = 0;
        break;
//......................................... кисть верх низ
    case Qt::Key_Left:
    case Qt::Key_Right:
        actualDeltas[4] = 0;
        break;
//......................................... кисть левоправо
    case Qt::Key_Space:
    case Qt::Key_X:
        actualDeltas[5] = 0;
        break;
//......................................... захват +-
     } //switch
}

// Слот для таймера, тут все просто. Как только таймер переполняется, меняем углы на текущие дельты.
void MainWindow::timerSlot()
{
    // TODO тут надо сделать что-то, чтобы углы не уходили за пределы, типо 200 градусов, или -10
    int i;
    for (i = 0; i < ANGLES_COUNT; i++)
    {
        angles[i] += actualDeltas[i];
    }
    ui->label_7->setText(QString::number(angles[0]));
    ui->label_8->setText(QString::number(angles[1]));
    ui->label_9->setText(QString::number(angles[2]));
    ui->label_10->setText(QString::number(angles[3]));
    ui->label_11->setText(QString::number(angles[4]));
    ui->label_12->setText(QString::number(angles[5]));

    //qDebug() << angles[0] << angles[1] << angles[2] << angles[3] << angles[4] << angles[5];
    //TODO я забыл обновить лейблы (
}

void MainWindow::on_connectButton_clicked()
{
    this->grabKeyboard();
    QString portName = ui->portNameBox->itemText(0);
    port.open(portName);
}

void MainWindow::on_disconnectButton_clicked()
{
    this->releaseKeyboard();
}
