#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDebug>
#include<QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  
public slots:
    /**
    * @brief Realiza a conexão com o server
    */
    void tcpConnect();
    /**
    * @brief Desconecta do server
    */
    void tcpDisconnect();
    /**
    * @brief Função para enviar os dados
    */
    void putData();
    /**
    * @brief Encontra o ip fornecido pelo user
    */
    QString getHost();
    /**
    * @brief Para o envio dos dados
    */
    void stopData();
    /**
    * @brief Repete o envio de dados [putData()] de acordo com o intervalo de tempo definido pelo user
    */
    void timerEvent(QTimerEvent *e);
private:
  Ui::MainWindow *ui;
  QTcpSocket *socket;
  int idTimer;

};

#endif // MAINWINDOW_H
