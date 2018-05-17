#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QSettings>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDateTime>
#include <QDir>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void timerTimeout();    
    void on_saveButton_clicked();
    void on_response_valueChanged(int arg1);
    void on_timeout_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
    QTimer timer;
    bool failLog;
    void portWrite(char data);
};

#endif // MAINWINDOW_H
