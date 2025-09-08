#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>

#include "window.h"


int main (int argc, char *argv[]) {
    QApplication app (argc, argv);

    QMainWindow *window = new QMainWindow;
    QMenuBar *tool_bar = new QMenuBar (window);
    Window *graph_area = new Window (window);
    QAction *action;

    if (graph_area->parse_command_line (argc, argv)) {
        QMessageBox::warning (0, "Wrong input arguments!", "Wrong input arguments!");
        return -1;
    }

    action = tool_bar->addAction ("Change function +1", graph_area, SLOT (change_func_next()));
    action->setShortcut (QString ("0"));

    action = tool_bar->addAction ("Change function -1", graph_area, SLOT (change_func_back()));
    action->setShortcut (QString ("9"));

    action = tool_bar->addAction ("Exit", window, SLOT (close ()));
    action->setShortcut (QString ("Ctrl+X"));

    action = tool_bar->addAction ("Change structure +1", graph_area, SLOT (change_struct_func_next()));
    action->setShortcut (QString("1"));

    action = tool_bar->addAction ("Change structure -1", graph_area, SLOT (change_struct_func_back()));
    action->setShortcut (QString("Q"));

    action = tool_bar->addAction ("Stretch", graph_area, SLOT(stretch_XY()));
    action->setShortcut(QString("2"));

    action = tool_bar->addAction ("Compress", graph_area, SLOT(compress_XY()));
    action->setShortcut(QString("3"));

    action = tool_bar->addAction ("Increase split", graph_area, SLOT(increase_split()));
    action->setShortcut(QString("4"));

    action = tool_bar->addAction ("Decrease split", graph_area, SLOT(decrease_split()));
    action->setShortcut(QString("5"));

    action = tool_bar->addAction ("Increase perturbation", graph_area, SLOT(increase_perturbation()));
    action->setShortcut(QString("6"));

    action = tool_bar->addAction ("Decrease perturbation", graph_area, SLOT(decrease_perturbation()));
    action->setShortcut(QString("7"));

    tool_bar->setMaximumHeight (30);

    window->setMenuBar (tool_bar);
    window->setCentralWidget (graph_area);
    window->setWindowTitle ("Graph");

    window->show ();
    app.exec ();
    delete window;
    return 0;
}