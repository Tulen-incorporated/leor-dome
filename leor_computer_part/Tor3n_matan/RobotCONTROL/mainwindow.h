#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QLineEdit>
#include <QStringList>
#include <QKeyEvent>
#include <QTimer>

#include "serialport.h"

// Количество углов, с которыми работаем.
#define ANGLES_COUNT 8

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    
private:
    Ui::MainWindow *ui;

    // Хранимые углы.
    int angles[ANGLES_COUNT];

    // Дельты углов по таймеру.
    int actualDeltas[ANGLES_COUNT];

    // Уставки дельт по таймеру.
    int absoluteDeltas[ANGLES_COUNT];

    // Таймер, который будет менять углы.
    QTimer timer;
    LeorSerial port;

private slots:
    // Слот для таймера.
    void timerSlot();

    void on_connectButton_clicked();
    void on_disconnectButton_clicked();
};






#endif // MAINWINDOW_H
