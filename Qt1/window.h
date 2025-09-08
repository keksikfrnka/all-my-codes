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
  double a;
  double b;
  int n;
  double (*f) (double);

public:
  Window (QWidget *parent);

  QSize minimumSizeHint () const;
  QSize sizeHint () const;

  int parse_command_line (int argc, char *argv[]);
  QPointF l2g (double x_loc, double y_loc, double y_min, double y_max);
public slots:
  void change_func();;
  void change_struct_func();
  void stretch_X();
  void compress_X();
  void increase_split();
  void decrease_split();

protected:
  void paintEvent (QPaintEvent *event);
};

#endif