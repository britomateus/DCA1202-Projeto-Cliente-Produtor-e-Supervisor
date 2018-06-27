#ifndef PLOTTER_H
#define PLOTTER_H

#include <QWidget>
#include<vector>

class Plotter : public QWidget
{
  Q_OBJECT
private:
  std::vector<double> tempos;
  std::vector<double> dados;
  float time;
public:
  /** @brief Construtor do Plotter
  **/
  explicit Plotter(QWidget *parent = 0);
  /** @brief Desenha retas através de pares de pontos.
  **/
  void paintEvent(QPaintEvent *e);
  /** @brief Função que carrega os dados e os prepara para mostrar no gráfico
  **/
  void loadData(std::vector <double>,std::vector <double>);
};

#endif // PLOTTER_H
