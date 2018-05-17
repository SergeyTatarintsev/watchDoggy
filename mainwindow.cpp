#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QDir::setCurrent(qApp->applicationDirPath());

    ui->setupUi(this);
    QSettings ini("./settings.ini",QSettings::IniFormat);
    ini.sync();
    for(int i=10;i>0;i--){
        ini.setValue(QString("start_%1").arg(i),ini.value(QString("start_%1").arg(i-1)));
        ini.setValue(QString("lastWoof_%1").arg(i),ini.value(QString("lastWoof_%1").arg(i-1)));
    }
    ini.setValue("start_0",QDateTime::currentDateTime().toString("dd/MM/yyyy-hh:mm:ss"));
    int i=0;int p=-1;
    foreach(QSerialPortInfo port,QSerialPortInfo::availablePorts()){
        ui->portList->insertItem(i++,QString("(%1) %2").arg(port.portName()).arg(port.description()),port.portName());
        if(ini.value("port").toString()==port.portName())
            p=i-1;
    }
    ui->portList->setCurrentIndex(p);
    failLog = ini.value("failLog",false).toBool();
    ui->response->setValue(ini.value("response",1).toInt());
    ui->timeout->setValue(ini.value("timer",200).toInt()*10);
    QObject::connect(&timer,SIGNAL(timeout()),this,SLOT(timerTimeout()));
    timer.setInterval(ui->response->value()*1000);
    timer.setSingleShot(false);
    timer.start();
}

MainWindow::~MainWindow()
{    
    delete ui;
}
void MainWindow::portWrite(char data){
    QSerialPort port(ui->portList->currentData().toString());
    int res=0;
    if(port.open(QSerialPort::WriteOnly))
        res = port.write(&data,1);
    if((res!=1||!port.isOpen())&&failLog){
        QSettings ini("./settings.ini",QSettings::IniFormat);
        ini.setValue("fails",QString("%1 %2").arg(ini.value("fails").toString()).arg(QDateTime::currentDateTime().toString("dd/MM/yyyy-hh:mm:ss")));
    }
    port.waitForBytesWritten(300);
    port.close();
}

void MainWindow::on_pushButton_clicked()
{
    portWrite(0xFF);
}

void MainWindow::timerTimeout()
{
    portWrite((char)(ui->timeout->value()/10));    
    QSettings ini("./settings.ini",QSettings::IniFormat);
    ini.setValue("lastWoof_0",QDateTime::currentDateTime().toString("dd/MM/yyyy-hh:mm:ss"));
    ui->statusBar->showMessage("Woof Woof!",500);
}

void MainWindow::on_saveButton_clicked()
{
    QSettings ini("./settings.ini",QSettings::IniFormat);
    ini.setValue("timer",char(ui->timeout->value()/10));
    ini.setValue("response",ui->response->value());
    ini.setValue("port",ui->portList->currentData().toString());
}

void MainWindow::on_response_valueChanged(int arg1)
{
    timer.setInterval(arg1*1000);
}

void MainWindow::on_timeout_valueChanged(int arg1)
{
    ui->timeout->setValue(int(arg1/10)*10);
}
