#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>

#include "window.h"


int main (int argc, char *argv[])
{
  QApplication app (argc, argv);

  QMainWindow *window = new QMainWindow;
  QMenuBar *tool_bar = new QMenuBar (window);
  Window *graph_area = new Window (window);
  QAction *action;

  if (graph_area->parse_command_line (argc, argv))
    {
      QMessageBox::warning (0, "Wrong input arguments!",
                            "Wrong input arguments!");
      return -1;
    }

  action = tool_bar->addAction ("Change function", graph_area, SLOT (change_func()));
  action->setShortcut (QString ("0"));

  action = tool_bar->addAction ("Exit", window, SLOT (close ()));
  action->setShortcut (QString ("Ctrl+X"));

  action = tool_bar->addAction ("Change structure", graph_area, SLOT (change_struct_func()));
  action->setShortcut (QString("1"));

  action = tool_bar->addAction ("Stretch", graph_area, SLOT(stretch_X()));
  action->setShortcut(QString("2"));

  action = tool_bar->addAction ("Compress", graph_area, SLOT(compress_X()));
  action->setShortcut(QString("3"));

  action = tool_bar->addAction ("Increase split", graph_area, SLOT(increase_split()));
  action->setShortcut(QString("4"));

  action = tool_bar->addAction ("Decrease split ", graph_area, SLOT(decrease_split()));
  action->setShortcut(QString("5"));

  tool_bar->setMaximumHeight (30);

  window->setMenuBar (tool_bar);
  window->setCentralWidget (graph_area);
  window->setWindowTitle ("Graph");

  window->show ();
  app.exec ();
  delete window;
  return 0;
}