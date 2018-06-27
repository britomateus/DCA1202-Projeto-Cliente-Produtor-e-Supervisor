#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QLabel>
#include <QLineEdit>
#include <QDateTime>
#include <QListWidget>
#include <QWidget>
#include <QSlider>
#include <vector>
#include<plotter.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    idTimer = 0;

    connect(ui->pushButtonGet,
            SIGNAL(clicked(bool)),
            this,
            SLOT(getData()));
    connect(ui->pushButton_Stop,
            SIGNAL(clicked(bool)),
            this,
            SLOT(stopData()));
    connect(ui->pushButton_Connect,
            SIGNAL(clicked(bool)),
            this,
            SLOT(tcpConnect()));
    connect(ui->pushButton_Disconnect,
            SIGNAL(clicked(bool)),
            this,
            SLOT(tcpDisconnect()));
    connect(ui->pushButton_Update,
            SIGNAL(clicked(bool)),
            this,
            SLOT(updateIp()));
}

void MainWindow::tcpConnect(){
    socket->connectToHost(ui->lineEdit_Host->text(),1234);
    if(socket->waitForConnected()){
        qDebug() << "Connected";
    }
    else{
        qDebug() << "Disconnected";
    }
}

void MainWindow::tcpDisconnect(){
    socket->disconnectFromHost();
}


void MainWindow::getData(){
    if(idTimer){
        killTimer(idTimer);
    }
    idTimer = startTimer(ui->horizontalSlider_Time->value()*1000);

}

void MainWindow::timerEvent(QTimerEvent *e){
    QString str;
    QStringList list;
    qint64 thetime, num;
    double max_x, min_x, min_y, max_y;
    std::vector<double> time;
    std::vector<double> data;
    std::vector<double> temposnorm;
    std::vector<double> dadosnorm;

    qDebug() << "to get data...";
    if(socket->state() == QAbstractSocket::ConnectedState){
        if(socket->isOpen()){
            qDebug() << "reading...";
            str = "get " + ui->listWidget_ListaDeClients->currentItem()->text() + " 30\r\n";
            socket->write(str.toStdString().c_str());
            socket->waitForBytesWritten();
            socket->waitForReadyRead();
            qDebug() << socket->bytesAvailable();
            time.clear();
            data.clear();
            while(socket->bytesAvailable()){
                str = socket->readLine().replace("\n","").replace("\r","");
                list = str.split(" ");

                if(list.size() == 2){
                    bool ok;
                    str = list.at(0);
                    thetime = str.toLongLong(&ok);
                    time.push_back(thetime);

                    str = list.at(1);
                    num = str.toLongLong(&ok);
                    data.push_back(num);
                    qDebug() << thetime << ": " << str;
                }
            }
        }
    }

    qDebug()<<data.size()<<time.size();
    //achando valores maximos e minimos
    max_x = time[0], min_x = time[0];
    min_y = data[0], max_y = data[0];

    for(int i = 1 ; i < 30; i++){
        if(time[i] < min_x){
            min_x = time[i];
        }
        else if(time[i] > max_x){
            max_x = time[i];
        }
        if(data[i] < min_y){
            min_y = data[i];
        }
        else if(data[i] > max_y){
            max_y = data[i];
        }
    }

    qDebug()<<max_x-min_x;


    qDebug()<<max_y<<min_y;


    //normalizando dados
    temposnorm.clear();
    dadosnorm.clear();
    for(int i = 0; i<30; i++){
        temposnorm.push_back((time[i] - min_x)/(max_x - min_x));
        dadosnorm.push_back((data[i] - min_y)/(max_y - min_y));
    }
    qDebug()<<"passou";
    ui->widget_Plotter->loadData(temposnorm,dadosnorm);
}

void MainWindow::stopData(void){
    if(idTimer){
        killTimer(idTimer);
    }
}

void MainWindow::updateIp(void){
    QString str;

    ui->listWidget_ListaDeClients->clear();
    if(socket->state() == QAbstractSocket::ConnectedState){
        socket->write("list\r\n");
        socket->waitForBytesWritten();
        socket->waitForReadyRead();
        while(socket->bytesAvailable()){
            str = socket->readLine().replace("\n","").replace("\r","");
            ui->listWidget_ListaDeClients->addItem(str);
        }
    }
    else{
        ui->listWidget_ListaDeClients->addItem("Lista vazia");
    }

}


MainWindow::~MainWindow()
{
    delete socket;
    delete ui;
}
