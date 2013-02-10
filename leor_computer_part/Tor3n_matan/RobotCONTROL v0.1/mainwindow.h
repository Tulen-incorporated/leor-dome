#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QWidget>
#include <QLineEdit>
#include <QStringList>
#include <QKeyEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int a;
    int b;
    int c;
    int d;
    int e;
    int f;

    void keyPressEvent(QKeyEvent *);
    
private:
    Ui::MainWindow *ui;


};






#endif // MAINWINDOW_H
