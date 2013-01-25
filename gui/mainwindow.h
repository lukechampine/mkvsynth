#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <VideoPlayer>
#include <QTextEdit>

class MainWindow: public QMainWindow
{
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = 0);
  ~MainWindow();
 private:
  QDockWidget *commandDock;
  QTextEdit *commandWidget;
  VideoPlayer *videoViewer;
  
  
};


#endif 
