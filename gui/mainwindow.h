#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileInfo>
#include <QTextEdit>
#include <QDockWidget>
#include <QMenuBar>
#include <QString>

#include "videowidget.h"

class MainWindow: public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = 0);
  ~MainWindow();

private:
  void setupWidgets();
  void setupMenus();

public slots:
  void stateChanged(int);
  void openFile();

private:
  void processCommands(QString commands);

private:
  QDockWidget *commandDock;
  QTextEdit *commandWidget;
  VideoWidget *videoBox;
  QMenuBar *menus;
  
  
};


#endif 
