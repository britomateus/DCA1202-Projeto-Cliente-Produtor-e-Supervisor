#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include<QLineEdit>
#include<QTextBrowser>
#include<QTimer>
#include<QLabel>
#include<QLCDNumber>
 
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    socket = new QTcpSocket(this);
 
    connect(ui->pushButtonPut,
            SIGNAL(clicked(bool)),
            this,
            SLOT(putData()));
    connect(ui->pushButton_connect,
            SIGNAL(clicked(bool)),
            this,
            SLOT(tcpConnect()));
    connect(ui->pushButton_stop,
            SIGNAL(clicked(bool)),
            this,
            SLOT(stopData()));
    connect(ui->pushButton_disconnect,
            SIGNAL (clicked(bool)),
            this,
            SLOT(tcpDisconnect()));
}
 
void MainWindow::tcpConnect(){
    socket->connectToHost(getHost(),1234);
    if(socket->waitForConnected(3000)){
        qDebug() << "Connected";
    }
    else{
        qDebug() << "Disconnected";
    }
}
 
void MainWindow::tcpDisconnect(){
    socket->disconnectFromHost();
}
 
 
void MainWindow::putData(){
    QString timeStr;
 
    timeStr = ui->label_time->text();
    idTimer = startTimer((timeStr.toInt())*1000);
 
}
 
QString MainWindow::getHost(){
    QString ip;
 
    ip = ui->lineEdit_Host->text();
 
    return ip;
 
}
 
void MainWindow::stopData(){
    killTimer(idTimer);
}
 
void MainWindow::timerEvent(QTimerEvent *e){
    QString str;
    int max,min;
    qint64 msecdate;
 
    max = ui->lcdNumber_max->value();
    min = ui->lcdNumber_min->value();
 
    if(min>max){
        ui->textBrowser->insertPlainText("O valor minimo inserido é maior que o valor máximo\n");
        killTimer(idTimer);
    }
    else{
        if(socket->state()== QAbstractSocket::ConnectedState){
            msecdate = QDateTime::currentDateTime().toMSecsSinceEpoch();
 
            str = "set "+ QString::number(msecdate) + " " + QString::number(qrand()%(max - min) + min)+"\r\n";
            ui->textBrowser->append(str);
 
            qDebug() << str;
            qDebug() << socket->write(str.toStdString().c_str()) << " bytes written";
            if(socket->waitForBytesWritten(3000)){
                qDebug() << "wrote";
            }
        }
    }
}
 
MainWindow::~MainWindow(){
    delete socket;
    delete ui;
}
