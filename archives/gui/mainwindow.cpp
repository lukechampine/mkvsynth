#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
  setupWidgets();
  setupMenus();
}

MainWindow::~MainWindow()
{
  delete commandDock;
  delete videoBox;
  delete menus;
}

void
MainWindow::setupWidgets()
// This function sets up all the widgets needed
{
  // allocates memory
  commandWidget = new QTextEdit;
  videoBox = new VideoWidget;
  commandDock = new QDockWidget;

  // sets up the dock widget
  commandDock->setWidget(commandWidget);
  commandDock->setAllowedAreas(Qt::TopDockWidgetArea);

  // assigns widget to correct places
  this->addDockWidget(Qt::TopDockWidgetArea,commandDock);
  this->setCentralWidget(videoBox);

  connect(videoBox,SIGNAL(qmpState(int)),this,SLOT(stateChanged(int)));
}

void
MainWindow::setupMenus()
// This function gets the menus to work as necessary
{
  menus = this->menuBar();

  QMenu *file = new QMenu("File");
  file->addAction("Open File",this,SLOT(openFile()));

  menus->addMenu(file);
}

void
MainWindow::openFile()
{
  QString fileName = QFileDialog::getOpenFileName(this,"Open File","Files (*.*)");

  setWindowTitle(QFileInfo(fileName).baseName());
  videoBox->openFile(fileName);
}

void
MainWindow::stateChanged(int newState)
{
  if ( newState == QMPwidget::PlayingState )
    {
      commandWidget->setReadOnly(true);
      processCommands(commandWidget->toPlainText());
    }
  else commandWidget->setReadOnly(false);
}

void
MainWindow::processCommands(QString commands)
{
  
}



