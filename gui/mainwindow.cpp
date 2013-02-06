#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
  commandWidget = new QTextEdit;
  videoViewer = new VideoPlayer;
  commandDock = new QDockWidget;
  
  commandDock->setWidget(commandWidget);
  this->addDockWidget(Qt::TopDockWidgetArea,commandDock);
  this->setCentralWidget(videoViewer);
}

MainWindow::~MainWindow()
{
  delete commandDock;
}
