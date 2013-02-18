#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QLineEdit>
#include <QStringList>
#include <QKeyEvent>
#include <QTimer>

#include "serialport.h"
#include "leorMessage.h"

// Количество углов, с которыми работаем.

#define MAX_LOG_LINES 20

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
    int angles[SERVO_COUNT];

    // Дельты углов по таймеру.
    int actualDeltas[SERVO_COUNT];

    // Уставки дельт по таймеру.
    int absoluteDeltas[SERVO_COUNT];

    // Таймер, который будет менять углы.
    QTimer timer;
    LeorSerial port;

private slots:
    // Слот для таймера.
    void timerSlot();

    void on_connectButton_clicked();
    void on_disconnectButton_clicked();
    void on_scanPortsButton_clicked();
    void addLogMessage(const QString & message);
};






#endif // MAINWINDOW_H
