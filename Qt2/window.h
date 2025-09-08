#ifndef WINDOW_H
#define WINDOW_H

#include <QtWidgets/QtWidgets>

class Window : public QWidget
{
  Q_OBJECT

private:
  int func_id;
  int structure;
  const char *f_name;
  double a_x;
  double a_y;
  double b_x;
  double b_y;
  int n_x;
  int n_y;
  int p;
  double (*f) (double, double);

public:
  Window (QWidget *parent);

  QSize minimumSizeHint () const;
  QSize sizeHint () const;

  int parse_command_line (int argc, char *argv[]);
  QPointF l2g (double x_loc, double y_loc, double y_min, double y_max);
public slots:
  void change_func_next();
  void change_func_back();
  void change_struct_func_next();
  void change_struct_func_back();
  void stretch_XY();
  void compress_XY();
  void increase_split();
  void decrease_split();
  void increase_perturbation();
  void decrease_perturbation();

protected:
  void paintEvent (QPaintEvent *event);
};

#endif