#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDebug>

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
    /** @brief Realiza conexão com o server
    **/
  void tcpConnect(void);
  /** @brief Desconecta do server
  **/
  void tcpDisconnect(void);
  /** @brief Captura dados do servidor de acordo com o tempo determinado pelo user
  **/
  void getData(void);
  /** @brief Para a captura de dados
  **/
  void stopData(void);
  /** @brief Mostra uma lista dos clientes conectados
  **/
  void updateIp(void);
  /** @brief Função que determina o tempo da captura de dados definido pelo user
  **/
  void timerEvent(QTimerEvent *e);

private:
  Ui::MainWindow *ui;
  QTcpSocket *socket;
  int idTimer;
};

#endif // MAINWINDOW_H
